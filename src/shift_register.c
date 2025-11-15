/*
 * MIT License
 *
 * Copyright (c)  2025 Society of Robotics and Automation
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

#include "shift_register.h"

static const char *TAG_SHIFT_REGISTER = "shift_register";

static bool shift_register_timer_on_alarm(gptimer_handle_t timer, const gptimer_alarm_event_data_t *edata, void *user_data)
{
    // Don't do anything just stop the timer
    gptimer_stop(timer);
    return false;
}

esp_err_t shift_register_gpio_init(shift_register_t *conf)
{
    // if (!conf) ...
    ESP_RETURN_ON_FALSE(
        (conf != NULL),
        ESP_ERR_INVALID_ARG,
        TAG_SHIFT_REGISTER,
        "Handle is NULL!"
    );

    // Configure GPIOs
    conf->sdata = SHIFT_REGISTER_SDATA;
    conf->srclk = SHIFT_REGISTER_SRCLK;
    conf->rclk  = SHIFT_REGISTER_RCLK;

    gpio_config_t io_conf = {0};
    uint64_t pin_mask = (1ULL << conf->sdata) |
                        (1ULL << conf->srclk) |
                        (1ULL << conf->rclk);

    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pin_bit_mask = pin_mask;
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
    ESP_RETURN_ON_ERROR(
        gpio_config(&io_conf),
        TAG_SHIFT_REGISTER,
        "Failed to configure shift register GPIOs"
    );

    // Configure timer
    conf->timer = NULL;
    gptimer_config_t timer_config = {
        .clk_src = GPTIMER_CLK_SRC_DEFAULT,
        .direction = GPTIMER_COUNT_UP,
        .resolution_hz = 1000000 // 1 MHz, 1 tick = 1 microsecond
    };

    ESP_RETURN_ON_ERROR(
        gptimer_new_timer(&timer_config, &(conf->timer)),
        TAG_SHIFT_REGISTER,
        "Failed to configure shift register timer"
    );

    // Register timer callback
    gptimer_event_callbacks_t cbs = {
        .on_alarm = shift_register_timer_on_alarm
    };

    ESP_RETURN_ON_ERROR(
        gptimer_register_event_callbacks(conf->timer, &cbs, NULL),
        TAG_SHIFT_REGISTER,
        "Failed to register timer callback"
    );

    // Enable the timer (will disable when we are done using it)
    ESP_RETURN_ON_ERROR(
        gptimer_enable(conf->timer),
        TAG_SHIFT_REGISTER,
        "Failed to enable timer"
    );

    // Configure the alarm (CONFIG_SR_CLOCK_HIGH_TIME us)
    gptimer_alarm_config_t alarm_config = {
        // number of ticks (microseconds here) to wait for alarm
        .alarm_count = CONFIG_SR_CLOCK_HIGH_TIME
    };
    
    ESP_RETURN_ON_ERROR(
        gptimer_set_alarm_action(conf->timer, &alarm_config),
        TAG_SHIFT_REGISTER,
        "Failed to set configure alarm"
    );

    return ESP_OK;
}

static inline void clock_toggle_rising(gptimer_handle_t timer, gpio_num_t clk, int32_t delay_us)
{
    gpio_set_level(clk, 0);     // ensure starts from low level
    gpio_set_level(clk, 1);     // initial edge (rising)
    gptimer_start(timer);       // start the timer (configured for CONFIG_SR_CLOCK_HIGH_TIME)
    gpio_set_level(clk, 0);     // final edge (falling)
}

esp_err_t shift_register_write_uint8(const shift_register_t *sreg, const uint8_t data, const bool instant)
{
    int i;

    ESP_RETURN_ON_FALSE(
        (sreg != NULL),
        ESP_ERR_INVALID_ARG,
        TAG_SHIFT_REGISTER,
        "Handle is NULL"
    );

    if (instant) {
        for (i = (BITS_IN_A_BYTE - 1); i >= 0; i--) {
            gpio_set_level(sreg->sdata, data & (1 << i));
            clock_toggle_rising(sreg->timer, sreg->srclk, CONFIG_SR_CLOCK_HIGH_TIME);
        }
        clock_toggle_rising(sreg->timer, sreg->rclk, CONFIG_SR_CLOCK_HIGH_TIME);
    } else {
        for (i = (BITS_IN_A_BYTE - 1); i >= 0; i--) {
            gpio_set_level(sreg->sdata, data & (1 << i));
            clock_toggle_rising(sreg->timer, sreg->srclk, CONFIG_SR_CLOCK_HIGH_TIME);
            clock_toggle_rising(sreg->timer, sreg->rclk, CONFIG_SR_CLOCK_HIGH_TIME);
        } 
    }

    return ESP_OK;
}

esp_err_t shift_register_write_uint32(const shift_register_t *sreg, const uint32_t data, const bool instant)
{
    int i;

    ESP_RETURN_ON_FALSE(
        (sreg != NULL),
        ESP_ERR_INVALID_ARG,
        TAG_SHIFT_REGISTER,
        "Handle is NULL"
    );

    if (instant) {
        for (i = (BITS_IN_4_BYTES - 1); i >= 0; i--) {
            gpio_set_level(sreg->sdata, data & (1 << i));
            clock_toggle_rising(sreg->timer, sreg->srclk, CONFIG_SR_CLOCK_HIGH_TIME);
        }
        clock_toggle_rising(sreg->timer, sreg->rclk, CONFIG_SR_CLOCK_HIGH_TIME);
    } else {
        for (i = (BITS_IN_4_BYTES - 1); i >= 0; i--) {
            gpio_set_level(sreg->sdata, data & (1 << i));
            clock_toggle_rising(sreg->timer, sreg->srclk, CONFIG_SR_CLOCK_HIGH_TIME);
            clock_toggle_rising(sreg->timer, sreg->rclk, CONFIG_SR_CLOCK_HIGH_TIME);
        } 
    }

    return ESP_OK;
}

esp_err_t shift_register_cleanup(shift_register_t sreg)
{
    ESP_RETURN_ON_ERROR(gptimer_disable(sreg.timer), TAG_SHIFT_REGISTER, "Failed to disable timer"); 
    ESP_RETURN_ON_ERROR(gptimer_del_timer(sreg.timer), TAG_SHIFT_REGISTER, "Failed to delete timer");
    return ESP_OK;
}
