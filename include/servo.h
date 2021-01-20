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

#define SERVO_A_MIN_PULSEWIDTH CONFIG_SERVO_A_MIN_PULSEWIDTH
#define SERVO_A_MAX_PULSEWIDTH CONFIG_SERVO_A_MAX_PULSEWIDTH
#define SERVO_A_MAX_DEGREE CONFIG_SERVO_A_MAX_DEGREE

#define SERVO_B_MIN_PULSEWIDTH CONFIG_SERVO_B_MIN_PULSEWIDTH
#define SERVO_B_MAX_PULSEWIDTH CONFIG_SERVO_B_MAX_PULSEWIDTH
#define SERVO_B_MAX_DEGREE CONFIG_SERVO_B_MAX_DEGREE

#define SERVO_C_MIN_PULSEWIDTH CONFIG_SERVO_C_MIN_PULSEWIDTH
#define SERVO_C_MAX_PULSEWIDTH CONFIG_SERVO_C_MAX_PULSEWIDTH
#define SERVO_C_MAX_DEGREE CONFIG_SERVO_C_MAX_DEGREE

/**
 * @brief Enables Servo port on the sra board, sets up PWM for the three pins in servo port.
 * 
 * @return esp_err_t - returns ESP_OK if servo pins initialised, else it returns ESP_ERR_INVALID_ARG 
 **/
esp_err_t enable_servo();

/**
 * @brief Set the angle of the servos attached to the servo port of SRA Board
 * 
 * @param servo_id pin number of the servo whose angle needs to be set
 * @param degree_of_rotation angle to which the servo must be set, depends on value of MAX_DEGREE macro
 * @return esp_err_t 
 */
esp_err_t set_angle_servo(int servo_id, unsigned int degree_of_rotation);

#endif