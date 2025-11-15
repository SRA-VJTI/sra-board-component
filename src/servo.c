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
 
mcpwm_cmpr_handle_t comparator_a= NULL;
mcpwm_cmpr_handle_t comparator_b= NULL;
mcpwm_cmpr_handle_t comparator_c= NULL;
mcpwm_cmpr_handle_t comparator_d= NULL;
 
esp_err_t enable_servo()
{
    ESP_LOGI(TAG_SERVO, "Create timer and operator for servos A and B");
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
 
    ESP_LOGI(TAG_SERVO, "Connect timer and operator for servos A and B");
    ESP_ERROR_CHECK(mcpwm_operator_connect_timer(oper, timer));
 
    ESP_LOGI(TAG_SERVO, "Create comparator_a and generator from the operator for servos A and B");
 
    mcpwm_comparator_config_t comparator_config_a = {
        .flags.update_cmp_on_tez = true,
    };
    ESP_ERROR_CHECK(mcpwm_new_comparator(oper, &comparator_config_a, &comparator_a));
 
    mcpwm_comparator_config_t comparator_config_b = {
        .flags.update_cmp_on_tez = true,
    };
    ESP_ERROR_CHECK(mcpwm_new_comparator(oper, &comparator_config_b, &comparator_b));
 
    mcpwm_gen_handle_t generator_a = NULL;
    mcpwm_generator_config_t generator_config_a = {
        .gen_gpio_num = SERVO_A,
    };
    ESP_ERROR_CHECK(mcpwm_new_generator(oper, &generator_config_a, &generator_a));
 
    // Similarly, create generator B for SERVO_B
    mcpwm_gen_handle_t generator_b = NULL;
    mcpwm_generator_config_t generator_config_b = {
        .gen_gpio_num = SERVO_B,
    };
    ESP_ERROR_CHECK(mcpwm_new_generator(oper, &generator_config_b, &generator_b));
 
    // Set actions for generator A and B
    ESP_ERROR_CHECK(mcpwm_generator_set_action_on_timer_event(generator_a,
                    MCPWM_GEN_TIMER_EVENT_ACTION(MCPWM_TIMER_DIRECTION_UP, MCPWM_TIMER_EVENT_EMPTY, MCPWM_GEN_ACTION_HIGH)));
    ESP_ERROR_CHECK(mcpwm_generator_set_action_on_compare_event(generator_a,
                    MCPWM_GEN_COMPARE_EVENT_ACTION(MCPWM_TIMER_DIRECTION_UP, comparator_a, MCPWM_GEN_ACTION_LOW)));
 
    ESP_ERROR_CHECK(mcpwm_generator_set_action_on_timer_event(generator_b,
                    MCPWM_GEN_TIMER_EVENT_ACTION(MCPWM_TIMER_DIRECTION_UP, MCPWM_TIMER_EVENT_EMPTY, MCPWM_GEN_ACTION_HIGH)));
    ESP_ERROR_CHECK(mcpwm_generator_set_action_on_compare_event(generator_b,
                    MCPWM_GEN_COMPARE_EVENT_ACTION(MCPWM_TIMER_DIRECTION_UP, comparator_b, MCPWM_GEN_ACTION_LOW)));
 
 
    // Now, create a new timer, operator, and generators for servos C and D
    ESP_LOGI(TAG_SERVO, "Create timer and operator for servos C and D");
    mcpwm_timer_handle_t timer_1 = NULL;
    mcpwm_timer_config_t timer_config_1 = {
        .group_id = 1,
        .clk_src = MCPWM_TIMER_CLK_SRC_DEFAULT,
        .resolution_hz = SERVO_TIMEBASE_RESOLUTION_HZ,
        .period_ticks = SERVO_TIMEBASE_PERIOD,
        .count_mode = MCPWM_TIMER_COUNT_MODE_UP,
    };
    ESP_ERROR_CHECK(mcpwm_new_timer(&timer_config_1, &timer_1));
 
    mcpwm_oper_handle_t oper_1 = NULL;
    mcpwm_operator_config_t operator_config_1 = {
        .group_id = 1, // operator must be in the same group as the timer
    };
    ESP_ERROR_CHECK(mcpwm_new_operator(&operator_config_1, &oper_1));
 
    ESP_LOGI(TAG_SERVO, "Connect timer and operator for servos C and D");
    ESP_ERROR_CHECK(mcpwm_operator_connect_timer(oper_1, timer_1));
   
    ESP_LOGI(TAG_SERVO, "Create comparator and generator from the operator for servos C and D");
 
    mcpwm_comparator_config_t comparator_config_c = {
        .flags.update_cmp_on_tez = true,
    };
    ESP_ERROR_CHECK(mcpwm_new_comparator(oper_1, &comparator_config_c, &comparator_c));
 
     mcpwm_comparator_config_t comparator_config_d = {
        .flags.update_cmp_on_tez = true,
    };
    ESP_ERROR_CHECK(mcpwm_new_comparator(oper_1, &comparator_config_d, &comparator_d));
 
// Similarly, create generator C for SERVO_C
    mcpwm_gen_handle_t generator_c = NULL;
    mcpwm_generator_config_t generator_config_c = {
        .gen_gpio_num = SERVO_C,
    };
    ESP_ERROR_CHECK(mcpwm_new_generator(oper_1, &generator_config_c, &generator_c));
 
// Similarly, create generator D for SERVO_D
mcpwm_gen_handle_t generator_d = NULL;
mcpwm_generator_config_t generator_config_d = {
    .gen_gpio_num = SERVO_D,
};
ESP_ERROR_CHECK(mcpwm_new_generator(oper_1, &generator_config_d, &generator_d));
 
// Set actions for generator C and D
ESP_ERROR_CHECK(mcpwm_generator_set_action_on_timer_event(generator_c,
                MCPWM_GEN_TIMER_EVENT_ACTION(MCPWM_TIMER_DIRECTION_UP, MCPWM_TIMER_EVENT_EMPTY, MCPWM_GEN_ACTION_HIGH)));
ESP_ERROR_CHECK(mcpwm_generator_set_action_on_compare_event(generator_c,
                MCPWM_GEN_COMPARE_EVENT_ACTION(MCPWM_TIMER_DIRECTION_UP, comparator_c, MCPWM_GEN_ACTION_LOW)));
 
ESP_ERROR_CHECK(mcpwm_generator_set_action_on_timer_event(generator_d,
                MCPWM_GEN_TIMER_EVENT_ACTION(MCPWM_TIMER_DIRECTION_UP, MCPWM_TIMER_EVENT_EMPTY, MCPWM_GEN_ACTION_HIGH)));
ESP_ERROR_CHECK(mcpwm_generator_set_action_on_compare_event(generator_d,
                MCPWM_GEN_COMPARE_EVENT_ACTION(MCPWM_TIMER_DIRECTION_UP, comparator_d, MCPWM_GEN_ACTION_LOW)));
 
 
    // Enable and start both timers
    ESP_LOGI(TAG_SERVO, "Enable and start timers");
    ESP_ERROR_CHECK(mcpwm_timer_enable(timer));
    ESP_ERROR_CHECK(mcpwm_timer_enable(timer_1));
 
    ESP_ERROR_CHECK(mcpwm_timer_start_stop(timer, MCPWM_TIMER_START_NO_STOP));
    ESP_ERROR_CHECK(mcpwm_timer_start_stop(timer_1, MCPWM_TIMER_START_NO_STOP));
 
    // Set the flag to indicate that servos are enabled
    enabled_servo_flag = 1;
 
    return ESP_OK;
}
 
static esp_err_t set_angle_servo_helper(int servo_pin, int servo_max, int servo_min_pulsewidth, int servo_max_pulsewidth, unsigned int degree_of_rotation)
{
    degree_of_rotation = degree_of_rotation > servo_max ? servo_max : degree_of_rotation;
 
    uint32_t cal_pulsewidth = 0;
    cal_pulsewidth = (servo_min_pulsewidth + ((servo_max_pulsewidth - servo_min_pulsewidth) * (degree_of_rotation)) / (servo_max));
 
 
    
    esp_err_t err;
    switch(servo_pin) {
        case SERVO_A:
            err = mcpwm_comparator_set_compare_value(comparator_a, cal_pulsewidth);
            break;
        case SERVO_B:
            err = mcpwm_comparator_set_compare_value(comparator_b, cal_pulsewidth);
            break;
        case SERVO_C:
            err = mcpwm_comparator_set_compare_value(comparator_c, cal_pulsewidth);
            break;
        case SERVO_D:
            err = mcpwm_comparator_set_compare_value(comparator_d, cal_pulsewidth);
            break;
        default:
            err = ESP_ERR_INVALID_ARG;
            break;
    }
 
    if (err == ESP_OK)
    {
        ESP_LOGI(TAG_SERVO, "set servo at pin %d: %d", servo_pin, degree_of_rotation);
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
            return set_angle_servo_helper(config->servo_pin, config->max_degree, config->min_pulse_width, config->max_pulse_width, degree_of_rotation);
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
 
 
int read_servo(servo_config *config)
{
    return config->angle;
}
