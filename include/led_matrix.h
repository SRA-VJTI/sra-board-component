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

#ifndef LED_MATRIX_H
#define LED_MATRIX_H

#include <stdint.h>
#include "shift_register.h"
#include "esp_log.h"

typedef uint32_t led_matrix_data_t;
typedef uint8_t  led_matrix_data_arr_t[6];

/**
 * @brief Struct used as a handle for LED matrix
 **/
typedef struct {
    led_matrix_data_t data;
    shift_register_t *config;
} led_matrix;

/**
 * @brief Enum used for the output mode of the LED matrix
 **/
typedef enum {
    LED_MATRIX_OUTPUT_SEQ,
    LED_MATRIX_OUTPUT_PAR
} led_matrix_output_mode_t;

/**
 * @brief Software constant for defining the number of rows in the LED matrix
 **/
static const uint8_t led_matrix_rows = CONFIG_LED_MATRIX_ROWS;

/**
 * @brief Software constant for defining the number of columns in the LED matrix
 **/
static const uint8_t led_matrix_columns = CONFIG_LED_MATRIX_COLUMNS;

// Default SRA Board mapping (change for future versions)
/**
 * @brief A mapping of the logical indexes of the LED matrix to the actual physical configuration of the LEDs, start from the bottom left
 **/
static const uint8_t led_matrix_map[CONFIG_LED_MATRIX_ROWS * CONFIG_LED_MATRIX_COLUMNS] = {
     0,  1,  2,  3,  4,  5,
    10, 11, 12, 13, 14, 15,
    20, 21, 22, 23,  8,  9,
    30, 31, 16, 17, 18, 19,
    24, 25, 26, 27, 28, 29
};

/**
 * @brief Enables and configures the LED Matrix and corresponding shift registers.

 * @param matrix: The handle to represent and use the matrix of LEDs.

 * @return Returns an error if the could not allocate enough memory or if shift register configuration failed, else returns ESP_OK
 **/
esp_err_t led_matrix_init(led_matrix **matrix);

/**
 * @brief Sets a single bit of the "data" field of the passed in matrix.

 * @param matrix: The handle to represent and use the matrix of LEDs.
 * @param led_number: The logical index of the pin (varies from [0, led_matrix_rows * led_matrix_columns - 1])
 * @param led_value: The value to set the LED (can be 0 - LOW or 1 - HIGH)

 * @return Returns an error if a NULL matrix is passed or if the led number exceeds (led_matrix_rows * led_matrix_columns - 1), else returns ESP_OK
 **/
esp_err_t led_matrix_set_bit(led_matrix *matrix, const uint8_t led_number, const uint8_t led_value);

/**
 * @brief Sets all the bits of the "data" field of the passed in matrix using led_matrix_set_bit().

 * @param matrix: The handle to represent and use the matrix of LEDs.
 * @param data: The data to write to the LED matrix.

 * @return Returns an error if a NULL matrix is passed, else returns ESP_OK
 **/
esp_err_t led_matrix_set_data(led_matrix *matrix, const led_matrix_data_t data);

/**
 * @brief Sets all the bits of the "data" field.

 * @param matrix: The handle to represent and use the matrix of LEDs.
 * @param data: The bitfield used to set the values of the LED matrix.

 * @return Returns an error if a NULL matrix is passed, else returns ESP_OK
 **/
esp_err_t led_matrix_set_data_raw(led_matrix *matrix, const led_matrix_data_t data);

/**
 * @brief Performs the write from the led_matrix structure to the actual LEDs on board

 * @param matrix: The handle to represent and use the matrix of LEDs.
 * @param mode: The way in which to display the values from the shift register.

 * @return Returns an error if a NULL matrix is passed, else returns ESP_OK
 **/
esp_err_t led_matrix_write(const led_matrix *matrix, const led_matrix_output_mode_t mode);

#endif