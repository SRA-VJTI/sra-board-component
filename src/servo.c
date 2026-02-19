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
#include "esp_check.h"

static const char *TAG_SERVO = "servo";
static int enabled_servo_flag = 0;

#define SERVO_TIMEBASE_RESOLUTION_HZ 1000000   //timer frequency 1MHz
#define SERVO_TIMEBASE_PERIOD 20000   //pwm signal frequency 50Hz

esp_err_t enable_servo_helper(servo_config *config , mcpwm_timer_handle_t timer, mcpwm_oper_handle_t operator)
{
    mcpwm_comparator_config_t cmp_config = {
        .flags.update_cmp_on_tez = true,
    };
    ESP_RETURN_ON_ERROR(mcpwm_new_comparator(operator,&cmp_config,&config->comparator),TAG_SERVO,"failed to create new comparator for servo %d",config->servo_pin);
    mcpwm_generator_config_t gen_config = {
        .gen_gpio_num = config->servo_pin,
    };
    ESP_RETURN_ON_ERROR(mcpwm_new_generator(operator,&gen_config,&config->generator),TAG_SERVO,"failed to create new generator for seervo %d",config->servo_pin);
    ESP_RETURN_ON_ERROR(mcpwm_generator_set_action_on_timer_event(config->generator,MCPWM_GEN_TIMER_EVENT_ACTION(MCPWM_TIMER_DIRECTION_UP,MCPWM_TIMER_EVENT_EMPTY,MCPWM_GEN_ACTION_HIGH)),TAG_SERVO,"failed to set high action for servo %d",config->servo_pin);
    ESP_RETURN_ON_ERROR(mcpwm_generator_set_action_on_compare_event(config->generator,MCPWM_GEN_COMPARE_EVENT_ACTION(MCPWM_TIMER_DIRECTION_UP,config->comparator,MCPWM_GEN_ACTION_LOW)),TAG_SERVO,"failedd to set low action for servo %d",config->servo_pin);
    uint32_t init_pulse = (uint32_t)(config->min_pulse_width);
    ESP_RETURN_ON_ERROR(mcpwm_comparator_set_compare_value(config->comparator,init_pulse),TAG_SERVO,"failed to set initial comparator value for servo %d ",config->servo_pin);
    return ESP_OK;
}
esp_err_t enable_servo()
{
    extern servo_config servo_a,servo_b,servo_c,servo_d ;
    mcpwm_timer_handle_t timer_ab = NULL;
    mcpwm_timer_handle_t timer_cd = NULL;
    mcpwm_oper_handle_t operator_ab = NULL;
    mcpwm_oper_handle_t operator_cd = NULL;
    mcpwm_timer_config_t timer_config = {
        .group_id =0,
        .clk_src = MCPWM_TIMER_CLK_SRC_DEFAULT,
        .resolution_hz = SERVO_TIMEBASE_RESOLUTION_HZ,
        .count_mode = MCPWM_TIMER_COUNT_MODE_UP,
        .period_ticks = SERVO_TIMEBASE_PERIOD,
    };
    ESP_RETURN_ON_ERROR(mcpwm_new_timer(&timer_config,&timer_ab),TAG_SERVO,"failed to create timer A and B");
    ESP_RETURN_ON_ERROR(mcpwm_new_timer(&timer_config,&timer_cd),TAG_SERVO,"failedd to create timer C and D");
    mcpwm_operator_config_t oper_config = {
        .group_id = 0,
    };

    ESP_RETURN_ON_ERROR(mcpwm_new_operator(&oper_config,&operator_ab),TAG_SERVO,"failed to create operator A and B");
    ESP_RETURN_ON_ERROR(mcpwm_new_operator(&oper_config,&operator_cd),TAG_SERVO,"failed to create operator C and D");

    ESP_RETURN_ON_ERROR(mcpwm_operator_connect_timer(operator_ab,timer_ab),TAG_SERVO,"failed to connect operator and timer A and B");
    ESP_RETURN_ON_ERROR(mcpwm_operator_connect_timer(operator_cd,timer_cd ),TAG_SERVO,"failedd to connect operator and timer C and D");

    ESP_RETURN_ON_ERROR(enable_servo_helper(&servo_a,timer_ab,operator_ab),TAG_SERVO,"failed to enable servo A");
    ESP_RETURN_ON_ERROR(enable_servo_helper(&servo_b,timer_ab,operator_ab),TAG_SERVO,"failed to enable servo B");
    ESP_RETURN_ON_ERROR(enable_servo_helper(&servo_c,timer_cd,operator_cd),TAG_SERVO,"failed to enable servo C");
    ESP_RETURN_ON_ERROR(enable_servo_helper(&servo_d ,timer_cd,operator_cd),TAG_SERVO,"failed to enable servo D");

    ESP_RETURN_ON_ERROR(mcpwm_timer_enable(timer_ab),TAG_SERVO,"failed to enable timer A and B");
    ESP_RETURN_ON_ERROR(mcpwm_timer_enable(timer_cd),TAG_SERVO,"failed to enable timer C and D");

    ESP_RETURN_ON_ERROR(mcpwm_timer_start_stop(timer_ab,MCPWM_TIMER_START_NO_STOP),TAG_SERVO,"failed to start timer A and B");
    ESP_RETURN_ON_ERROR(mcpwm_timer_start_stop(timer_cd,MCPWM_TIMER_START_NO_STOP),TAG_SERVO,"failed to start timer C and D");

    enabled_servo_flag = 1;
    ESP_LOGI(TAG_SERVO,"all servos enabled");
    return ESP_OK;

}

esp_err_t set_angle_servo(servo_config *config, unsigned int degree_of_rotation)
{
    if(degree_of_rotation > (unsigned int)config->max_degree){
        degree_of_rotation = (unsigned int)config->max_degree;
    }
    uint32_t pulse_width = (uint32_t)(config->min_pulse_width)+((uint32_t)(config->max_pulse_width - config->min_pulse_width)*(degree_of_rotation))/(uint32_t)config->max_degree;
    if (enabled_servo_flag)
    {
        ESP_RETURN_ON_ERROR(mcpwm_comparator_set_compare_value(config->comparator,pulse_width),TAG_SERVO,"error setting angle on pin %d",config->servo_pin);
        config->angle = (int)(degree_of_rotation);
        return ESP_OK;
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
