#pragma once

#include "esp_err.h"
#include "mcpwm_interface.h"

#define MOTOR_A_0 100
#define MOTOR_A_1 101

#define MOTOR_FORWARD 200
#define MOTOR_BACKWARD 201
#define MOTOR_STOP 202

/**
 *  @brief Enable motor driver
 *  @param motor_handle_t Motor object handle
 *  @param motor_id set it as MOTOR_A_0, MOTOR_A_1 to select the appropriate motor to bind to the handle.
 *  @return
 *    - ESP_OK: Success
 * - ESP_ERR_INVALID_ARG: Invalid argument
 * - ESP_FAIL: Failed
 */
esp_err_t enable_motor_driver(motor_handle_t *motor, int motor_id);

/*
 * @brief Set duty cycle for one of the motors.
 * @param motor_id set it as MOTOR_A_0, MOTOR_A_1 to select the appropriate motor to set its speed and direction
 * @param direction set is as MOTOR_FORWARD for forward motion, MOTOR_BACKWARD for backward motion, MOTOR_STOP to stop the motor
 * @param duty_cycle set the duty cycle of the motor driver PWM
 * @return 
 * 	- ESP_OK if speed correctly
 * 	- ESP_FAIL if any error occurs
*/
esp_err_t set_motor_speed(motor_handle_t motor, int direction, float duty_cycle);
