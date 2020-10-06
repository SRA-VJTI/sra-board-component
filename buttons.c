#include "buttons.h"

static const char* TAG_BUTTONS = "buttons";
static int enabled_buttons_flag = 0;

esp_err_t enable_buttons()
{
    gpio_config_t io_conf;
    io_conf.pin_bit_mask = ((1ULL<<BUTTON_1) | (1ULL<<BUTTON_2) | (1ULL<<BUTTON_3) | (1ULL<<BUTTON_4));
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pull_up_en = 1;
    io_conf.pull_down_en = 0;
    io_conf.intr_type = GPIO_INTR_DISABLE;

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

int read_button(int pin_number)
{
    if (enabled_buttons_flag)
    {
        return !gpio_get_level((gpio_num_t)pin_number);
    }
    else
    {
        ESP_LOGE(TAG_BUTTONS, "error: buttons not enabled, call enable_buttons() before reading buttons");
        return 0;
    }
}
