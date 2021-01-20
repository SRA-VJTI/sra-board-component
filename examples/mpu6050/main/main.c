/*
 * Copyright (c) 2021 Society of Robotics and Automation.
 *
 * This file is part of SRA Board Component 
 * (see https://www.sravjti.in).
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
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
