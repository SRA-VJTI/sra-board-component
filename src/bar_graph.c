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

#include "bar_graph.h"

static const char *TAG_BAR_GRAPH = "bar_graph";
static int enabled_bar_graph_flag = 0;

// bitmask is a safety mechanism, say if motor driver A is off and motor driver B is in Normal mode, then only
// pins IN1 - IN4 are usable in the bar graph, as pins IN5 - IN8 are connected to pins of motor driver B and these cannot be used.
// So, bit mask sets bits of IN5 - IN8 as 0 and that of IN1 - IN4 as 1, because if we set pins used by mcpwm, esp32 will crash or behave oddly.
// IN1 is the leftmost bit and IN8 is the rightmost bit
static const uint8_t bitmask[3] = {0xFF, 0x0F};
//                          state    0     1
// Just an array of pins used by bar graph led
static const int pin_out[8] = {BG_LED_1, BG_LED_2, BG_LED_3, BG_LED_4, BG_LED_5, BG_LED_6, BG_LED_7, BG_LED_8};

esp_err_t enable_bar_graph()
{
    uint64_t bit_mask = 0;
    // motor driver a is off so we can use IN1 - IN8 pins
    if (get_motor_driver_status() == 0)
    {
        bit_mask = (1ULL << BG_LED_1) | (1ULL << BG_LED_2) | (1ULL << BG_LED_3) | (1ULL << BG_LED_4) | (1ULL << BG_LED_5) | (1ULL << BG_LED_6) | (1ULL << BG_LED_7) | (1ULL << BG_LED_8);
        enabled_bar_graph_flag = 0;
    }
    // motor driver a is in normal mode, so we can use IN5, IN6, IN7, IN8 pins
    else if (get_motor_driver_status() == 1)
    {
        bit_mask = (1ULL << BG_LED_5) | (1ULL << BG_LED_6) | (1ULL << BG_LED_7) | (1ULL << BG_LED_8);
        enabled_bar_graph_flag = 1;
    }

    gpio_config_t io_conf;
    // bit mask for the pins, each bit maps to a GPIO
    io_conf.pin_bit_mask = bit_mask;
    // set gpio mode to input
    io_conf.mode = GPIO_MODE_OUTPUT;
    // enable pull up resistors
    io_conf.pull_up_en = 0;
    // disable pull down resistors
    io_conf.pull_down_en = 1;
    // disable gpio interrupts
    io_conf.intr_type = GPIO_INTR_DISABLE;
    // detailed description can be found at https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/gpio.html#_CPPv413gpio_config_t

    esp_err_t err = gpio_config(&io_conf);
    if (err == ESP_OK)
    {
        ESP_LOGI(TAG_BAR_GRAPH, "enabled bar graph leds in mode: %d", enabled_bar_graph_flag);
    }
    else
    {
        ESP_LOGE(TAG_BAR_GRAPH, "error: %s", esp_err_to_name(err));
        enabled_bar_graph_flag = 0;
    }

    return err;
}

esp_err_t set_bar_graph(uint8_t data)
{
    // store the bitmask according to the mode of bar graph led's
    uint8_t mode_bitmask = bitmask[enabled_bar_graph_flag];

    // since we want to read the 8 bits, we can use a loop 8 times, and left shift 8 times to extract the 8 bits
    for (int i = 0; i < 8; i++)
    {
        // bitwise-and bitmask with 0x80, so with this we can extract the value of leftmost bit.
        // Yxxxxxxx & 10000000 == 10000000, if Y = 1 and Yxxxxxxx & 10000000 == 00000000, if Y = 0
        // if it is 1, means the pin is usable, so we get into the if loop.
        if ((mode_bitmask & 0x80) == 0x80)
        {
            // We do the same process to extract the leftmost bit of data
            // If it is 1, then set the pin to 1, else set it to 0
            if ((data & 0x80) == 0x80)
            {
                // set gpio value to 1
                gpio_set_level((gpio_num_t)pin_out[i], 1);
            }
            else
            {
                // set gpio value to 0
                gpio_set_level((gpio_num_t)pin_out[i], 0);
            }
        }
        // left-shift bitmask and data once, we do this 8 times to read all the bits
        // 00110011 << 1 = 01100110
        mode_bitmask = mode_bitmask << 1;
        data = data << 1;
    }

    return ESP_OK;
}

uint8_t bool_to_uint8(bool *input_array)
{
    return ((input_array[0] << 7) | (input_array[1] << 6) | (input_array[2] << 5) | (input_array[3] << 4) | (input_array[4] << 3) | (input_array[5] << 2) | (input_array[6] << 1) | input_array[7]);
}