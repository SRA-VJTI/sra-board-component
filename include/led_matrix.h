#ifndef LED_MATRIX_H
#define LED_MATRIX_H

#include <stdint.h>

#include "shift_register.h"

#include "esp_log.h"

typedef uint32_t led_matrix_data_t;

typedef struct {
    led_matrix_data_t data;
    shift_register_t *config;
} led_matrix;

typedef enum {
    LED_MATRIX_OUTPUT_SEQ,
    LED_MATRIX_OUTPUT_PAR
} led_matrix_output_mode_t;

// Currently unused but good for future reference
static const uint8_t led_matrix_rows = CONFIG_LED_MATRIX_ROWS;
static const uint8_t led_matrix_columns = CONFIG_LED_MATRIX_COLUMNS;

esp_err_t led_matrix_init(led_matrix **matrix);

esp_err_t led_matrix_set_data(led_matrix *matrix, const led_matrix_data_t data);
esp_err_t led_matrix_write(const led_matrix *matrix, const led_matrix_output_mode_t mode);

#endif