#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/portable.h>

#include <limits.h>
#include "led_matrix.h"
#include "driver/gpio.h"
#include <esp_err.h>
#include <esp_log.h>
#include <esp_heap_caps.h>

#define SPECIAL_PATTERN ((led_matrix_data_t) 555543840)

static const char *TAG = "LED_MATRIX_MAIN";

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
        led_matrix_set_data(xMyLEDMatrix, UINT32_MAX);
        led_matrix_write(xMyLEDMatrix, LED_MATRIX_OUTPUT_PAR);
        ESP_LOGI(TAG, "All LEDs turned ON");

        // Wait for 3000 ms
        vTaskDelay(pdMS_TO_TICKS(3000));

        // Clear the LED Matrix
        led_matrix_set_data(xMyLEDMatrix, 0);
        led_matrix_write(xMyLEDMatrix, LED_MATRIX_OUTPUT_PAR);
        ESP_LOGI(TAG, "All LEDs turned OFF");
        
        // Wait for 3000 ms
        vTaskDelay(pdMS_TO_TICKS(3000));
    }

    led_matrix_data_t d = 1;
    while (1) {
        // Write data
        led_matrix_set_data(xMyLEDMatrix, d);
        led_matrix_write(xMyLEDMatrix, LED_MATRIX_OUTPUT_PAR);
        ESP_LOGI(
            TAG,
            "Streaming pattern: %lu, SDATA:%d SRCLK:%d RCLK:%d",
            xMyLEDMatrix->data,
            gpio_get_level(xMyLEDMatrix->config->sdata),
            gpio_get_level(xMyLEDMatrix->config->srclk),
            gpio_get_level(xMyLEDMatrix->config->rclk)
        );

        // Update
        if (d)
            d = d << 1;
        else
            d = 1;
    }
}
