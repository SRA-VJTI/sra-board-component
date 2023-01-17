/*
 * MIT License
 *
 * Copyright (c)  2023 Society of Robotics and Automation
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

static const char *TAG = "STEPPER_DEBUG";

static const stepper_config_t stepper_1 = {
    .step_pin = STEP,
    .dir_pin = DIR,
    .accl_decel = 0.001,
    .min_step_interval_us = MIN_STEP_INTERVAL,
    .steps_per_revolution = 200,
    .current_step_count = 0,
};

void run_motor(void *arg)
{
    while (1)
    {
        ESP_LOGI(TAG, "Inside task");
        move_to_step(&stepper_1, 10000);
        ESP_LOGI(TAG, "Done 1");
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        move_to_step(&stepper_1, 0);
        ESP_LOGI(TAG, "Done 2");
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void app_main(void)
{
    init_stepper(&stepper_1);
    xTaskCreatePinnedToCore(&run_motor, "Homing Task", 4096, NULL, 1, NULL, 0);
}
