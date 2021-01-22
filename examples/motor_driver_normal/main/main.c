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
