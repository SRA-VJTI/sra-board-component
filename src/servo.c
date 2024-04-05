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

#include "servo.h"

static const char *TAG_SERVO = "servo";
static int enabled_servo_flag = 0;

#define SERVO_TIMEBASE_RESOLUTION_HZ 1000000  // 1MHz, 1us per tick
#define SERVO_TIMEBASE_PERIOD        20000    // 20000 ticks, 20ms

mcpwm_cmpr_handle_t comparators[4] = {NULL};

esp_err_t enable_servo()
{
    ESP_LOGI(TAG_SERVO, "Create timer and operator");
    mcpwm_timer_handle_t timer = NULL;
    mcpwm_timer_config_t timer_config = {
        .group_id = 0,
        .clk_src = MCPWM_TIMER_CLK_SRC_DEFAULT,
        .resolution_hz = SERVO_TIMEBASE_RESOLUTION_HZ,
        .period_ticks = SERVO_TIMEBASE_PERIOD,
        .count_mode = MCPWM_TIMER_COUNT_MODE_UP,
    };

    ESP_ERROR_CHECK(mcpwm_new_timer(&timer_config, &timer));

    mcpwm_oper_handle_t oper = NULL;
    mcpwm_operator_config_t operator_config = {
        .group_id = 0, // operator must be in the same group as the timer
    };

    ESP_ERROR_CHECK(mcpwm_new_operator(&operator_config, &oper));

    ESP_LOGI(TAG_SERVO, "Connect timer and operator");
    ESP_ERROR_CHECK(mcpwm_operator_connect_timer(oper, timer));

    ESP_LOGI(TAG_SERVO, "Create comparators and generators from the operator");
    mcpwm_gen_handle_t generators[4] = {NULL};

    for (int i = 0; i < 4; i++) {
        // Check if the comparator is already initialized
        if (comparators[i] == NULL) {
            mcpwm_comparator_config_t comparator_config = {
                .flags.update_cmp_on_tez = true,
            };
            ESP_ERROR_CHECK(mcpwm_new_comparator(oper, &comparator_config, &comparators[i]));
        }

        mcpwm_generator_config_t generator_config = {
            .gen_gpio_num = (i == 0 ? SERVO_A : (i == 1 ? SERVO_B : (i == 2 ? SERVO_C : SERVO_D))),
        };

        ESP_ERROR_CHECK(mcpwm_new_generator(oper, &generator_config, &generators[i]));
    }
    
    for (int i = 0; i < 4; i++) {
        ESP_ERROR_CHECK(mcpwm_generator_set_action_on_timer_event(generators[i],
                        MCPWM_GEN_TIMER_EVENT_ACTION(MCPWM_TIMER_DIRECTION_UP, MCPWM_TIMER_EVENT_EMPTY, MCPWM_GEN_ACTION_HIGH)));
    }

    for (int i = 0; i < 4; i++) {
        ESP_ERROR_CHECK(mcpwm_timer_enable(timer));
        ESP_ERROR_CHECK(mcpwm_timer_start_stop(timer, MCPWM_TIMER_START_NO_STOP));
    }

    // Set the flag to indicate that servos are enabled
    enabled_servo_flag = 1;

    return ESP_OK;
}    

static esp_err_t set_angle_servo_helper(int servo_pin, int servo_max, int servo_min_pulsewidth, int servo_max_pulsewidth, int cmp_num, unsigned int degree_of_rotation)
{
    degree_of_rotation = degree_of_rotation > servo_max ? servo_max : degree_of_rotation;

    uint32_t cal_pulsewidth = 0;
    cal_pulsewidth = (servo_min_pulsewidth + ((servo_max_pulsewidth - servo_min_pulsewidth) * (degree_of_rotation)) / (servo_max));

    esp_err_t err = mcpwm_comparator_set_compare_value(comparators[cmp_num], cal_pulsewidth);
    if (err == ESP_OK)
    {
        ESP_LOGI(TAG_SERVO, "set servo at pin %d: %ud", servo_pin, degree_of_rotation);
    }
    else
    {
        ESP_LOGE(TAG_SERVO, "error: servo at pin %d: %s", servo_pin, esp_err_to_name(err));
    }

    return err;
}

esp_err_t set_angle_servo(servo_config *config, unsigned int degree_of_rotation)
{
        if (enabled_servo_flag)
    {
        if (config->servo_pin)
        {
            config->angle = degree_of_rotation;
            return set_angle_servo_helper(config->servo_pin, config->max_degree, config->min_pulse_width, config->max_pulse_width, config->cmp_num, degree_of_rotation);
        }
        else
        {
            ESP_LOGE(TAG_SERVO, "error: incorrect servo pin passed to function");
            return ESP_FAIL;
        }
    }
    else
    {
        ESP_LOGE(TAG_SERVO, "error: servos not enabled, call enable_servo() before using servos");
        return ESP_FAIL;
    }
}
