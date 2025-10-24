#include "shift_register.h"

static const char *TAG = "Shift Register";

esp_err_t shift_register_gpio_init(shift_register_t **conf)
{
    // Allocate memory
    *conf = (shift_register_t *) heap_caps_malloc(sizeof(shift_register_t), MALLOC_CAP_DEFAULT | MALLOC_CAP_8BIT);

    // if (!conf) ...
    ESP_RETURN_ON_FALSE(
        (*conf != NULL),
        ESP_ERR_NO_MEM,
        TAG,
        "Could not allocate memory!"
    );

    (*conf)->sdata = SHIFT_REGISTER_SDATA;
    (*conf)->srclk = SHIFT_REGISTER_SRCLK;
    (*conf)->rclk  = SHIFT_REGISTER_RCLK;

    gpio_set_direction((*conf)->sdata, GPIO_MODE_OUTPUT);
    gpio_set_direction((*conf)->srclk, GPIO_MODE_OUTPUT);
    gpio_set_direction((*conf)->rclk,  GPIO_MODE_OUTPUT);
    gpio_set_level((*conf)->sdata, 0);
    gpio_set_level((*conf)->srclk, 0);
    gpio_set_level((*conf)->rclk,  0);

    return ESP_OK;
}

static inline void clock_toggle_rising(gpio_num_t clk, int32_t delay_us)
{
    gpio_set_level(clk, 0);     // ensure starts from low level
    gpio_set_level(clk, 1);     // initial edge (rising)
    esp_rom_delay_us(delay_us); // clock delay (busy wait)
    gpio_set_level(clk, 0);     // final edge (falling)
}

esp_err_t shift_register_write_byte(const shift_register_t *sreg, const uint8_t data, const bool instant)
{
    int i;

    ESP_RETURN_ON_FALSE(
        (sreg != NULL),
        ESP_ERR_INVALID_ARG,
        TAG,
        "Handle is NULL"
    );

    if (instant) {
        for (i = (BITS_IN_A_BYTE - 1); i >= 0; i--) {
            gpio_set_level(sreg->sdata, data & (1 << i));
            clock_toggle_rising(sreg->srclk, CONFIG_SR_CLOCK_HIGH_TIME);
            gpio_set_level(sreg->sdata, 0);
        } 
        clock_toggle_rising(sreg->rclk, CONFIG_SR_CLOCK_HIGH_TIME);
    } else {
        for (i = (BITS_IN_A_BYTE - 1); i >= 0; i--) {
            gpio_set_level(sreg->sdata, data & (1 << i));
            clock_toggle_rising(sreg->srclk, CONFIG_SR_CLOCK_HIGH_TIME);
            gpio_set_level(sreg->sdata, 0);
            clock_toggle_rising(sreg->rclk, CONFIG_SR_CLOCK_HIGH_TIME);
        } 
    }

    return ESP_OK;
}

esp_err_t shift_register_write_int(const shift_register_t *sreg, const uint32_t data, const bool instant)
{
    int i;

    ESP_RETURN_ON_FALSE(
        (sreg != NULL),
        ESP_ERR_INVALID_ARG,
        TAG,
        "Handle is NULL"
    );

    if (instant) {
        for (i = (BITS_IN_4_BYTES - 1); i >= 0; i--) {
            gpio_set_level(sreg->sdata, data & (1 << i));
            clock_toggle_rising(sreg->srclk, CONFIG_SR_CLOCK_HIGH_TIME);
            gpio_set_level(sreg->sdata, 0);
        } 
        clock_toggle_rising(sreg->rclk, CONFIG_SR_CLOCK_HIGH_TIME);
    } else {
        for (i = (BITS_IN_4_BYTES - 1); i >= 0; i--) {
            gpio_set_level(sreg->sdata, data & (1 << i));
            clock_toggle_rising(sreg->srclk, CONFIG_SR_CLOCK_HIGH_TIME);
            gpio_set_level(sreg->sdata, 0);
            clock_toggle_rising(sreg->rclk, CONFIG_SR_CLOCK_HIGH_TIME);
        } 
    }

    return ESP_OK;
}
