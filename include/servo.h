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

#ifndef SERVO_H
#define SERVO_H

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "driver/mcpwm.h"
#include "soc/mcpwm_periph.h"
#include "esp_attr.h"

#include "sdkconfig.h"
#include "esp_log.h"
#include "esp_err.h"
#include "pin_defs.h"

typedef struct
{
    int servo_pin;
    int min_pulse_width;
    int max_pulse_width;
    int max_degree;
    int angle;
    mcpwm_unit_t mcpwm_num;
    mcpwm_timer_t timer_num;
    mcpwm_generator_t gen;
} servo_config;

/** @struct servo_config
 *  @brief This structure contains the configuration of servos
 *  @var servo_config::servo_pin
 *  Member 'servo_pin' contains the gpio pin number to which servo is connected
 *  @var servo_config::min_pulse_width
 *  Member 'min_pulse_width' contains the minimum pulse width of servo motor
 *  @var servo_config::max_pulse_width
 *  Member 'max_pulse_width' contains the maximum pulse width of servo motor
 *  @var servo_config::max_degree
 *  Member 'max_degree' contains the maximum degree servo motor can rotate
 *  @var servo_config::mcpwm_num
 *  Member 'mcpwm_num' contains MCPWM unit to use
 *  @var servo_config::timer_num
 *  Member 'timer_num' contains MCPWM timer to use
 *  @var servo_config::gen
 *  Member 'gen' contains MCPWM operator to use
 */

/**
 * @brief Enables Servo port on the sra board, sets up PWM for the three pins in servo port.
 *
 * @return esp_err_t - returns ESP_OK if servo pins initialised, else it returns ESP_ERR_INVALID_ARG
 **/
esp_err_t enable_servo();

/**
 * @brief Set the angle of the servos attached to the servo port of SRA Board
 *
 * @param config pointer to the servo_config struct
 * @param degree_of_rotation angle to which the servo must be set, depends on value of MAX_DEGREE macro
 * @return esp_err_t
 */
esp_err_t set_angle_servo(servo_config *config, unsigned int degree_of_rotation);

/**
 * @brief Get the angle of the servos
 * @return esp_err_t
 */
int read_servo(servo_config *config);

#endif