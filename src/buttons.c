#include "buttons.h"

static const char* TAG_BUTTONS = "buttons";
static int enabled_buttons_flag = 0;

esp_err_t enable_buttons()
{
    gpio_config_t io_conf;
    // bit mask for the pins, each bit maps to a GPIO 
    io_conf.pin_bit_mask = ((1ULL<<BUTTON_1) | (1ULL<<BUTTON_2) | (1ULL<<BUTTON_3) | (1ULL<<BUTTON_4));
    // set gpio mode to input
    io_conf.mode = GPIO_MODE_INPUT;
    // enable pull up resistors
    io_conf.pull_up_en = 1;
    // disable pull down resistors
    io_conf.pull_down_en = 0;
    // disable gpio interrupts
    io_conf.intr_type = GPIO_INTR_DISABLE;
    // detailed description can be found at https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/gpio.html#_CPPv413gpio_config_t

    esp_err_t err = gpio_config(&io_conf);
    if (err == ESP_OK)
    {
        ESP_LOGI(TAG_BUTTONS, "enabled buttons");
        enabled_buttons_flag = 1;
    }
    else
    {
        ESP_LOGE(TAG_BUTTONS, "error: %s", esp_err_to_name(err));
        enabled_buttons_flag = 0;
    }

    return err;
}

int read_button(int buttons_id)
{
    if (enabled_buttons_flag)
    {
        return !gpio_get_level((gpio_num_t)buttons_id);
    }
    else
    {
        ESP_LOGE(TAG_BUTTONS, "error: buttons not enabled, call enable_buttons() before reading buttons");
        return 0;
    }
}
