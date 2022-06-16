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
#define STR(A) #A

esp_err_t enable_servo()
{
    esp_err_t err;
    CHECK_LOGE(err, mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM0A, SERVO_A), TAG_SERVO, "error: servo A: %s", esp_err_to_name(err));
    CHECK_LOGE(err, mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM0B, SERVO_B), TAG_SERVO, "error: servo B: %s", esp_err_to_name(err));
    CHECK_LOGE(err, mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM1A, SERVO_C), TAG_SERVO, "error: servo C: %s", esp_err_to_name(err));
    CHECK_LOGE(err, mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM1B, SERVO_D), TAG_SERVO, "error: servo D: %s", esp_err_to_name(err));

    mcpwm_config_t pwm_config;
    // sets the pwm frequency = 50
    pwm_config.frequency = 50;
    // sets the initial duty cycle of PWMxA = 0
    pwm_config.cmpr_a = 0;
    // sets the initial duty cycle of PWMxB = 0
    pwm_config.cmpr_b = 0;
    // sets the pwm counter mode
    pwm_config.counter_mode = MCPWM_UP_COUNTER;
    // sets the pwm duty mode
    pwm_config.duty_mode = MCPWM_DUTY_MODE_0;

    // init pwm 0a, 1a, 2a with the above settings

    esp_err_t err_A = mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_0, &pwm_config);
    esp_err_t err_B = mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_1, &pwm_config);

    if (err_A == ESP_OK && err_B == ESP_OK)
    {
        enabled_servo_flag = 1;
        ESP_LOGI(TAG_SERVO, "enabled servos");

        return ESP_OK;
    }
    else
    {
        enabled_servo_flag = 0;
        return ESP_FAIL;
    }
}

static esp_err_t set_angle_servo_helper(int servo_pin, int servo_max, int servo_min_pulsewidth, int servo_max_pulsewidth, unsigned int degree_of_rotation, mcpwm_unit_t mcpwm_num, mcpwm_timer_t timer_num, mcpwm_generator_t gen)
{
    degree_of_rotation = degree_of_rotation > servo_max ? servo_max : degree_of_rotation;

    uint32_t cal_pulsewidth = 0;
    cal_pulsewidth = (servo_min_pulsewidth + ((servo_max_pulsewidth - servo_min_pulsewidth) * (degree_of_rotation)) / (servo_max));

    esp_err_t err = mcpwm_set_duty_in_us(mcpwm_num, timer_num, gen, cal_pulsewidth);
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
            return set_angle_servo_helper(config->servo_pin, config->max_degree, config->min_pulse_width, config->max_pulse_width, degree_of_rotation, config->mcpwm_num, config->timer_num, config->gen);
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
