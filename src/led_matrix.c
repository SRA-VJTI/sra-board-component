/*
 * MIT License
 *
 * Copyright (c) 2026 Society of Robotics and Automation
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

#include "led_matrix.h"

#include <string.h>

#include "driver/gpio.h"
#include "driver/spi_master.h"
#include "esp_check.h"
#include "pin_defs.h"

static const char *TAG = "led_matrix";

enum {
    MAX7219_REG_DIGIT0 = 0x01,
    MAX7219_REG_DECODE_MODE = 0x09,
    MAX7219_REG_INTENSITY = 0x0A,
    MAX7219_REG_SCAN_LIMIT = 0x0B,
    MAX7219_REG_SHUTDOWN = 0x0C,
    MAX7219_REG_DISPLAY_TEST = 0x0F,
};

#define LED_MATRIX_SPI_HOST SPI2_HOST
#define LED_MATRIX_SPI_CLOCK_HZ (1 * 1000 * 1000)
#define LED_MATRIX_DEFAULT_BRIGHTNESS 4

/* On the SRA board, MAX7219 digit registers 0..7 address columns from left
 * to right; bit 7 is the top LED and bit 0 is the bottom LED in each column.
 * Transpose the framebuffer's top-to-bottom rows into these columns before
 * sending them to the display, so bitmaps and fonts use the same orientation. */
static void logical_to_board_rows(const uint8_t logical_rows[LED_MATRIX_HEIGHT],
                                  uint8_t board_rows[LED_MATRIX_HEIGHT])
{
    memset(board_rows, 0, LED_MATRIX_HEIGHT);
    for (uint8_t y = 0; y < LED_MATRIX_HEIGHT; ++y) {
        for (uint8_t x = 0; x < LED_MATRIX_WIDTH; ++x) {
            if (logical_rows[y] & (uint8_t)(1U << (LED_MATRIX_WIDTH - 1U - x))) {
                board_rows[x] |= (uint8_t)(1U << (LED_MATRIX_WIDTH - 1U - y));
            }
        }
    }
}

static esp_err_t check_initialized(const led_matrix_t *matrix)
{
    ESP_RETURN_ON_FALSE(matrix != NULL, ESP_ERR_INVALID_ARG, TAG, "Matrix handle is NULL");
    ESP_RETURN_ON_FALSE(matrix->initialized, ESP_ERR_INVALID_STATE, TAG, "Matrix is not initialized");
    ESP_RETURN_ON_FALSE(matrix->spi_device != NULL, ESP_ERR_INVALID_STATE, TAG, "SPI device is unavailable");
    return ESP_OK;
}

static esp_err_t write_register(const led_matrix_t *matrix, uint8_t reg, uint8_t value)
{
    const uint8_t tx_data[2] = { reg, value };
    spi_transaction_t transaction = {
        .length = 16,
        .tx_buffer = tx_data,
    };

    ESP_RETURN_ON_ERROR(spi_device_acquire_bus(matrix->spi_device, portMAX_DELAY),
                        TAG, "Failed to acquire SPI bus");

    esp_err_t result = gpio_set_level(LED_MATRIX_LOAD, 0);
    if (result == ESP_OK) {
        result = spi_device_polling_transmit(matrix->spi_device, &transaction);
    }
    esp_err_t load_result = gpio_set_level(LED_MATRIX_LOAD, 1);
    spi_device_release_bus(matrix->spi_device);

    if (result != ESP_OK) {
        return result;
    }
    return load_result;
}

static esp_err_t initialize_display(led_matrix_t *matrix)
{
    ESP_RETURN_ON_ERROR(write_register(matrix, MAX7219_REG_DISPLAY_TEST, 0x00),
                        TAG, "Failed to disable display test mode");
    ESP_RETURN_ON_ERROR(write_register(matrix, MAX7219_REG_SCAN_LIMIT, 0x07),
                        TAG, "Failed to configure scan limit");
    ESP_RETURN_ON_ERROR(write_register(matrix, MAX7219_REG_DECODE_MODE, 0x00),
                        TAG, "Failed to disable BCD decoding");
    ESP_RETURN_ON_ERROR(write_register(matrix, MAX7219_REG_INTENSITY,
                                       matrix->configured_brightness),
                        TAG, "Failed to configure brightness");
    ESP_RETURN_ON_ERROR(write_register(matrix, MAX7219_REG_SHUTDOWN, 0x01),
                        TAG, "Failed to enable display scanning");
    for (uint8_t row = 0; row < LED_MATRIX_HEIGHT; ++row) {
        ESP_RETURN_ON_ERROR(write_register(matrix, MAX7219_REG_DIGIT0 + row, 0x00),
                            TAG, "Failed to clear display row");
    }
    return ESP_OK;
}

static void release_spi_resources(led_matrix_t *matrix)
{
    if (matrix->spi_device != NULL) {
        spi_bus_remove_device(matrix->spi_device);
        matrix->spi_device = NULL;
        spi_bus_free(LED_MATRIX_SPI_HOST);
    }
    gpio_reset_pin(LED_MATRIX_LOAD);
}

esp_err_t led_matrix_init(led_matrix_t *matrix)
{
    ESP_RETURN_ON_FALSE(matrix != NULL, ESP_ERR_INVALID_ARG, TAG, "Matrix handle is NULL");
    ESP_RETURN_ON_FALSE(GPIO_IS_VALID_OUTPUT_GPIO(LED_MATRIX_CLK) &&
                        GPIO_IS_VALID_OUTPUT_GPIO(LED_MATRIX_DIN) &&
                        GPIO_IS_VALID_OUTPUT_GPIO(LED_MATRIX_LOAD),
                        ESP_ERR_INVALID_ARG, TAG, "CLK, DIN, and LOAD must be output-capable GPIOs");
    ESP_RETURN_ON_FALSE(LED_MATRIX_CLK != LED_MATRIX_DIN &&
                        LED_MATRIX_CLK != LED_MATRIX_LOAD &&
                        LED_MATRIX_DIN != LED_MATRIX_LOAD,
                        ESP_ERR_INVALID_ARG, TAG, "CLK, DIN, and LOAD pins must be distinct");

    matrix->initialized = false;
    matrix->spi_device = NULL;
    matrix->configured_brightness = LED_MATRIX_DEFAULT_BRIGHTNESS;
    memset(matrix->framebuffer, 0, sizeof(matrix->framebuffer));

    gpio_config_t config = {
        .pin_bit_mask = (1ULL << LED_MATRIX_LOAD),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    ESP_RETURN_ON_ERROR(gpio_config(&config), TAG, "Failed to configure MAX7219 LOAD GPIO");
    ESP_RETURN_ON_ERROR(gpio_set_level(LED_MATRIX_LOAD, 1), TAG, "Failed to set LOAD high");

    spi_bus_config_t bus_config = {
        .mosi_io_num = LED_MATRIX_DIN,
        .miso_io_num = -1,
        .sclk_io_num = LED_MATRIX_CLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = 2,
    };
    esp_err_t err = spi_bus_initialize(LED_MATRIX_SPI_HOST, &bus_config, SPI_DMA_DISABLED);
    if (err != ESP_OK) {
        gpio_reset_pin(LED_MATRIX_LOAD);
        return err;
    }

    spi_device_interface_config_t device_config = {
        .clock_speed_hz = LED_MATRIX_SPI_CLOCK_HZ,
        .mode = 0,
        .spics_io_num = -1,
        .queue_size = 1,
    };
    err = spi_bus_add_device(LED_MATRIX_SPI_HOST, &device_config, &matrix->spi_device);
    if (err != ESP_OK) {
        spi_bus_free(LED_MATRIX_SPI_HOST);
        gpio_reset_pin(LED_MATRIX_LOAD);
        return err;
    }

    err = initialize_display(matrix);
    if (err != ESP_OK) {
        release_spi_resources(matrix);
        return err;
    }

    matrix->initialized = true;
    return ESP_OK;
}

esp_err_t led_matrix_set_pixel(led_matrix_t *matrix,
                                  uint8_t x, uint8_t y, bool on)
{
    ESP_RETURN_ON_ERROR(check_initialized(matrix), TAG, "Matrix is unavailable");
    ESP_RETURN_ON_FALSE(x < LED_MATRIX_WIDTH && y < LED_MATRIX_HEIGHT,
                        ESP_ERR_INVALID_ARG, TAG, "Pixel coordinate is outside the 8x8 matrix");

    const uint8_t mask = (uint8_t)(1U << (LED_MATRIX_WIDTH - 1 - x));
    if (on) {
        matrix->framebuffer[y] |= mask;
    } else {
        matrix->framebuffer[y] &= (uint8_t)~mask;
    }
    return ESP_OK;
}

esp_err_t led_matrix_set_row(led_matrix_t *matrix,
                                 uint8_t row, uint8_t pixels)
{
    ESP_RETURN_ON_ERROR(check_initialized(matrix), TAG, "Matrix is unavailable");
    ESP_RETURN_ON_FALSE(row < LED_MATRIX_HEIGHT,
                        ESP_ERR_INVALID_ARG, TAG, "Row index is outside the 8x8 matrix");

    matrix->framebuffer[row] = pixels;
    return ESP_OK;
}

esp_err_t led_matrix_draw_bitmap(led_matrix_t *matrix,
                                     const uint8_t rows[LED_MATRIX_HEIGHT])
{
    ESP_RETURN_ON_ERROR(check_initialized(matrix), TAG, "Matrix is unavailable");
    ESP_RETURN_ON_FALSE(rows != NULL, ESP_ERR_INVALID_ARG, TAG, "Bitmap pointer is NULL");

    memcpy(matrix->framebuffer, rows, sizeof(matrix->framebuffer));
    return ESP_OK;
}

esp_err_t led_matrix_show(const led_matrix_t *matrix)
{
    ESP_RETURN_ON_ERROR(check_initialized(matrix), TAG, "Matrix is unavailable");

    uint8_t board_rows[LED_MATRIX_HEIGHT];
    logical_to_board_rows(matrix->framebuffer, board_rows);
    for (uint8_t row = 0; row < LED_MATRIX_HEIGHT; ++row) {
        ESP_RETURN_ON_ERROR(write_register(matrix, MAX7219_REG_DIGIT0 + row,
                                           board_rows[row]),
                            TAG, "Failed to update display row");
    }
    return ESP_OK;
}

esp_err_t led_matrix_clear(led_matrix_t *matrix)
{
    ESP_RETURN_ON_ERROR(check_initialized(matrix), TAG, "Matrix is unavailable");

    memset(matrix->framebuffer, 0, sizeof(matrix->framebuffer));
    return led_matrix_show(matrix);
}

esp_err_t led_matrix_set_brightness(led_matrix_t *matrix,
                                        uint8_t brightness)
{
    ESP_RETURN_ON_ERROR(check_initialized(matrix), TAG, "Matrix is unavailable");
    ESP_RETURN_ON_FALSE(brightness <= 0x0F, ESP_ERR_INVALID_ARG,
                        TAG, "Brightness must be between 0 and 15");

    esp_err_t result = write_register(matrix, MAX7219_REG_INTENSITY, brightness);
    if (result == ESP_OK) {
        matrix->configured_brightness = brightness;
    }
    return result;
}

esp_err_t led_matrix_set_enabled(const led_matrix_t *matrix,
                                     bool enabled)
{
    ESP_RETURN_ON_ERROR(check_initialized(matrix), TAG, "Matrix is unavailable");
    return write_register(matrix, MAX7219_REG_SHUTDOWN, enabled ? 0x01 : 0x00);
}

esp_err_t led_matrix_deinit(led_matrix_t *matrix)
{
    ESP_RETURN_ON_ERROR(check_initialized(matrix), TAG, "Matrix is unavailable");

    esp_err_t result = spi_bus_remove_device(matrix->spi_device);
    matrix->spi_device = NULL;
    esp_err_t err = spi_bus_free(LED_MATRIX_SPI_HOST);
    if (result == ESP_OK && err != ESP_OK) {
        result = err;
    }
    err = gpio_reset_pin(LED_MATRIX_LOAD);
    if (result == ESP_OK && err != ESP_OK) {
        result = err;
    }
    matrix->initialized = false;
    return result;
}
