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

#include "esp_random.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "sra_board.h"

void app_main(void)
{
    led_matrix_t matrix;
    ESP_ERROR_CHECK(led_matrix_init(&matrix));
    ESP_ERROR_CHECK(led_matrix_set_brightness(&matrix, 4));

    uint8_t rows[LED_MATRIX_HEIGHT];

    while (1) {
        // Blink all 64 LEDs to check that every pixel turns on and off.
        for (int blink = 0; blink < 3; blink++) {
            for (int row = 0; row < LED_MATRIX_HEIGHT; row++) {
                rows[row] = 0xFF;
            }
            ESP_ERROR_CHECK(led_matrix_draw_bitmap(&matrix, rows));
            ESP_ERROR_CHECK(led_matrix_show(&matrix));
            vTaskDelay(pdMS_TO_TICKS(1000));

            ESP_ERROR_CHECK(led_matrix_clear(&matrix));
            vTaskDelay(pdMS_TO_TICKS(500));
        }

        // Random bitmaps exercise different combinations of rows and columns.
        for (int frame = 0; frame < 24; frame++) {
            for (int row = 0; row < LED_MATRIX_HEIGHT; row++) {
                rows[row] = (uint8_t)esp_random();
            }
            ESP_ERROR_CHECK(led_matrix_draw_bitmap(&matrix, rows));
            ESP_ERROR_CHECK(led_matrix_show(&matrix));
            vTaskDelay(pdMS_TO_TICKS(150));
        }

        ESP_ERROR_CHECK(led_matrix_clear(&matrix));
        vTaskDelay(pdMS_TO_TICKS(800));
    }
}
