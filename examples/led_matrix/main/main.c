/*
 * MIT License
 *
 * Copyright (c)  2025 Society of Robotics and Automation
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

#include <limits.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/portable.h>
#include <driver/gpio.h>
#include <esp_err.h>
#include <esp_log.h>

#include "led_matrix.h"

static const char *TAG = "LED Matrix demo";

void app_main(void)
{
    // Initialise the LED Matrix and Shift Register Pins
    led_matrix xMyLEDMatrix = led_matrix_init();

    ESP_LOGI(TAG, "Shift register pins -> SDATA: %d, SRCLK: %d, RCLK: %d",
             xMyLEDMatrix.config.sdata,
             xMyLEDMatrix.config.srclk,
             xMyLEDMatrix.config.rclk);

    for (int i = 0; i < 3; i++) {
        // Set the initial pattern and send the data
        ESP_ERROR_CHECK(led_matrix_set_data_raw(&xMyLEDMatrix, UINT32_MAX));
        ESP_ERROR_CHECK(led_matrix_write(&xMyLEDMatrix, LED_MATRIX_OUTPUT_PAR));
        ESP_LOGI(TAG, "All LEDs turned ON");

        // Wait for 1000 ms
        vTaskDelay(pdMS_TO_TICKS(1000));

        // Clear the LED Matrix
        ESP_ERROR_CHECK(led_matrix_set_data_raw(&xMyLEDMatrix, 0));
        ESP_ERROR_CHECK(led_matrix_write(&xMyLEDMatrix, LED_MATRIX_OUTPUT_PAR));
        ESP_LOGI(TAG, "All LEDs turned OFF");
        
        // Wait for 1000 ms
        vTaskDelay(pdMS_TO_TICKS(1000));
    }

    int i = 0;
    while (1) {
        // Turn on a single LED matrix corresponding to the integer (2^i)
        ESP_ERROR_CHECK(led_matrix_set_data(&xMyLEDMatrix, 0x1 << i));
        ESP_ERROR_CHECK(led_matrix_write(&xMyLEDMatrix, LED_MATRIX_OUTPUT_PAR));
        if (++i == CONFIG_LED_MATRIX_ROWS * CONFIG_LED_MATRIX_COLUMNS)
            i = 0;

        // Wait for 1000 ms
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
