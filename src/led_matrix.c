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

esp_err_t led_matrix_set_bit(led_matrix *matrix, const uint8_t pin, const uint8_t led_value)
{
    ESP_RETURN_ON_FALSE(
        (matrix != NULL),
        ESP_ERR_INVALID_ARG,
        TAG,
        "Handle is NULL"
    );

    ESP_RETURN_ON_FALSE(
        (pin < 30),
        ESP_ERR_INVALID_ARG,
        TAG,
        "LED index greater than %d!",
        led_matrix_rows * led_matrix_columns - 1
    );

    // Find the physical pin from the logical pin
    const uint8_t physical_pin = led_matrix_map[pin];
    // Find the corresponding pin
    const uint8_t current_bit = (matrix->data & (1 << physical_pin));

    // If the bits don't match then flip the bit
    if ((current_bit && !led_value) || (!current_bit && led_value))
        matrix->data ^= (1 << physical_pin);

    return ESP_OK;
}

esp_err_t led_matrix_set_data(led_matrix *matrix, const led_matrix_data_t data)
{
    ESP_RETURN_ON_FALSE(
        (matrix != NULL),
        ESP_ERR_INVALID_ARG,
        TAG,
        "Handle is NULL"
    );
    
    esp_err_t ret = ESP_OK;
    for (int i = 0; i < led_matrix_rows * led_matrix_columns; i++)
        if ((ret = led_matrix_set_bit(matrix, i, data & (0x1 << i))) != ESP_OK)
            return ret;

    return ESP_OK;
}

esp_err_t led_matrix_set_data_raw(led_matrix *matrix, const led_matrix_data_t data)
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