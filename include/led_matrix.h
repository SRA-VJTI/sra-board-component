#ifndef LED_MATRIX_H
#define LED_MATRIX_H

#include <stdint.h>

#include "shift_register.h"

#include "esp_log.h"

typedef uint32_t led_matrix_data_t;
typedef uint8_t  led_matrix_data_arr_t[6];

typedef struct {
    led_matrix_data_t data;
    shift_register_t *config;
} led_matrix;

typedef enum {
    LED_MATRIX_OUTPUT_SEQ,
    LED_MATRIX_OUTPUT_PAR
} led_matrix_output_mode_t;

static const uint8_t led_matrix_rows = CONFIG_LED_MATRIX_ROWS;
static const uint8_t led_matrix_columns = CONFIG_LED_MATRIX_COLUMNS;

// Default SRA Board mapping (change for future versions)
static const uint8_t led_matrix_map[CONFIG_LED_MATRIX_ROWS * CONFIG_LED_MATRIX_COLUMNS] = {
     0,  1,  2,  3,  4,  5,
    10, 11, 12, 13, 14, 15,
    20, 21, 22, 23,  8,  9,
    30, 31, 16, 17, 18, 19,
    24, 25, 26, 27, 28, 29
};

esp_err_t led_matrix_init(led_matrix **matrix);

esp_err_t led_matrix_set_bit(led_matrix *matrix, const uint8_t led_number, const uint8_t led_value);
esp_err_t led_matrix_set_data(led_matrix *matrix, const led_matrix_data_t data);
esp_err_t led_matrix_set_data_raw(led_matrix *matrix, const led_matrix_data_t data);
esp_err_t led_matrix_write(const led_matrix *matrix, const led_matrix_output_mode_t mode);

#endif