/*
 * MIT License
 *
 * Copyright (c)  2026 Society of Robotics and Automation
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

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "sra_board.h"
#include "i2cdev.h"

// LSA: raw ADC range and the range the readings are mapped to
#define BLACK_MARGIN 4095
#define WHITE_MARGIN 0
#define CONSTRAIN_LSA_LOW 0
#define CONSTRAIN_LSA_HIGH 1000

// Motor driver: duty cycle (0-100) and how long each step of the test lasts
#define MOTOR_TEST_SPEED 60
#define MOTOR_RUN_MS 2000
#define MOTOR_STOP_MS 1000

// Servos: sweep range in degrees and the delay between each degree
#define SERVO_SWEEP_MAX_DEGREE 90
#define SERVO_STEP_MS 50
#define SERVO_PAUSE_MS 1000

// LED matrix: number of blinks, how long the LEDs stay on and off, and the pause before blinking again
#define LED_MATRIX_BLINKS 3
#define LED_MATRIX_ON_MS 500
#define LED_MATRIX_OFF_MS 500
#define LED_MATRIX_PAUSE_MS 2000

// MPU: raw accelerometer value for 1 g at the default +-2 g full scale, and how often it is logged
#define G_RAW_VALUE 16384.0f
#define MPU_LOG_MS 500
#define MPU_RETRY_MS 1000
#define MPU6050_WHO_AM_I_REG 0x75

static const char *TAG_LSA = "test_lsa";
static const char *TAG_MOTOR = "test_motor";
static const char *TAG_SERVO = "test_servo";
static const char *TAG_LED_MATRIX = "test_led_matrix";
static const char *TAG_MPU = "test_mpu";

#ifdef CONFIG_ENABLE_OLED
// Set once at startup, before the tasks start, if the OLED responded
static bool oled_ready = false;
#endif

// enable_servo() looks these up by name, so they must be global and named servo_a to servo_d
servo_config servo_a = {
    .servo_pin = SERVO_A,
    .min_pulse_width = CONFIG_SERVO_A_MIN_PULSEWIDTH,
    .max_pulse_width = CONFIG_SERVO_A_MAX_PULSEWIDTH,
    .max_degree = CONFIG_SERVO_A_MAX_DEGREE,
};

servo_config servo_b = {
    .servo_pin = SERVO_B,
    .min_pulse_width = CONFIG_SERVO_B_MIN_PULSEWIDTH,
    .max_pulse_width = CONFIG_SERVO_B_MAX_PULSEWIDTH,
    .max_degree = CONFIG_SERVO_B_MAX_DEGREE,
};

servo_config servo_c = {
    .servo_pin = SERVO_C,
    .min_pulse_width = CONFIG_SERVO_C_MIN_PULSEWIDTH,
    .max_pulse_width = CONFIG_SERVO_C_MAX_PULSEWIDTH,
    .max_degree = CONFIG_SERVO_C_MAX_DEGREE,
};

servo_config servo_d = {
    .servo_pin = SERVO_D,
    .min_pulse_width = CONFIG_SERVO_D_MIN_PULSEWIDTH,
    .max_pulse_width = CONFIG_SERVO_D_MAX_PULSEWIDTH,
    .max_degree = CONFIG_SERVO_D_MAX_DEGREE,
};

/*
 * Reads the LSA and draws the five readings as bars on the OLED.
 * All OLED (LVGL) calls stay in this task because LVGL is not thread safe.
 */
static void lsa_oled_task(void *arg)
{
    adc_handle_t line_sensor;
    ESP_ERROR_CHECK(enable_line_sensor(&line_sensor));

    line_sensor_array line_sensor_readings;
    int loop_count = 0;

    while (1)
    {
        line_sensor_readings = read_line_sensor(line_sensor);
        for (int i = 0; i < 5; i++)
        {
            // constrain lsa readings between BLACK_MARGIN and WHITE_MARGIN
            line_sensor_readings.adc_reading[i] = bound(line_sensor_readings.adc_reading[i], WHITE_MARGIN, BLACK_MARGIN);
            // map readings from (WHITE_MARGIN, BLACK_MARGIN) to (CONSTRAIN_LSA_LOW, CONSTRAIN_LSA_HIGH)
            line_sensor_readings.adc_reading[i] = map(line_sensor_readings.adc_reading[i], WHITE_MARGIN, BLACK_MARGIN, CONSTRAIN_LSA_LOW, CONSTRAIN_LSA_HIGH);
            line_sensor_readings.adc_reading[i] = 1000 - line_sensor_readings.adc_reading[i];
        }

#ifdef CONFIG_ENABLE_OLED
        // Taller bar = more light reflected back (white surface)
        if (oled_ready)
        {
            display_lsa(line_sensor_readings);
        }
#endif

        // Log about twice a second so the motor and MPU logs stay readable
        if (loop_count++ % 10 == 0)
        {
            ESP_LOGI(TAG_LSA, "LSA_0: %d \t LSA_1: %d \t LSA_2: %d \t LSA_3: %d \t LSA_4: %d", line_sensor_readings.adc_reading[0], line_sensor_readings.adc_reading[1], line_sensor_readings.adc_reading[2], line_sensor_readings.adc_reading[3], line_sensor_readings.adc_reading[4]);
        }
        vTaskDelay(pdMS_TO_TICKS(50));
    }
}

/*
 * Runs one motor forward, stops, runs it backward and stops again, then
 * releases it.
 *
 * The motors and the servos all use MCPWM group 0, which has three timers.
 * The servos take two of them, so only one motor can hold the third one at
 * a time. Releasing the motor after its test frees that timer for the other.
 */
static void test_motor(int motor_id, const char *name)
{
    motor_handle_t motor;
    ESP_ERROR_CHECK(enable_motor_driver(&motor, motor_id));

    ESP_LOGI(TAG_MOTOR, "%s forward", name);
    set_motor_speed(motor, MOTOR_FORWARD, MOTOR_TEST_SPEED);
    vTaskDelay(pdMS_TO_TICKS(MOTOR_RUN_MS));

    ESP_LOGI(TAG_MOTOR, "%s stop", name);
    set_motor_speed(motor, MOTOR_STOP, 0);
    vTaskDelay(pdMS_TO_TICKS(MOTOR_STOP_MS));

    ESP_LOGI(TAG_MOTOR, "%s backward", name);
    set_motor_speed(motor, MOTOR_BACKWARD, MOTOR_TEST_SPEED);
    vTaskDelay(pdMS_TO_TICKS(MOTOR_RUN_MS));

    ESP_LOGI(TAG_MOTOR, "%s stop", name);
    set_motor_speed(motor, MOTOR_STOP, 0);
    vTaskDelay(pdMS_TO_TICKS(MOTOR_STOP_MS));

    // The timer has to be disabled before it can be deleted
    ESP_ERROR_CHECK(motor->disable(motor));
    ESP_ERROR_CHECK(motor->del(motor));
}

/*
 * Tests both motors of the normal mode motor driver, one after the other, forever.
 */
static void motor_task(void *arg)
{
    while (1)
    {
        test_motor(MOTOR_A_0, "MOTOR_A_0");
        test_motor(MOTOR_A_1, "MOTOR_A_1");
    }
}

/*
 * Sweeps all four servos from 0 to 90 degrees and back, forever.
 */
static void servo_task(void *arg)
{
    ESP_ERROR_CHECK(enable_servo());

    while (1)
    {
        ESP_LOGI(TAG_SERVO, "Sweeping 0 -> %d degrees", SERVO_SWEEP_MAX_DEGREE);
        for (int i = 0; i < SERVO_SWEEP_MAX_DEGREE; i++)
        {
            set_angle_servo(&servo_a, i);
            set_angle_servo(&servo_b, i);
            set_angle_servo(&servo_c, i);
            set_angle_servo(&servo_d, i);
            vTaskDelay(pdMS_TO_TICKS(SERVO_STEP_MS));
        }

        ESP_LOGI(TAG_SERVO, "Sweeping %d -> 0 degrees", SERVO_SWEEP_MAX_DEGREE);
        for (int i = SERVO_SWEEP_MAX_DEGREE; i > 0; i--)
        {
            set_angle_servo(&servo_a, i);
            set_angle_servo(&servo_b, i);
            set_angle_servo(&servo_c, i);
            set_angle_servo(&servo_d, i);
            vTaskDelay(pdMS_TO_TICKS(SERVO_STEP_MS));
        }
        vTaskDelay(pdMS_TO_TICKS(SERVO_PAUSE_MS));
    }
}

/*
 * Blinks every LED of the matrix three times, pauses and repeats, to check
 * that the ESP32 can turn each of the 64 LEDs on and off.
 */
static void led_matrix_task(void *arg)
{
    led_matrix_t matrix;
    ESP_ERROR_CHECK(led_matrix_init(&matrix));
    ESP_ERROR_CHECK(led_matrix_set_brightness(&matrix, 4));

    // one byte per row, one bit per LED
    static const uint8_t all_on[LED_MATRIX_HEIGHT] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

    while (1)
    {
        ESP_LOGI(TAG_LED_MATRIX, "Blinking all LEDs %d times", LED_MATRIX_BLINKS);
        for (int blink = 0; blink < LED_MATRIX_BLINKS; blink++)
        {
            ESP_ERROR_CHECK(led_matrix_draw_bitmap(&matrix, all_on));
            ESP_ERROR_CHECK(led_matrix_show(&matrix));
            vTaskDelay(pdMS_TO_TICKS(LED_MATRIX_ON_MS));

            ESP_ERROR_CHECK(led_matrix_clear(&matrix));
            vTaskDelay(pdMS_TO_TICKS(LED_MATRIX_OFF_MS));
        }
        vTaskDelay(pdMS_TO_TICKS(LED_MATRIX_PAUSE_MS));
    }
}

/*
 * Reads the accelerometer and logs the acceleration along each axis in g.
 */
static void mpu_task(void *arg)
{
    // Wait until the MPU answers on the I2C bus before calling enable_mpu6050(),
    // since that creates a new mutex every time it is called. This way a
    // missing or badly seated sensor shows up clearly and the other tests keep running
    i2c_dev_t mpu_probe = {
        .port = I2C_MASTER_NUM,
        .addr = MPU6050_ADDR,
        .cfg = {
            .mode = I2C_MODE_MASTER,
            .sda_io_num = I2C_MASTER_SDA_IO,
            .scl_io_num = I2C_MASTER_SCL_IO,
            .master.clk_speed = I2C_MASTER_FREQ_HZ,
        },
    };
    uint8_t who_am_i;
    ESP_ERROR_CHECK(i2cdev_init());
    while (i2c_dev_read_reg(&mpu_probe, MPU6050_WHO_AM_I_REG, &who_am_i, 1) != ESP_OK)
    {
        ESP_LOGE(TAG_MPU, "MPU did not respond at I2C address 0x%02X, retrying...", MPU6050_ADDR);
        vTaskDelay(pdMS_TO_TICKS(MPU_RETRY_MS));
    }

    if (enable_mpu6050() != ESP_OK)
    {
        ESP_LOGE(TAG_MPU, "MPU initialization failed");
        vTaskDelete(NULL);
    }
    ESP_LOGI(TAG_MPU, "MPU initialized");

    uint8_t acce_bytes[BUFF_SIZE];
    int16_t acce_raw_value[BUFF_SIZE / 2];

    while (1)
    {
        if (mpu6050_read_acce(acce_bytes, sizeof(acce_bytes)) == ESP_OK)
        {
            combine_msb_lsb_raw_data(acce_bytes, acce_raw_value);
            // Lying flat, Z reads about 1 g; tilting the board moves it to X and Y
            ESP_LOGI(TAG_MPU, "Accel (g) X: %0.2f \t Y: %0.2f \t Z: %0.2f", acce_raw_value[0] / G_RAW_VALUE, acce_raw_value[1] / G_RAW_VALUE, acce_raw_value[2] / G_RAW_VALUE);
        }
        else
        {
            ESP_LOGE(TAG_MPU, "Error reading accelerometer!");
        }
        vTaskDelay(pdMS_TO_TICKS(MPU_LOG_MS));
    }
}

#ifdef CONFIG_ENABLE_OLED
/*
 * Checks that the OLED answers on the I2C bus. init_oled() stops the program
 * if the OLED does not respond, so it is only called once this check passes.
 */
static bool oled_found(void)
{
    i2c_dev_t oled_probe = {
        .port = I2C_NUM_1,
        .addr = OLED_I2C_ADDRESS,
        .cfg = {
            .mode = I2C_MODE_MASTER,
            .sda_io_num = OLED_SDA,
            .scl_io_num = OLED_SCL,
            .sda_pullup_en = GPIO_PULLUP_ENABLE,
            .scl_pullup_en = GPIO_PULLUP_ENABLE,
            .master.clk_speed = OLED_IIC_FREQ_HZ,
        },
    };
    // A lone control byte (command mode, no command) does not change the display
    const uint8_t control_byte = OLED_CONTROL_BYTE_CMD_STREAM;

    return i2cdev_init() == ESP_OK && i2c_dev_write(&oled_probe, NULL, 0, &control_byte, 1) == ESP_OK;
}
#endif

void app_main(void)
{
#ifdef CONFIG_ENABLE_OLED
    // Shows the SRA and Wall-E logos, then clears the screen. A missing OLED
    // is reported instead of stopping the tests of the other components
    if (oled_found())
    {
        init_oled();
        oled_ready = true;
        vTaskDelay(100);
        lv_obj_clean(lv_scr_act());
    }
    else
    {
        ESP_LOGE(TAG_LSA, "OLED did not respond at I2C address 0x%02X (SDA: GPIO %d, SCL: GPIO %d), LSA readings will only be logged", OLED_I2C_ADDRESS, OLED_SDA, OLED_SCL);
    }
#endif

    xTaskCreate(lsa_oled_task, "lsa_oled_task", 4096, NULL, 1, NULL);
    xTaskCreate(motor_task, "motor_task", 4096, NULL, 1, NULL);
    xTaskCreate(servo_task, "servo_task", 4096, NULL, 1, NULL);
    xTaskCreate(led_matrix_task, "led_matrix_task", 4096, NULL, 1, NULL);
    xTaskCreate(mpu_task, "mpu_task", 4096, NULL, 1, NULL);
}
