/*
 * MIT License
 *
 * Copyright (c)  2021 Society of Robotics and Automation
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

#include "switches.h"

static const char *TAG_SWITCHES = "switches";
static int enabled_switches_flag = 0;

esp_err_t enable_switches()
{
    gpio_config_t io_conf;
    // bit mask for the pins, each bit maps to a GPIO
    io_conf.pin_bit_mask = ((1ULL << SWITCH_1) | (1ULL << SWITCH_2) | (1ULL << SWITCH_3) | (1ULL << SWITCH_4));
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
        ESP_LOGI(TAG_SWITCHES, "enabled switches");
        enabled_switches_flag = 1;
    }
    else
    {
        ESP_LOGE(TAG_SWITCHES, "error: %s", esp_err_to_name(err));
        enabled_switches_flag = 0;
    }

    return err;
}

int read_switch(int switch_id)
{
    if (enabled_switches_flag)
    {
        return !gpio_get_level((gpio_num_t)switch_id);
    }
    else
    {
        ESP_LOGE(TAG_SWITCHES, "error: switches not enabled, call enable_switches() before reading switches");
        return 0;
    }
}
