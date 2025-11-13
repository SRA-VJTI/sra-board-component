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

#include <string.h>
#include <stdint.h>
#include <ctype.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "esp_log.h"

#include "shift_register.h"

/**
 * @brief An integer used as a bitarray to represent the LED matrix data type.
 */
typedef uint32_t led_matrix_data_t;

/**
 * @brief An array of booleans used to represent the LED matrix data instead of the uint32_t.

 * @note Valid when the SRA Board is in its upright orientation (text and logo upright).
 */
typedef bool led_matrix_data_arr_t[CONFIG_LED_MATRIX_ROWS][CONFIG_LED_MATRIX_COLUMNS];

/**
 * @brief Struct used as a handle for LED matrix
 **/
typedef struct {
    led_matrix_data_t data;
    shift_register_t config;
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

/**
 * @brief A mapping of the logical indexes of the LED matrix to the actual physical configuration of the LEDs, start from the bottom left

 * @details This mapping starts from the bottom left of the matrix and defines how each logical LED index corresponds to the physical wiring of the LED matrix
 * @details Default SRA Board LED matrix mappings (change for future versions, current version : v2.7)

 * @note This mapping starts from the bottom left of the matrix and the next logical index corresponds to the LED to the right in the same row. If the LED happens to be the last one in the row then the index corresponds to the first LED in the next (upper) row
 * @note For the v2.7 mapping pins outputs 6 and 7 are not connected to any LEDs and do not have any effect on the final pattern of the LEDs. (Remove or update this note in later versions)
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
led_matrix led_matrix_init(void);

/**
 * @brief Converts a boolean array of size CONFIG_LED_MATRIX_ROWSxCONFIG_LED_MATRIX_COLUMNS to a 32-bit unsigned integer

 * @param input_arr: Boolean input array of dimensions (CONFIG_LED_MATRIX_ROWS, CONFIG_LED_MATRIX_COLUMNS)

 * @return A 32-bit value representing the LED states, where each bit corresponds to one physical LED as defined by @ref led_matrix_map.
 */
led_matrix_data_t bool_to_uint32(const led_matrix_data_arr_t input_arr);

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

/** 
 * @brief Displays the entered string character by character on the LED Matrix with a delay of "wait_ms" ms between characters

 * @param matrix: The handle to represent and use the matrix of LEDs.
 * @param message: The message to be displayed on the screen.
 * @param wait_ms: The time to wait between two characters (in ms)

 * @return Returns an error if a NULL matrix or message is passed, else returns ESP_OK
 **/
esp_err_t led_matrix_display_string(led_matrix *matrix, const char *message, double wait_ms);

/**
 * @brief Performs cleanup on the LED Matrix contents

 * @param

 * @return Returns an error if there is an error while cleaning up the shift register, else returns ESP_OK
 **/
esp_err_t led_matrix_cleanup(led_matrix matrix);

#endif