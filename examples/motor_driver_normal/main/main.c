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
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "sra_board.h"

static const char* TAG_SWITCH_CONTROL_MOTOR = "switch_control";

void app_main(void)
{
    enable_motor_driver(a, NORMAL_MODE); // Enable motor driver A in Normal Mode
    while(1)
    {
        // Rotate Motors A_0 and A_1 in FORWARD direction
        set_motor_speed(MOTOR_A_0, MOTOR_FORWARD, 80);
        ESP_LOGI(TAG_SWITCH_CONTROL_MOTOR, "MOTOR_A_0 FORWARD");	
        set_motor_speed(MOTOR_A_1, MOTOR_FORWARD, 80);
		ESP_LOGI(TAG_SWITCH_CONTROL_MOTOR, "MOTOR_A_1 FORWARD");
        
        vTaskDelay(2000 / portTICK_PERIOD_MS);

        // Rotate Motors A_0 and A_1 in BACKWARD direction
        set_motor_speed(MOTOR_A_0, MOTOR_BACKWARD, 80);
		ESP_LOGI(TAG_SWITCH_CONTROL_MOTOR, "MOTOR_A_0 BACKWARD");
        set_motor_speed(MOTOR_A_1, MOTOR_BACKWARD, 80);
		ESP_LOGI(TAG_SWITCH_CONTROL_MOTOR, "MOTOR_A_1 BACKWARD");

        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
}
