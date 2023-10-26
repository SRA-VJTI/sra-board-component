#include "esp_log.h"
#include "esp_check.h"
#include "motor_driver.h"
#include "utils.h"
#include "pin_defs.h"

static const char *TAG = "MOTOR";

esp_err_t enable_motor_driver(motor_handle_t *motor, int motor_id) { 
	motor_config_t motor_config;
	if(motor_id == MOTOR_A_0){
		motor_config.pwma_gpio_num = MDA_NORMAL_IN_1,
		motor_config.pwmb_gpio_num = MDA_NORMAL_IN_2,
		motor_config.pwm_freq_hz = 20000; // 20kHz PWM frequency
	} else if(motor_id == MOTOR_A_1) {
		motor_config.pwma_gpio_num = MDA_NORMAL_IN_3,
		motor_config.pwmb_gpio_num = MDA_NORMAL_IN_4,
		motor_config.pwm_freq_hz = 20000; // 20kHz PWM frequency
	} else {
		ESP_LOGE(TAG, "Invalid motor id");
		return ESP_FAIL;
	}
	motor_mcpwm_config_t motor_mcpwm_config;
	motor_mcpwm_config.group_id = 0;
	motor_mcpwm_config.resolution_hz = 10000000; // 10MHz
	ESP_ERROR_CHECK(motor_new_mcpwm_device(&motor_config, &motor_mcpwm_config, motor));
	ESP_ERROR_CHECK((*motor)->enable(*motor));
	return ESP_OK;
}

esp_err_t set_motor_speed(motor_handle_t motor, int direction, float speed) {
	speed = (uint32_t)map(speed, 0, 100, 0, 500);
	if(direction == MOTOR_FORWARD){
		motor->forward(motor);
		motor->set_speed(motor, speed);
		return ESP_OK;
	} else if(direction == MOTOR_BACKWARD){
		motor->backwards(motor);
		motor->set_speed(motor, speed);
		return ESP_OK;
	} else if(direction == MOTOR_STOP){
		motor->brake(motor);
		return ESP_OK;
	} else {
		ESP_LOGE(TAG, "Invalid motor direction");
		return ESP_FAIL;
	}
}
