#include "bar_graph.h"

static const char* TAG_BAR_GRAPH = "bar_graph";
static int enabled_bar_graph_flag = 0;

// IN1 is the leftmost bit and IN8 is the rightmost bit
static uint8_t bitmask[9] = {0x00, 0xFF, 0xFC, 0xF0, 0xCF, 0xCC, 0xC0, 0x0F, 0x0C};
//                   state    0     1     2     3     4     5     6     7     8

static int pin_out[8] = {BG_LED_1, BG_LED_2, BG_LED_3, BG_LED_4, BG_LED_5, BG_LED_6, BG_LED_7, BG_LED_8};

esp_err_t enable_bar_graph()
{
    uint64_t bit_mask = 0;
    if (read_motor_driver_mode(a) == 0 && read_motor_driver_mode(b) == 0)
    {
        bit_mask = (1ULL<<BG_LED_1) | (1ULL<<BG_LED_2) | (1ULL<<BG_LED_3) | (1ULL<<BG_LED_4) | (1ULL<<BG_LED_5) | (1ULL<<BG_LED_6) | (1ULL<<BG_LED_7) | (1ULL<<BG_LED_8); 
        enabled_bar_graph_flag = 1;
    }
    else if (read_motor_driver_mode(a) == 0 & read_motor_driver_mode(b) == 1)
    {
        bit_mask = (1ULL<<BG_LED_1) | (1ULL<<BG_LED_2) | (1ULL<<BG_LED_3) | (1ULL<<BG_LED_4) | (1ULL<<BG_LED_5) | (1ULL<<BG_LED_6); 
        enabled_bar_graph_flag = 2;
    }
    else if (read_motor_driver_mode(a) == 0 & read_motor_driver_mode(b) == 2)
    {
        bit_mask = (1ULL<<BG_LED_1) | (1ULL<<BG_LED_2) | (1ULL<<BG_LED_3) | (1ULL<<BG_LED_4); 
        enabled_bar_graph_flag = 3;
    }
    else if (read_motor_driver_mode(a) == 1 & read_motor_driver_mode(b) == 0)
    {
        bit_mask = (1ULL<<BG_LED_1) | (1ULL<<BG_LED_2) | (1ULL<<BG_LED_5) | (1ULL<<BG_LED_6) | (1ULL<<BG_LED_7) | (1ULL<<BG_LED_8); 
        enabled_bar_graph_flag = 4;
    }
    else if (read_motor_driver_mode(a) == 1 & read_motor_driver_mode(b) == 1)
    {
        bit_mask = (1ULL<<BG_LED_1) | (1ULL<<BG_LED_2) | (1ULL<<BG_LED_5) | (1ULL<<BG_LED_6); 
        enabled_bar_graph_flag = 5;
    }
    else if (read_motor_driver_mode(a) == 1 & read_motor_driver_mode(b) == 2)
    {
        bit_mask = (1ULL<<BG_LED_1) | (1ULL<<BG_LED_2); 
        enabled_bar_graph_flag = 6;
    }
    else if (read_motor_driver_mode(a) == 2 & read_motor_driver_mode(b) == 0)
    {
        bit_mask = (1ULL<<BG_LED_5) | (1ULL<<BG_LED_6) | (1ULL<<BG_LED_7) | (1ULL<<BG_LED_8); 
        enabled_bar_graph_flag = 7;
    }
    else if (read_motor_driver_mode(a) == 2 & read_motor_driver_mode(b) == 1)
    {
        bit_mask = (1ULL<<BG_LED_5) | (1ULL<<BG_LED_6); 
        enabled_bar_graph_flag = 8;
    }
    else
    {
        enabled_bar_graph_flag = 0;
        ESP_LOGE(TAG_BAR_GRAPH, "error: both motor drivers enabled, not able to use the bar graph in this mode");

        return ESP_FAIL;
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
    uint8_t mode_bitmask = bitmask[enabled_bar_graph_flag];

    for (int i = 0; i < 8; i++)
    {
        if ((mode_bitmask & 0x80) == 0x80)
        {
            if ((data & 0x80) == 0x80)
            {
                gpio_set_level((gpio_num_t)pin_out[i], 1);
            }
            else
            {
                gpio_set_level((gpio_num_t)pin_out[i], 0);
            }
        }
        mode_bitmask = mode_bitmask << 1;
        data = data << 1;
    }

    return ESP_OK;
}
