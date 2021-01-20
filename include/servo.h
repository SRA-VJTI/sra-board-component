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