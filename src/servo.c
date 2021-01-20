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

static esp_err_t set_angle_servo_helper(char *servo_name, int servo_max, int servo_min_pulsewidth, int servo_max_pulsewidth, unsigned int degree_of_rotation, mcpwm_unit_t mcpwm_num, mcpwm_timer_t timer_num, mcpwm_generator_t gen)
{
    degree_of_rotation = degree_of_rotation > servo_max ? servo_max : degree_of_rotation;

    uint32_t cal_pulsewidth = 0;
    cal_pulsewidth = (servo_min_pulsewidth + 2 * (((servo_max_pulsewidth - servo_min_pulsewidth) * (degree_of_rotation)) / (servo_max)));

    esp_err_t err = mcpwm_set_duty_in_us(mcpwm_num, timer_num, gen, cal_pulsewidth);
    if (err == ESP_OK)
    {
        ESP_LOGI(TAG_SERVO, "set %s: %ud", servo_name, degree_of_rotation);
    }
    else
    {
        ESP_LOGE(TAG_SERVO, "error: %s: %s", servo_name, esp_err_to_name(err));
    }

    return err;
}

esp_err_t set_angle_servo(int servo_id, unsigned int degree_of_rotation)
{
    if (enabled_servo_flag)
    {
        if (servo_id == SERVO_A)
        {
            return set_angle_servo_helper(STR(SERVO_A), SERVO_A_MAX_DEGREE, SERVO_A_MIN_PULSEWIDTH, SERVO_A_MAX_PULSEWIDTH, degree_of_rotation, MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A);
        }
        else if (servo_id == SERVO_B)
        {
            return set_angle_servo_helper(STR(SERVO_B), SERVO_B_MAX_DEGREE, SERVO_B_MIN_PULSEWIDTH, SERVO_B_MAX_PULSEWIDTH, degree_of_rotation, MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_B);
        }
        else if (servo_id == SERVO_C)
        {
            return set_angle_servo_helper(STR(SERVO_C), SERVO_C_MAX_DEGREE, SERVO_C_MIN_PULSEWIDTH, SERVO_C_MAX_PULSEWIDTH, degree_of_rotation, MCPWM_UNIT_0, MCPWM_TIMER_1, MCPWM_OPR_A);
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
