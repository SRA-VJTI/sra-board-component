#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/portable.h>
#include <freertos/projdefs.h>

#include <limits.h>
#include "led_matrix.h"
#include "driver/gpio.h"
#include <esp_err.h>
#include <esp_log.h>
#include <esp_heap_caps.h>

#define SPECIAL_PATTERN {};

static const char *TAG = "LED Matrix demo";

// 24 25 26 27 28 29
// 30 31 16 17 18 19
// 20 21 22 23 08 09
// 10 11 12 13 14 15
// 00 01 02 03 04 05

void app_main(void)
{
    led_matrix *xMyLEDMatrix = NULL;

    // Initialise the LED Matrix and Shift Register Pins
    ESP_ERROR_CHECK(led_matrix_init(&xMyLEDMatrix));

    ESP_LOGI(TAG, "Shift register pins -> SDATA: %d, SRCLK: %d, RCLK: %d",
             xMyLEDMatrix->config->sdata,
             xMyLEDMatrix->config->srclk,
             xMyLEDMatrix->config->rclk);

    for (int i = 0; i < 3; i++) {
        // Set the initial pattern and send the data
        led_matrix_set_data_raw(xMyLEDMatrix, UINT32_MAX);
        led_matrix_write(xMyLEDMatrix, LED_MATRIX_OUTPUT_PAR);
        ESP_LOGI(TAG, "All LEDs turned ON");

        // Wait for 1000 ms
        vTaskDelay(pdMS_TO_TICKS(1000));

        // Clear the LED Matrix
        led_matrix_set_data_raw(xMyLEDMatrix, 0);
        led_matrix_write(xMyLEDMatrix, LED_MATRIX_OUTPUT_PAR);
        ESP_LOGI(TAG, "All LEDs turned OFF");
        
        // Wait for 1000 ms
        vTaskDelay(pdMS_TO_TICKS(1000));
    }

    int i = 0;
    while (1) {
        led_matrix_set_data(xMyLEDMatrix, i);
        led_matrix_write(xMyLEDMatrix, LED_MATRIX_OUTPUT_PAR);
        if (++i == UINT32_MAX)
            i = 0;
        vTaskDelay(pdMS_TO_TICKS(1500));
    }
}
