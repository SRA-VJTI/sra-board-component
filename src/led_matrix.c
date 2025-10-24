#include "led_matrix.h"

static const char *TAG = "LED Matrix";

esp_err_t led_matrix_init(led_matrix **matrix)
{
    esp_err_t ret = ESP_OK;
    
    *matrix = (led_matrix *) heap_caps_malloc(sizeof(led_matrix), MALLOC_CAP_DEFAULT | MALLOC_CAP_8BIT);
    ESP_RETURN_ON_FALSE(
        (*matrix != NULL),
        ESP_ERR_NO_MEM,
        TAG,
        "Could not allocate memory!"
    );

    ret = shift_register_gpio_init(&((*matrix)->config));
    if (ret != ESP_OK)
        return ret;

    (*matrix)->data = 0;
    return ret;
}

esp_err_t led_matrix_set_data(led_matrix *matrix, const led_matrix_data_t data)
{
    ESP_RETURN_ON_FALSE(
        (matrix != NULL),
        ESP_ERR_INVALID_ARG,
        TAG,
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
        TAG,
        "Handle is NULL"
    );

    if (mode == LED_MATRIX_OUTPUT_SEQ)
        shift_register_write_int(matrix->config, matrix->data, 0);
    else if (mode == LED_MATRIX_OUTPUT_PAR)
        shift_register_write_int(matrix->config, matrix->data, 1);
    else
        return ESP_ERR_INVALID_ARG;

    return ESP_OK;
}