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


#include "led_matrix.h"
#include "led_matrix_font.h"

static const char *TAG_LED_MATRIX = "led_matrix";

led_matrix led_matrix_init(void)
{
    shift_register_t conf;
    ESP_ERROR_CHECK(shift_register_gpio_init(&conf));
    return (led_matrix) {
        .data = 0,
        .config = conf
    };
}

led_matrix_data_t bool_to_uint32(const led_matrix_data_arr_t input_arr)
{
    uint8_t logical_pin;
    led_matrix_data_t ret = 0;

    for (int r = 0; r < led_matrix_rows; r++)
        for (int c = 0; c < led_matrix_columns; c++)
            if (input_arr[r][c]) {
                logical_pin = (led_matrix_rows - (r + 1)) * led_matrix_columns + c;
                ret = ret | (0x1 << led_matrix_map[logical_pin]);
            }

    return ret;
}

esp_err_t led_matrix_set_bit(led_matrix *matrix, const uint8_t logical_pin, const uint8_t led_value)
{
    ESP_RETURN_ON_FALSE(
        (matrix != NULL),
        ESP_ERR_INVALID_ARG,
        TAG_LED_MATRIX,
        "Handle is NULL"
    );

    ESP_RETURN_ON_FALSE(
        (logical_pin < (led_matrix_rows * led_matrix_columns)),
        ESP_ERR_INVALID_ARG,
        TAG_LED_MATRIX,
        "LED index greater than %d!",
        led_matrix_rows * led_matrix_columns - 1
    );

    // Find the physical pin from the logical pin
    const uint8_t physical_pin = led_matrix_map[logical_pin];
    // Find the corresponding bit
    const uint8_t current_bit = (matrix->data >> physical_pin) & 0x1;

    // If the bits don't match then flip the bit
    if ((!current_bit && led_value) || (current_bit && !led_value))
        matrix->data ^= (0x1 << physical_pin);

    return ESP_OK;
}

esp_err_t led_matrix_set_data(led_matrix *matrix, const led_matrix_data_t data)
{
    ESP_RETURN_ON_FALSE(
        (matrix != NULL),
        ESP_ERR_INVALID_ARG,
        TAG_LED_MATRIX,
        "Handle is NULL"
    );
    
    esp_err_t ret = ESP_OK;
    for (int i = 0; i < led_matrix_rows * led_matrix_columns; i++)
        if ((ret = led_matrix_set_bit(matrix, i, (data >> i) & 0x1)) != ESP_OK)
            return ret;

    return ESP_OK;
}

esp_err_t led_matrix_set_data_raw(led_matrix *matrix, const led_matrix_data_t data)
{
    ESP_RETURN_ON_FALSE(
        (matrix != NULL),
        ESP_ERR_INVALID_ARG,
        TAG_LED_MATRIX,
        "Handle is NULL"
    );
    
    matrix->data = data;
    return ESP_OK;
}

esp_err_t led_matrix_write(const led_matrix *matrix, const led_matrix_output_mode_t mode)
{
    ESP_RETURN_ON_FALSE(
        (matrix != NULL),
        ESP_ERR_INVALID_ARG,
        TAG_LED_MATRIX,
        "Handle is NULL"
    );

    if (mode == LED_MATRIX_OUTPUT_SEQ)
        shift_register_write_uint32(&(matrix->config), matrix->data, 0);
    else if (mode == LED_MATRIX_OUTPUT_PAR)
        shift_register_write_uint32(&(matrix->config), matrix->data, 1);
    else
        return ESP_ERR_INVALID_ARG;

    return ESP_OK;
}

esp_err_t led_matrix_display_string(led_matrix *matrix, const char *message, double wait_ms)
{
    ESP_RETURN_ON_FALSE(
        (matrix != NULL),
        ESP_ERR_INVALID_ARG,
        TAG_LED_MATRIX,
        "Handle is NULL"
    );

    ESP_RETURN_ON_FALSE(
        (message != NULL),
        ESP_ERR_INVALID_ARG,
        TAG_LED_MATRIX,
        "Handle is NULL"
    );

    size_t i;
    size_t mlen = strlen(message);
    for (i = 0; i < mlen; i++)
    {
        // If character is less than 0x20 (" ") or greater than 0x7F (DEL) in ASCII
        if (iscntrl((uint8_t) message[i]) || message[i] == 127) {
            matrix->data = bool_to_uint32(led_matrix_chars[0]); // Set it to a space
        } else {
            matrix->data = bool_to_uint32(led_matrix_chars[message[i] - 0x20]);
        }
        led_matrix_write(matrix, LED_MATRIX_OUTPUT_PAR);
        vTaskDelay(pdMS_TO_TICKS(wait_ms));
    }

    return ESP_OK;
}