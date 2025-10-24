#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/portable.h>

#include "led_matrix.h"

#include <esp_err.h>
#include <esp_log.h>
#include <esp_heap_caps.h>

#define SPECIAL_PATTERN ((led_matrix_data_t) 555543840)

void app_main(void)
{
    led_matrix *xMyLEDMatrix = NULL;

    // Initialise the LED Matrix and Shift Register Pins
    ESP_ERROR_CHECK(led_matrix_init(&xMyLEDMatrix));

    for (int i = 0; i < 3; i++) {
        // Set the initial pattern and send the data
        led_matrix_set_data(xMyLEDMatrix, BITS_IN_4_BYTES - 1);
        led_matrix_write(xMyLEDMatrix, LED_MATRIX_OUTPUT_PAR);

        // Wait for 3000ms
        vTaskDelay(pdMS_TO_TICKS(3000));

        // Clear the LED Matrix
        led_matrix_set_data(xMyLEDMatrix, 0);
        led_matrix_write(xMyLEDMatrix, LED_MATRIX_OUTPUT_PAR);

        // Wait for 3000ms
        vTaskDelay(pdMS_TO_TICKS(3000));
    }

    led_matrix_data_t d = 1;
    while (1) {
        // Write data
        led_matrix_set_data(xMyLEDMatrix, d);
        led_matrix_write(xMyLEDMatrix, LED_MATRIX_OUTPUT_PAR);

        // Wait for 3000ms
        vTaskDelay(pdMS_TO_TICKS(3000));

        // Update
        if (d)
            d = d << 1;
        else
            d = 1;
    }
}
