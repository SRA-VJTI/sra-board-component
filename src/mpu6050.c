/*
 * MIT License
 *
 * Copyright (c)  2021 Society of Robotics and Automation
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "mpu6050.h"

static const char *TAG_MPU = "mpu_6050";

// Initialise the I2C bus and install driver to specified pins
esp_err_t i2c_master_init(void)
{
    int i2c_master_port = I2C_MASTER_NUM;
    i2c_config_t conf;
    conf.mode = I2C_MODE_MASTER;
    conf.sda_io_num = I2C_MASTER_SDA_IO;
    conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf.scl_io_num = I2C_MASTER_SCL_IO;
    conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
    conf.master.clk_speed = I2C_MASTER_FREQ_HZ;
    i2c_param_config(i2c_master_port, &conf);
    esp_err_t ret = i2c_driver_install(i2c_master_port, conf.mode, I2C_MASTER_RX_BUF_DISABLE, I2C_MASTER_TX_BUF_DISABLE, 0);
    if (ret != ESP_OK)
        ESP_LOGE(TAG_MPU, "I2C Master Initialisation Failed!");

    return ret;
}

// Initialise and power ON, MPU6050
esp_err_t enable_mpu6050(void)
{
    CHECK(i2c_master_init());

    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (MPU6050_ADDR << 1) | WRITE_BIT, ACK_CHECK_EN);
    i2c_master_write_byte(cmd, 0x6B, ACK_CHECK_EN);
    i2c_master_write_byte(cmd, 0x00, ACK_CHECK_EN);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
    return ret;
}

// Read accelerometer values
esp_err_t mpu6050_read_acce(uint8_t *data_rd, size_t size)
{
    if (size == 0)
        return ESP_OK;
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (MPU6050_ADDR << 1) | WRITE_BIT, ACK_CHECK_EN);
    i2c_master_write_byte(cmd, ACCE_START_ADD, ACK_CHECK_EN);
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (MPU6050_ADDR << 1) | READ_BIT, ACK_CHECK_EN);
    if (size > 1)
        i2c_master_read(cmd, data_rd, size - 1, ACK_VAL);

    i2c_master_read_byte(cmd, data_rd + size - 1, NACK_VAL);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
    return ret;
}

// Read gyroscope values
esp_err_t mpu6050_read_gyro(uint8_t *data_rd, size_t size)
{
    if (size == 0)
        return ESP_OK;

    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (MPU6050_ADDR << 1) | WRITE_BIT, ACK_CHECK_EN);
    i2c_master_write_byte(cmd, GYRO_START_ADD, ACK_CHECK_EN);
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (MPU6050_ADDR << 1) | READ_BIT, ACK_CHECK_EN);
    if (size > 1)
        i2c_master_read(cmd, data_rd, size - 1, ACK_VAL);

    i2c_master_read_byte(cmd, data_rd + size - 1, NACK_VAL);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, 1000 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
    return ret;
}

// Combine the MSB and LSB values (8-bit) to a single value (16-bit)
void combine_msb_lsb_raw_data(uint8_t *buf_1, int16_t *buf_2)
{
    buf_2[0] = ((buf_1[0] << 8) + buf_1[1]);
    buf_2[1] = ((buf_1[2] << 8) + buf_1[3]);
    buf_2[2] = ((buf_1[4] << 8) + buf_1[5]);
}

// Notations in the following functions: Index 0 - Roll angle; Index 1 - Pitch angle

// Compute the accelerometer angle using the raw data
void compute_acce_angle(int16_t ax, int16_t ay, int16_t az, float *acce_angle)
{
    acce_angle[0] = atan2(ay, sqrt(pow(ax, 2) + pow(az, 2))) * RAD_TO_DEG;
    acce_angle[1] = atan2(-ax, sqrt(pow(ay, 2) + pow(az, 2))) * RAD_TO_DEG;
}

// Compute the gyroscope angle using the raw data
void compute_gyro_angle(int16_t gx, int16_t gy, int16_t gz, float dt, float *gyro_angle)
{
    // (1 / 131) sensitivity factor of Gyroscope: 1 degree rotation gives a reading of 131 units
    gx = gx / 131;
    gy = gy / 131;
    gz = gz / 131;

    gyro_angle[0] = gx * dt;
    gyro_angle[1] = gy * dt;

    /*
        In case the roll angle varies widely when only the pitch angle changes, activate the following equations.
        gyro_angle[0] = dt * (gx + gy * sin(gyro_angle[0]) * tan(gyro_angle[1]) + gz * cos(gyro_angle[0]) * tan(gyro_angle[1]));
        gyro_angle[1] = dt * (gy * cos(gyro_angle[0]) - gz * sin(gyro_angle[0]));
    */
}

esp_err_t read_mpu6050_raw(int16_t *acce_raw_value, int16_t *gyro_raw_value)
{
    esp_err_t err = ESP_FAIL;
    uint8_t acce_rd[BUFF_SIZE], gyro_rd[BUFF_SIZE];

    if (mpu6050_read_acce(acce_rd, BUFF_SIZE) == ESP_OK && mpu6050_read_gyro(gyro_rd, BUFF_SIZE) == ESP_OK)
        err = ESP_OK;

    combine_msb_lsb_raw_data(acce_rd, acce_raw_value);
    combine_msb_lsb_raw_data(gyro_rd, gyro_raw_value);

    return err;
}

// Fuse the gyroscope and accelerometer angle in a complementary fashion
void complementary_filter(int16_t *acce_raw_value, int16_t *gyro_raw_value, float *complementary_angle, float *mpu_offset)
{
    static bool is_initial_reading = true;
    static uint32_t timer;

    float acce_angle[2], gyro_angle[2];
    float dt;

    int i = 0;

    if (is_initial_reading)
    {
        is_initial_reading = false;
        compute_acce_angle(acce_raw_value[0], acce_raw_value[1], acce_raw_value[2], acce_angle);

        for (i = 0; i < 2; i++)
            complementary_angle[i] = acce_angle[i] - mpu_offset[i];

        timer = esp_timer_get_time();
        return;
    }

    dt = (float)(esp_timer_get_time() - timer) / 1000000;
    timer = esp_timer_get_time();

    compute_acce_angle(acce_raw_value[0], acce_raw_value[1], acce_raw_value[2], acce_angle);
    compute_gyro_angle(gyro_raw_value[0], gyro_raw_value[1], gyro_raw_value[2], dt, gyro_angle);

    for (i = 0; i < 2; i++)
    {
        acce_angle[i] = acce_angle[i] - mpu_offset[i];
        complementary_angle[i] = ALPHA * (complementary_angle[i] + gyro_angle[i]) + (1 - ALPHA) * acce_angle[i];
    }
}

// Calculate roll and pitch angles of the MPU after applying the complementary filter
esp_err_t read_mpu6050(float *euler_angle, float *mpu_offset)
{
    esp_err_t err = ESP_OK;
    int16_t acce_raw_value[BUFF_SIZE / 2], gyro_raw_value[BUFF_SIZE / 2];
    static float complementary_angle[2];

    if (read_mpu6050_raw(acce_raw_value, gyro_raw_value) != ESP_OK)
    {
        ESP_LOGE(TAG_MPU, "%s", "Failed to read MPU!");
        err = ESP_FAIL;
    }

    complementary_filter(acce_raw_value, gyro_raw_value, complementary_angle, mpu_offset);
    memcpy(euler_angle, complementary_angle, 2 * sizeof(float));

    return err;
}
