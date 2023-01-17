/*
 * MIT License
 *
 * Copyright (c)  2023 Society of Robotics and Automation
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

#include "stepper.h"
#include <math.h>
static const char *TAG_STEPPER = "stepper";

static esp_timer_handle_t oneshot_timer;
int ramp_up = 0;
int delay;
long steps_to_move = 0;
static unsigned long n = 0;

static void oneshot_timer_callback(void *arg);

esp_err_t init_stepper(stepper_config_t *stepper)
{
    gpio_config_t io_conf = {};
    // Configure Stepper motor control pins as output

    // disable interrupt
    io_conf.intr_type = GPIO_INTR_DISABLE;
    // set as output mode
    io_conf.mode = GPIO_MODE_OUTPUT;
    // bit mask of the pins that you want to set,e.g.GPIO18/19
    io_conf.pin_bit_mask = ((1ULL << stepper->dir_pin) | (1ULL << stepper->step_pin));
    // disable pull-down mode
    io_conf.pull_down_en = 0;
    // disable pull-up mode
    io_conf.pull_up_en = 0;
    // configure GPIO with the given settings
    esp_err_t err = gpio_config(&io_conf);

    gpio_set_level(stepper->dir_pin, 0);
    gpio_set_level(stepper->step_pin, 0);

    if (err == ESP_OK)
    {
        ESP_LOGI(TAG_STEPPER, "Stepper driver Configured");
        return ESP_OK;
    }
    else
    {
        return ESP_FAIL;
    }
}

static int calc_first_delay_in_millis(stepper_config_t *stepper)
{
    float delay_in_sec = 0.676 * sqrtf((4 * M_PI / stepper->steps_per_revolution) / stepper->accl_decel);
    int delay = (int)((2000 * delay_in_sec) + 0.5);
    return delay;
}

static int cal_delay_accl(stepper_config_t *stepper, int n)
{
    int cn = stepper->prev_delay - (2 * stepper->prev_delay) / (4 * n + 1);
    return abs(cn);
}

static int cal_delay_decel(stepper_config_t *stepper, int n)
{
    int cn = (stepper->prev_delay * (4 * n + 1)) / (4 * n - 1);
    return abs(cn);
}

esp_err_t move_to_step(stepper_config_t *stepper, float pos_step)
{

    const esp_timer_create_args_t oneshot_timer_args = {
        .callback = &oneshot_timer_callback,
        /* argument specified here will be passed to timer callback function */
        .arg = (void *)stepper,
        .name = "step",
    };

    ESP_ERROR_CHECK(esp_timer_create(&oneshot_timer_args, &oneshot_timer));
    delay = calc_first_delay_in_millis(stepper);
    n = 0;
    ramp_up = 0;
    steps_to_move = pos_step - stepper->current_step_count;
    stepper->prev_delay = delay;
    stepper->dir = (steps_to_move < 0) ? -1 : 1;
    stepper->movement_done = false;
    gpio_set_level(stepper->dir_pin, steps_to_move < 0 ? 1 : 0);
    while (1)
    {
        if (!stepper->movement_done && !esp_timer_is_active(oneshot_timer))
        {
            ESP_ERROR_CHECK(esp_timer_start_once(oneshot_timer, delay));
            stepper->prev_delay = delay;
        }
        if (stepper->movement_done && !esp_timer_is_active(oneshot_timer))
        {
            ESP_ERROR_CHECK(esp_timer_delete(oneshot_timer));
            break;
        }
        // Loop takes the minimum amout of delay possible
    }

    return ESP_OK;
}

static void oneshot_timer_callback(void *arg)
{
    stepper_config_t *stepper = (stepper_config_t *)arg;
    if (n < abs(steps_to_move))
    {
        gpio_set_level(stepper->step_pin, 1);
        ets_delay_us(5);
        gpio_set_level(stepper->step_pin, 0);
        stepper->current_step_count += stepper->dir;
        n++;
    }
    else
    {
        stepper->movement_done = true;
    }

    if (ramp_up == 0)
    {
        delay = cal_delay_accl(stepper, n);
        if (delay <= stepper->min_step_interval_us)
        {
            delay = stepper->min_step_interval_us;
            ramp_up = n;
        }
        if (n >= abs(steps_to_move) / 2)
        {
            ramp_up = n;
        }
    }
    else if (n >= abs(steps_to_move) - ramp_up)
    {
        delay = cal_delay_decel(stepper, abs(steps_to_move) - n);
    }
}