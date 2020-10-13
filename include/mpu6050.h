#ifndef _MPU6050_H
#define _MPU6050_H

#include <stdio.h>
#include <math.h>

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

#define MPU6050_ADDR 0x68   /*!< slave address for mpu6050 sensor */
#define ACCE_START_ADD 0x3B //Accelerometer start address
#define GYRO_START_ADD 0x43 //gyroscope start address

#define WRITE_BIT I2C_MASTER_WRITE /*!< I2C master write */
#define READ_BIT I2C_MASTER_READ   /*!< I2C master read */
#define ACK_CHECK_EN 0x1           /*!< I2C master will check ack from slave*/
#define ACK_CHECK_DIS 0x0          /*!< I2C master will not check ack from slave */
#define ACK_VAL 0x0                /*!< I2C ack value */
#define NACK_VAL 0x1               /*!< I2C nack value */

#define ALPHA 0.9834
#define RAD_TO_DEG 57.2957795
#define BUFF_SIZE 6

esp_err_t i2c_master_init();

esp_err_t mpu6050_init(i2c_port_t i2c_num);

void start_mpu();

esp_err_t mpu6050_read_acce(i2c_port_t i2c_num, uint8_t *data_rd, size_t size);

esp_err_t mpu6050_read_gyro(i2c_port_t i2c_num, uint8_t *data_rd, size_t size);

void combine_msb_lsb_raw_data(uint8_t *buf_1, int16_t *buf_2);

esp_err_t complementary_filter(int16_t *acce_raw_value, int16_t *gyro_raw_value, float complementary_angle[], float initial_acce_angle);

esp_err_t compute_angle(uint8_t *acce_rd, uint8_t *gyro_rd, int16_t *acce_raw_value, int16_t *gyro_raw_value, float initial_acce_angle, float *roll_angle, float *pitch_angle);

#endif