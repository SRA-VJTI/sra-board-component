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

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#include "sdkconfig.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "sra_board.h"

static const char *TAG = "mpu_example";

void app_main()
{
    // Enable swtiches for switching between the different graphs we need to plot
    enable_switches();
    
    // Enable the MPU-6050 Sensor
    enable_mpu6050();

    // Setting up the result arrays, time difference and initial conditions
    float acce_angle[2], gyro_angle[2], complementary_angle[2], dt;
    float mpu_offset[2] = {0.0f, 0.0f};

    int16_t acce_raw_value[BUFF_SIZE / 2], gyro_raw_value[BUFF_SIZE / 2];
    uint32_t timer = esp_timer_get_time();

    // Read the MPU raw values
    while (read_mpu6050_raw(acce_raw_value, gyro_raw_value) == ESP_OK)
    {
        // Calculate the time difference between two samples
        dt = (float)(esp_timer_get_time() - timer) / 1000000;
        timer = esp_timer_get_time();
        
        // Get the angles as per the accelerometer raw values only
        compute_acce_angle(acce_raw_value[0], acce_raw_value[1], acce_raw_value[2], acce_angle);
        // Get the angles computed as per the gyroscope raw values only
        compute_gyro_angle(gyro_raw_value[0], gyro_raw_value[1], gyro_raw_value[2], dt, gyro_angle);
        // Get fused angles due to both the accelerometer and gyroscope
        complementary_filter(acce_raw_value, gyro_raw_value, complementary_angle, mpu_offset);

        // Plotting the different output plots
        if (read_switch(SWITCH_1))
            printf("%f \n", acce_angle[1]);
        else if (read_switch(SWITCH_2))
            printf("%f \n", gyro_angle[1]);
        else if (read_switch(SWITCH_3))
            printf("%f \n", complementary_angle[1]);
        else
            printf("%f \t  %f \t  %f \n ", acce_angle[1], gyro_angle[1], complementary_angle[1]);
    }

    ESP_LOGE(TAG, "%s", "Error reading values!");
}
