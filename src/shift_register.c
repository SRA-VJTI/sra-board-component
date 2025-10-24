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

    gpio_config_t io_conf = {0};
    uint64_t pin_mask = (1ULL << (*conf)->sdata) |
                        (1ULL << (*conf)->srclk) |
                        (1ULL << (*conf)->rclk);

    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pin_bit_mask = pin_mask;
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
    ESP_RETURN_ON_ERROR(
        gpio_config(&io_conf),
        TAG,
        "Failed to configure shift register GPIOs"
    );

    gpio_set_level((*conf)->sdata, 1);
    gpio_set_level((*conf)->srclk, 1);
    gpio_set_level((*conf)->rclk,  1);
    ESP_LOGI(TAG, "Sanity high check -> SDATA:%d SRCLK:%d RCLK:%d",
             gpio_get_level((*conf)->sdata),
             gpio_get_level((*conf)->srclk),
             gpio_get_level((*conf)->rclk));
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
            gpio_set_level(sreg->sdata, (data >> i) & 0x1);
            clock_toggle_rising(sreg->srclk, CONFIG_SR_CLOCK_HIGH_TIME);
        }
        clock_toggle_rising(sreg->rclk, CONFIG_SR_CLOCK_HIGH_TIME);
    } else {
        for (i = (BITS_IN_A_BYTE - 1); i >= 0; i--) {
            gpio_set_level(sreg->sdata, (data >> i) & 0x1);
            clock_toggle_rising(sreg->srclk, CONFIG_SR_CLOCK_HIGH_TIME);
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
            gpio_set_level(sreg->sdata, (data >> i) & 0x1);
            clock_toggle_rising(sreg->srclk, CONFIG_SR_CLOCK_HIGH_TIME);
        }
        clock_toggle_rising(sreg->rclk, CONFIG_SR_CLOCK_HIGH_TIME);
    } else {
        for (i = (BITS_IN_4_BYTES - 1); i >= 0; i--) {
            gpio_set_level(sreg->sdata, (data >> i) & 0x1);
            clock_toggle_rising(sreg->srclk, CONFIG_SR_CLOCK_HIGH_TIME);
            clock_toggle_rising(sreg->rclk, CONFIG_SR_CLOCK_HIGH_TIME);
        } 
    }

    return ESP_OK;
}
