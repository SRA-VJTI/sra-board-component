/*
 * Copyright (c) 2021 Society of Robotics and Automation.
 *
 * This file is part of SRA Board Component 
 * (see https://www.sravjti.in).
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
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
