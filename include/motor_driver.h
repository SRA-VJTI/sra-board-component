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

#ifndef MOTOR_DRIVER_H
#define MOTOR_DRIVER_H

#include "esp_err.h"
#include "esp_log.h"
#include "mcpwm_interface.h"

#define MOTOR_A_0 100
#define MOTOR_A_1 101

#define MOTOR_FORWARD 200
#define MOTOR_BACKWARD 201
#define MOTOR_STOP 202

#define MCPWM_FREQ 20000
#define MCPWM_RESOLUTION 10000000
/**
 *  @brief Enable motor driver
 *  @param motor_handle_t Motor object handle
 *  @param motor_id set it as MOTOR_A_0, MOTOR_A_1 to select the appropriate motor to bind to the handle.
 *  @return
 *    - ESP_OK: Success
 *    - ESP_ERR_INVALID_ARG: Invalid argument
 *    - ESP_FAIL: Failed
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


/**
 * @brief Checks if the motor driver is engaged.
 *
 * @return true if the motor driver is engaged, false otherwise.
 */
int get_motor_driver_status();

#endif
