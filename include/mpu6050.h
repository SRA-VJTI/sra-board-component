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

#ifndef _MPU6050_H
#define _MPU6050_H

#include <stdio.h>
#include <math.h>
#include <string.h>

#include "sdkconfig.h"
#include "esp_attr.h"

#include "esp_log.h"
#include "esp_err.h"

#include "freertos/FreeRTOS.h"
#include "driver/i2c.h"
#include "driver/gpio.h"
#include "esp_timer.h"

#include "pin_defs.h"

#define I2C_MASTER_SCL_IO MPU6050_SCL /*!< gpio number for I2C master clock */
#define I2C_MASTER_SDA_IO MPU6050_SDA /*!< gpio number for I2C master data  */
#define I2C_MASTER_NUM I2C_NUM_1      /*!< I2C port number for master dev */
#define I2C_MASTER_TX_BUF_DISABLE 0   /*!< I2C master do not need buffer */
#define I2C_MASTER_RX_BUF_DISABLE 0   /*!< I2C master do not need buffer */
#define I2C_MASTER_FREQ_HZ 100000     /*!< I2C master clock frequency */

#define MPU6050_RA_PWR_MGMT_1 0x6B
#define MPU6050_CLOCK_PLL_XGYRO 0x01

#define MPU6050_RA_GYRO_CONFIG 0x1B
#define MPU6050_GYRO_FS_250 0x00

#define MPU6050_RA_ACCEL_CONFIG 0x1C
#define MPU6050_ACCEL_FS_2 0x00

#define MPU6050_ADDR 0x68   /*!< slave address for mpu6050 sensor */
#define ACCE_START_ADDR 0x3B /*!< accelerometer start address */
#define GYRO_START_ADDR 0x43 /*!< gyroscope start address */

#define WRITE_BIT I2C_MASTER_WRITE /*!< I2C master write */
#define READ_BIT I2C_MASTER_READ   /*!< I2C master read */
#define ACK_CHECK_EN 0x1           /*!< I2C master will check ack from slave*/
#define ACK_CHECK_DIS 0x0          /*!< I2C master will not check ack from slave */
#define ACK_VAL 0x0                /*!< I2C ack value */
#define NACK_VAL 0x1               /*!< I2C nack value */

#define ALPHA 0.9834
#define RAD_TO_DEG 57.2957795
#define BUFF_SIZE 6

#define MPU_CALIBRATION_AVG_COUNT CONFIG_MPU_CALIBRATION_AVG_COUNT

/**
 * @brief Initialise the ESP32 I2C Driver in Master Mode
 * 
 * @return esp_err_t returns ESP_OK if I2C driver initialised properly, ESP_ERR_INVALID_ARG for Parameter error, ESP_FAIL Driver for install error 
 */
esp_err_t i2c_master_init();

/**
 * @brief Initialise MPU-6050 (by sending the appropriate queued commands); 
 * Refer this for more information: https://invensense.tdk.com/wp-content/uploads/2015/02/MPU-6000-Datasheet1.pdf
 * 
 * @return esp_err_t returns ESP_OK if MPU-6050 initialised successfully, else the appropriate error code
 * Refer this for more info on ESP32 I2C Error codes: https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/i2c.html
 */
esp_err_t enable_mpu6050(void);

/**
 * @brief Get the accelerometer raw values (Ax, Ay, Az) into an 8-bit array
 * Note that these raw values are actually 16-bit, split into MSB (Byte) and LSB (Byte), sent one after the other
 * @param data_rd Buffer array for storing raw values
 * @param size Size of the buffer array (6 in our case)
 * @return esp_err_t returns ESP_OK if acceleromter read successfully, else the appropriate error code 
 */
esp_err_t mpu6050_read_acce(uint8_t *data_rd, size_t size);

/**
 * @brief Get the gyroscope raw values (Gx, Gy, Gz) into an 8-bit array
 * Note that these raw values are actually 16-bit, split into MSB (Byte) and LSB (Byte), sent one after the other
 * @param data_rd Buffer array for storing raw values
 * @param size Size of the buffer array (6 in our case)
 * @return esp_err_t returns ESP_OK if gyroscope read successfully, else the appropriate error code
 */
esp_err_t mpu6050_read_gyro(uint8_t *data_rd, size_t size);

/**
 * @brief Combine two 8-bit values to a 16-bit one
 * Note that EVEN indices in the input buffer represent the MSB (Byte)
 * 
 * @param buf_1 8-bit Input array of size 6
 * @param buf_2 16-bit Output array of size 3
 */
void combine_msb_lsb_raw_data(uint8_t *buf_1, int16_t *buf_2);

/**
 * @brief Compute the Euler angle (Pitch and Roll) from the accelerometer raw values
 * Axes and Angles - X: Roll, Y: Pitch, Z: Yaw
 * 
 * @param ax Raw accelerometer value (X-axis)
 * @param ay Raw accelerometer value (Y-axis)
 * @param az Raw accelerometer value (Z-axis)
 * @param acce_angle Resultant angle array 
 */
void compute_acce_angle(int16_t ax, int16_t ay, int16_t az, float *acce_angle);

/**
 * @brief Compute the Euler angle (Pitch and Roll) from the gyroscope raw values
 * Axes and Angles - X: Roll, Y: Pitch, Z: Yaw
 * More on this here: https://philsal.co.uk/projects/imu-attitude-estimation
 * @param gx Raw gyroscope value (X-axis)
 * @param gy Raw gyroscope value (Y-axis)
 * @param gz Raw gyroscope value (Z-axis)
 * @param dt Sampling time for gyroscope readings (interval between 2 readings)
 * @param gyro_angle Resultant angle array
 */
void compute_gyro_angle(int16_t gx, int16_t gy, int16_t gz, float dt, float *gyro_angle);

/**
 * @brief A wrapper for reading the 8-bit raw values from MPU and then combining them to their final form 
 * i.e. 16-bit
 * 
 * @param acce_raw_value 16-bit array for storing the accelerometer raw values
 * @param gyro_raw_value 16-bit array for storing the gyroscope raw values
 * @return esp_err_t returns ESP_OK if successful, else the appropriate error code
 */
esp_err_t read_mpu6050_raw(int16_t *acce_raw_value, int16_t *gyro_raw_value);

/**
 * @brief Fuse the gyroscope and accelerometer angle in a complementary fashion
 * More on this here: https://robotics.stackexchange.com/questions/10746/complimentary-filter-issues
 * @param acce_raw_value Raw values from the accelerometer
 * @param gyro_raw_value Raw values from the gyroscope
 * @param complementary_angle Resultant fused and filtered angle
 * @param mpu_offset Offset of the MPU (accelerometer) at rest position
 */
void complementary_filter(int16_t *acce_raw_value, int16_t *gyro_raw_value, float *complementary_angle, float *mpu_offset);

/**
 * @brief The ultimate function (application ready); takes in the input raw values and initial conditions and gives out the complementary pitch and roll angles
 * 
 * @param euler_angle Input array of angles to store the results in (passed by reference)
 * @param mpu_offset Initial conditions for the accelerometer and gyroscope (Angle at rest position)
 * @return esp_err_t returns ESP_OK if successful, else the appropriate error code
 */
esp_err_t read_mpu6050(float *euler_angle, float *mpu_offset);

/**
 * @brief Function to calculate the MPU offset for raw values.
 * @return esp_err_t returns ESP_OK if successful, else ESP_FAIL
 */
esp_err_t calibrate_mpu6050();

/**
 * @brief Helper function for the function calibrate_mpu6050() to calculate the average of the raw values.
 * @param acce_raw_value_avg Input array of accelerometer raw values (passed by reference) to be filled by the function.
 * @param gyro_raw_value_avg Input array of gyroscope raw values (passed by reference) to be filled by the function.
 * @param acce_offs Offset to be applied to the accelerometer raw values.
 * @param gyro_offs Offset to be applied to the gyroscope raw values.
 * @return esp_err_t returns ESP_OK if successful, else ESP_FAIL
 */
esp_err_t avg_sensors(int16_t *acce_raw_value_avg, int16_t *gyro_raw_value_avg, const int16_t *acce_offs, const int16_t *gyro_offs);

#endif
