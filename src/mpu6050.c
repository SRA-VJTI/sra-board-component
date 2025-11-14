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
#include "i2cdev.h"

static const char *TAG_MPU = "mpu_6050";
static i2c_dev_t mpu6050_dev_t;

#define MIN_ACCE_ERROR 5
#define MIN_GYRO_ERROR 5
#define MAX_CALIBRATION_ATTEMPTS 20
#define G_RAW_VALUE 16384  //accelerometer reads this value for 1 g acceleration.

static int16_t acce_raw_value_offset[BUFF_SIZE / 2] = {0, 0, 0}, gyro_raw_value_offset[BUFF_SIZE / 2] = {0, 0, 0};

// Initialise the I2C bus and install driver to specified pins
esp_err_t i2c_master_init(void)
{
    CHECK(i2cdev_init());

    mpu6050_dev_t.port = I2C_MASTER_NUM;
    mpu6050_dev_t.addr = MPU6050_ADDR;
    mpu6050_dev_t.cfg.mode = I2C_MODE_MASTER;
    mpu6050_dev_t.cfg.sda_io_num = I2C_MASTER_SDA_IO;
    mpu6050_dev_t.cfg.scl_io_num = I2C_MASTER_SCL_IO;
    mpu6050_dev_t.cfg.master.clk_speed = I2C_MASTER_FREQ_HZ;

    return i2c_dev_create_mutex(&mpu6050_dev_t);
}

// Initialise and power ON, MPU6050
esp_err_t enable_mpu6050(void)
{
    CHECK(i2c_master_init());
    I2C_DEV_TAKE_MUTEX(&mpu6050_dev_t);

    // Set Clock Source
    uint8_t value;
    I2C_DEV_CHECK(&mpu6050_dev_t, i2c_dev_read_reg(&mpu6050_dev_t, MPU6050_RA_PWR_MGMT_1, &value, 1));
    value = value & 0xf8;
    value = value | MPU6050_CLOCK_PLL_XGYRO;
    I2C_DEV_CHECK(&mpu6050_dev_t, i2c_dev_write_reg(&mpu6050_dev_t, MPU6050_RA_PWR_MGMT_1, &value, 1));

    // Set fullscale gyro range
    value = MPU6050_GYRO_FS_250;
    I2C_DEV_CHECK(&mpu6050_dev_t, i2c_dev_write_reg(&mpu6050_dev_t, MPU6050_RA_GYRO_CONFIG, &value, 1));

    // Set fullscale accel range
    value = MPU6050_ACCEL_FS_2;
    I2C_DEV_CHECK(&mpu6050_dev_t, i2c_dev_write_reg(&mpu6050_dev_t, MPU6050_RA_ACCEL_CONFIG, &value, 1));

    // Set Sleep Mode to false
    value = 0x00;
    I2C_DEV_CHECK(&mpu6050_dev_t, i2c_dev_read_reg(&mpu6050_dev_t, MPU6050_RA_PWR_MGMT_1, &value, 1));
    value = value & 0x9f; // also check 0xbf
    I2C_DEV_CHECK(&mpu6050_dev_t, i2c_dev_write_reg(&mpu6050_dev_t, MPU6050_RA_PWR_MGMT_1, &value, 1));

    // Add MPU6050_RA_WHO_AM_I

    I2C_DEV_GIVE_MUTEX(&mpu6050_dev_t);

    return ESP_OK;
}

// Read accelerometer values
esp_err_t mpu6050_read_acce(uint8_t *data_rd, size_t size)
{
    if (size == 0)
    {
        return ESP_OK;
    }

    I2C_DEV_TAKE_MUTEX(&mpu6050_dev_t);
    I2C_DEV_CHECK(&mpu6050_dev_t, i2c_dev_read_reg(&mpu6050_dev_t, ACCE_START_ADDR, data_rd, size));
    I2C_DEV_GIVE_MUTEX(&mpu6050_dev_t);

    return ESP_OK;
}

// Read gyroscope values
esp_err_t mpu6050_read_gyro(uint8_t *data_rd, size_t size)
{
    if (size == 0)
    {
        return ESP_OK;
    }

    I2C_DEV_TAKE_MUTEX(&mpu6050_dev_t);
    I2C_DEV_CHECK(&mpu6050_dev_t, i2c_dev_read_reg(&mpu6050_dev_t, GYRO_START_ADDR, data_rd, size));
    I2C_DEV_GIVE_MUTEX(&mpu6050_dev_t);

    return ESP_OK;
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

esp_err_t avg_sensors(int16_t *acce_raw_value_avg, int16_t *gyro_raw_value_avg, const int16_t *acce_offs, const int16_t *gyro_offs)
{
	int16_t curr_acce_raw_value[BUFF_SIZE / 2], curr_gyro_raw_value[BUFF_SIZE / 2];
	long acce_raw_value_sum[BUFF_SIZE / 2] = {0, 0, 0}, gyro_raw_value_sum[BUFF_SIZE / 2] = {0, 0, 0};
	int i, j, k;
	for (i = 0; i < MPU_CALIBRATION_AVG_COUNT; i++)
	{
		if (read_mpu6050_raw(curr_acce_raw_value, curr_gyro_raw_value) != ESP_OK)
		{
			ESP_LOGE(TAG_MPU, "%s", "Failed to read MPU for calibration !");
			return ESP_FAIL;
		}

		for (j = 0; j < BUFF_SIZE / 2; j++)
		{
			acce_raw_value_sum[j] += curr_acce_raw_value[j] - acce_offs[j];
			gyro_raw_value_sum[j] += curr_gyro_raw_value[j] - gyro_offs[j];
		}
	}

	for (k = 0; k < BUFF_SIZE / 2; k++)
	{
		acce_raw_value_avg[k] = acce_raw_value_sum[k] / MPU_CALIBRATION_AVG_COUNT;
		gyro_raw_value_avg[k] = gyro_raw_value_sum[k] / MPU_CALIBRATION_AVG_COUNT;
	}

	return ESP_OK;
}

// Calculate mpu_offset for calibration.
esp_err_t calibrate_mpu6050()
{
	int16_t acce_raw_value_avg[BUFF_SIZE / 2] = {0, 0, 0}, gyro_raw_value_avg[BUFF_SIZE / 2] = {0, 0, 0};
	int8_t i, offset_ready;

	if (avg_sensors(acce_raw_value_avg, gyro_raw_value_avg, acce_raw_value_offset, gyro_raw_value_offset) != ESP_OK)
		return ESP_FAIL;

	gyro_raw_value_offset[0] = gyro_raw_value_avg[0];
	gyro_raw_value_offset[1] = gyro_raw_value_avg[1];
	gyro_raw_value_offset[2] = gyro_raw_value_avg[2];

	acce_raw_value_offset[0] = acce_raw_value_avg[0];
	acce_raw_value_offset[1] = acce_raw_value_avg[1];
	acce_raw_value_offset[2] = (G_RAW_VALUE - acce_raw_value_avg[2]);

	for (i = 0, offset_ready = 0; i < MAX_CALIBRATION_ATTEMPTS && offset_ready != 6; i++)
	{
		if (avg_sensors(acce_raw_value_avg, gyro_raw_value_avg, acce_raw_value_offset, gyro_raw_value_offset) != ESP_OK)
			return ESP_FAIL;

		if(abs(gyro_raw_value_avg[0]) <= MIN_GYRO_ERROR) offset_ready++;
		else gyro_raw_value_offset[0] += gyro_raw_value_avg[0];

		if(abs(gyro_raw_value_avg[1]) <= MIN_GYRO_ERROR) offset_ready++;
		else gyro_raw_value_offset[1] += gyro_raw_value_avg[1];

		if(abs(gyro_raw_value_avg[2]) <= MIN_GYRO_ERROR) offset_ready++;
		else gyro_raw_value_offset[2] += gyro_raw_value_avg[2];

		if(abs(acce_raw_value_avg[0]) <= MIN_ACCE_ERROR) offset_ready++;
		else acce_raw_value_offset[0] += acce_raw_value_avg[0];

		if(abs(acce_raw_value_avg[1]) <= MIN_ACCE_ERROR) offset_ready++;
		else acce_raw_value_offset[1] += acce_raw_value_avg[1];

		if(abs(G_RAW_VALUE - acce_raw_value_avg[2]) <= MIN_ACCE_ERROR) offset_ready++;
		else acce_raw_value_offset[2] += acce_raw_value_avg[2] - G_RAW_VALUE;

		vTaskDelay(10 / portTICK_PERIOD_MS);
	}

	if (i == MAX_CALIBRATION_ATTEMPTS)
	{
		ESP_LOGW(TAG_MPU, "Maximum calibration attemps limit exceeded, quiting calibration.");
		return ESP_FAIL;
	}

	ESP_LOGI("accelerometer offset values: ", "%d | %d | %d", acce_raw_value_offset[0], acce_raw_value_offset[1], acce_raw_value_offset[2]);
	ESP_LOGI("gyroscope offset values: ", "%d | %d | %d", gyro_raw_value_offset[0], gyro_raw_value_offset[1], gyro_raw_value_offset[2]);
	return ESP_OK;
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

