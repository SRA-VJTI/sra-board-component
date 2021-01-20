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
#include "motor_driver.h"

static const char *TAG_MOTOR_DRIVER = "motor_driver";
static int mode_motor_driver_a = 0;
static int mode_motor_driver_b = 0;

esp_err_t enable_motor_driver_a(int mode)
{
    esp_err_t err;

    if (mode == PARALLEL_MODE)
    {
        CHECK_LOGE(err, mcpwm_gpio_init(MCPWM_UNIT_1, MCPWM0A, MDA_PARALLEL_IN_1_2), TAG_MOTOR_DRIVER, "error: %s", esp_err_to_name(err));
        CHECK_LOGE(err, mcpwm_gpio_init(MCPWM_UNIT_1, MCPWM0B, MDA_PARALLEL_IN_3_4), TAG_MOTOR_DRIVER, "error: %s", esp_err_to_name(err));
    }
    else if (mode == NORMAL_MODE)
    {
        CHECK_LOGE(err, mcpwm_gpio_init(MCPWM_UNIT_1, MCPWM0A, MDA_NORMAL_IN_1), TAG_MOTOR_DRIVER, "error: %s", esp_err_to_name(err));
        CHECK_LOGE(err, mcpwm_gpio_init(MCPWM_UNIT_1, MCPWM0B, MDA_NORMAL_IN_2), TAG_MOTOR_DRIVER, "error: %s", esp_err_to_name(err));
        CHECK_LOGE(err, mcpwm_gpio_init(MCPWM_UNIT_1, MCPWM1A, MDA_NORMAL_IN_3), TAG_MOTOR_DRIVER, "error: %s", esp_err_to_name(err));
        CHECK_LOGE(err, mcpwm_gpio_init(MCPWM_UNIT_1, MCPWM1B, MDA_NORMAL_IN_4), TAG_MOTOR_DRIVER, "error: %s", esp_err_to_name(err));
    }
    else
    {
        ESP_LOGE(TAG_MOTOR_DRIVER, "error: %s", "invalid motor driver mode selected");
        return ESP_FAIL;
    }

    mcpwm_config_t pwm_config;
    // sets the pwm frequency = 20000
    pwm_config.frequency = 20000;
    // sets the initial duty cycle of PWMxA = 0
    pwm_config.cmpr_a = 0;
    // sets the initial duty cycle of PWMxB = 0
    pwm_config.cmpr_b = 0;
    // sets the pwm counter mode
    pwm_config.counter_mode = MCPWM_UP_COUNTER;
    // sets the pwm duty mode
    pwm_config.duty_mode = MCPWM_DUTY_MODE_0;

    if (mode == PARALLEL_MODE)
    {
        esp_err_t err1_0 = mcpwm_init(MCPWM_UNIT_1, MCPWM_TIMER_0, &pwm_config);

        if (err1_0 == ESP_OK)
        {
            ESP_LOGI(TAG_MOTOR_DRIVER, "enabled motor driver A in parallel mode");

            mode_motor_driver_a = 1;
            return ESP_OK;
        }
        else
        {
            mode_motor_driver_a = 0;
            return ESP_FAIL;
        }
    }
    else if (mode == NORMAL_MODE)
    {
        esp_err_t err1_0 = mcpwm_init(MCPWM_UNIT_1, MCPWM_TIMER_0, &pwm_config);
        esp_err_t err1_1 = mcpwm_init(MCPWM_UNIT_1, MCPWM_TIMER_1, &pwm_config);

        if (err1_0 == ESP_OK && err1_1 == ESP_OK)
        {
            ESP_LOGI(TAG_MOTOR_DRIVER, "enabled motor driver A in normal mode");
            mode_motor_driver_a = 2;

            return ESP_OK;
        }
        else
        {
            mode_motor_driver_a = 0;
            return ESP_FAIL;
        }
    }
    else
    {
        ESP_LOGE(TAG_MOTOR_DRIVER, "error: %s", "invalid motor driver mode selected");
        return ESP_FAIL;
    }
}

esp_err_t enable_motor_driver_b(int mode)
{
    esp_err_t err;

    if (mode == PARALLEL_MODE)
    {
        CHECK_LOGE(err, mcpwm_gpio_init(MCPWM_UNIT_1, MCPWM1A, MDB_PARALLEL_IN_5_7), TAG_MOTOR_DRIVER, "error: %s", esp_err_to_name(err));
        CHECK_LOGE(err, mcpwm_gpio_init(MCPWM_UNIT_1, MCPWM1B, MDB_PARALLEL_IN_6_8), TAG_MOTOR_DRIVER, "error: %s", esp_err_to_name(err));
    }
    else if (mode == NORMAL_MODE)
    {
        CHECK_LOGE(err, mcpwm_gpio_init(MCPWM_UNIT_1, MCPWM2A, MDB_NORMAL_IN_5), TAG_MOTOR_DRIVER, "error: %s", esp_err_to_name(err));
        CHECK_LOGE(err, mcpwm_gpio_init(MCPWM_UNIT_1, MCPWM2B, MDB_NORMAL_IN_6), TAG_MOTOR_DRIVER, "error: %s", esp_err_to_name(err));
        CHECK_LOGE(err, mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM2A, MDB_NORMAL_IN_7), TAG_MOTOR_DRIVER, "error: %s", esp_err_to_name(err));
        CHECK_LOGE(err, mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM2B, MDB_NORMAL_IN_8), TAG_MOTOR_DRIVER, "error: %s", esp_err_to_name(err));
    }
    else
    {
        ESP_LOGE(TAG_MOTOR_DRIVER, "error: %s", "invalid motor driver mode selected");
        return ESP_FAIL;
    }

    mcpwm_config_t pwm_config;
    // sets the pwm frequency = 20000
    pwm_config.frequency = 20000;
    // sets the initial duty cycle of PWMxA = 0
    pwm_config.cmpr_a = 0;
    // sets the initial duty cycle of PWMxB = 0
    pwm_config.cmpr_b = 0;
    // sets the pwm counter mode
    pwm_config.counter_mode = MCPWM_UP_COUNTER;
    // sets the pwm duty mode
    pwm_config.duty_mode = MCPWM_DUTY_MODE_0;

    if (mode == PARALLEL_MODE)
    {
        esp_err_t err1_1 = mcpwm_init(MCPWM_UNIT_1, MCPWM_TIMER_1, &pwm_config);

        if (err1_1 == ESP_OK)
        {
            ESP_LOGI(TAG_MOTOR_DRIVER, "enabled motor driver B in parallel mode");

            mode_motor_driver_b = 1;
            return ESP_OK;
        }
        else
        {
            mode_motor_driver_b = 0;
            return ESP_FAIL;
        }
    }
    else if (mode == NORMAL_MODE)
    {
        esp_err_t err0_2 = mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_2, &pwm_config);
        esp_err_t err1_2 = mcpwm_init(MCPWM_UNIT_1, MCPWM_TIMER_2, &pwm_config);

        if (err0_2 == ESP_OK && err1_2 == ESP_OK)
        {
            ESP_LOGI(TAG_MOTOR_DRIVER, "enabled motor driver B in normal mode");

            mode_motor_driver_b = 2;
            return ESP_OK;
        }
        else
        {
            mode_motor_driver_b = 0;
            return ESP_FAIL;
        }
    }
    else
    {
        ESP_LOGE(TAG_MOTOR_DRIVER, "error: %s", "invalid motor driver mode selected");
        return ESP_FAIL;
    }
}

static esp_err_t set_motor_speed_helper(int direction, float duty_cycle, mcpwm_unit_t mcpwm_num, mcpwm_timer_t timer_num)
{
    if (direction == MOTOR_FORWARD)
    {
        mcpwm_set_duty(mcpwm_num, timer_num, MCPWM_OPR_A, duty_cycle);
        mcpwm_set_duty_type(mcpwm_num, timer_num, MCPWM_OPR_A, MCPWM_DUTY_MODE_0);
        mcpwm_set_duty(mcpwm_num, timer_num, MCPWM_OPR_B, 0);
        mcpwm_set_duty_type(mcpwm_num, timer_num, MCPWM_OPR_B, MCPWM_DUTY_MODE_0);

        return ESP_OK;
    }
    else if (direction == MOTOR_BACKWARD)
    {
        mcpwm_set_duty(mcpwm_num, timer_num, MCPWM_OPR_A, 0);
        mcpwm_set_duty_type(mcpwm_num, timer_num, MCPWM_OPR_A, MCPWM_DUTY_MODE_0);
        mcpwm_set_duty(mcpwm_num, timer_num, MCPWM_OPR_B, duty_cycle);
        mcpwm_set_duty_type(mcpwm_num, timer_num, MCPWM_OPR_B, MCPWM_DUTY_MODE_0);

        return ESP_OK;
    }
    else if (direction == MOTOR_STOP)
    {
        mcpwm_set_duty(mcpwm_num, timer_num, MCPWM_OPR_A, 0);
        mcpwm_set_duty_type(mcpwm_num, timer_num, MCPWM_OPR_A, MCPWM_DUTY_MODE_0);
        mcpwm_set_duty(mcpwm_num, timer_num, MCPWM_OPR_B, 0);
        mcpwm_set_duty_type(mcpwm_num, timer_num, MCPWM_OPR_B, MCPWM_DUTY_MODE_0);

        return ESP_OK;
    }
    else
    {
        ESP_LOGE(TAG_MOTOR_DRIVER, "invalid motor direction selected");
        return ESP_FAIL;
    }
}

esp_err_t set_motor_speed(int motor_id, int direction, float duty_cycle)
{
    if ((motor_id == MOTOR_A_0 || motor_id == MOTOR_A_1) && read_motor_driver_mode(a) != 0)
    {
        if (read_motor_driver_mode(a) == PARALLEL_MODE)
        {
            return set_motor_speed_helper(direction, duty_cycle, MCPWM_UNIT_1, MCPWM_TIMER_0);
        }
        else if (read_motor_driver_mode(a) == NORMAL_MODE)
        {
            mcpwm_timer_t timer_val = motor_id == MOTOR_A_0 ? MCPWM_TIMER_0 : MCPWM_TIMER_1;

            return set_motor_speed_helper(direction, duty_cycle, MCPWM_UNIT_1, timer_val);
        }
        else
        {
            ESP_LOGE(TAG_MOTOR_DRIVER, "error: %s", "motor driver A is disabled, call enable_motor_driver(a)");
            return ESP_FAIL;
        }
    }
    else if ((motor_id == MOTOR_B_0 || motor_id == MOTOR_B_1) && read_motor_driver_mode(b) != 0)
    {
        if (read_motor_driver_mode(b) == PARALLEL_MODE)
        {
            return set_motor_speed_helper(direction, duty_cycle, MCPWM_UNIT_1, MCPWM_TIMER_1);
        }
        else if (read_motor_driver_mode(b) == NORMAL_MODE)
        {
            mcpwm_unit_t unit_val = motor_id == MOTOR_B_0 ? MCPWM_UNIT_1 : MCPWM_UNIT_0;

            return set_motor_speed_helper(direction, duty_cycle, unit_val, MCPWM_TIMER_2);
        }
        else
        {
            ESP_LOGE(TAG_MOTOR_DRIVER, "error: %s", "motor driver A is disabled, call enable_motor_driver(a)");
            return ESP_FAIL;
        }
    }
    else
    {
        ESP_LOGE(TAG_MOTOR_DRIVER, "invalid motor_id selected");
        return ESP_FAIL;
    }
}

int read_motor_driver_mode_a()
{
    return mode_motor_driver_a;
}

int read_motor_driver_mode_b()
{
    return mode_motor_driver_b;
}
