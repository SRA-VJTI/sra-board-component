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

#include "motor_driver.h"
#include "utils.h"
#include "pin_defs.h"

static const char *TAG = "MOTOR";

static int enabled_motor_driver_flag = 0;

esp_err_t enable_motor_driver(motor_handle_t *motor, int motor_id) { 
	motor_config_t motor_config;
	if(motor_id == MOTOR_A_0){
		motor_config.pwma_gpio_num = MDA_NORMAL_IN_1,
		motor_config.pwmb_gpio_num = MDA_NORMAL_IN_2,
		motor_config.pwm_freq_hz = MCPWM_FREQ; // 20kHz PWM frequency
	} else if(motor_id == MOTOR_A_1) {
		motor_config.pwma_gpio_num = MDA_NORMAL_IN_3,
		motor_config.pwmb_gpio_num = MDA_NORMAL_IN_4,
		motor_config.pwm_freq_hz = MCPWM_FREQ; // 20kHz PWM frequency
	} else {
		ESP_LOGE(TAG, "Invalid motor id");
		return ESP_FAIL;
	}
	motor_mcpwm_config_t motor_mcpwm_config;
	motor_mcpwm_config.group_id = 0;
	motor_mcpwm_config.resolution_hz = MCPWM_RESOLUTION; // 10MHz
	ESP_ERROR_CHECK(motor_new_mcpwm_device(&motor_config, &motor_mcpwm_config, motor));
	ESP_ERROR_CHECK((*motor)->enable(*motor));
	enabled_motor_driver_flag = 1;
	return ESP_OK;
}

esp_err_t set_motor_speed(motor_handle_t motor, int direction, float speed) {
	// map is used to map the speed from 0-100 to 0-MCPWM_TICKS. MCPWM_TICKS is the resolution of the pwm signal. It is calculated as MCPWM_TICKS = MCPWM_RESOLUTION / MCPWM_FREQ
	speed = (uint32_t)map(speed, 0, 100, 0, MCPWM_RESOLUTION / MCPWM_FREQ);
	if(direction == MOTOR_FORWARD){
		motor->forward(motor);
		motor->set_speed(motor, speed);
		return ESP_OK;
	} else if(direction == MOTOR_BACKWARD){
		motor->backwards(motor);
		motor->set_speed(motor, speed);
		return ESP_OK;
	} else if(direction == MOTOR_STOP){
		motor->set_speed(motor, 0);
		return ESP_OK;
	} else {
		ESP_LOGE(TAG, "Invalid motor direction");
		return ESP_FAIL;
	}
}

int get_motor_driver_status() {
	return enabled_motor_driver_flag;
}
