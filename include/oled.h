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

#ifndef OLED_H
#define OLED_H

#include <stdio.h>
#include <string.h>

#include <driver/gpio.h>
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "pin_defs.h"
#include "lsa.h"

#ifdef CONFIG_ENABLE_OLED
#include "lvgl.h"
#include "lvgl_helpers.h"
#endif

#define SRA_LOGO 1
#define WALLE_LOGO 2
#define MARIO_LOGO 3
#define WALLE_TEXT CONFIG_WALL_E_LOGO_TEXT
#define MARIO_TEXT CONFIG_MARIO_LOGO_TEXT

/**
 * @brief enables and configures OLED display

 * @param None

 * @return esp_err_t i.e it shows if OLED is initialised successfully or not
 **/
esp_err_t init_oled();

/**
 * @brief displays SRA/Wall-E/Mario logo on OLED screen

 * @param logo_id macro 

 * @return esp_err_t i.e it shows if logo is displayed successfully or not
 **/
esp_err_t display_logo(int logo_id);

/**
 * @brief displays LSA bar on OLED screen

 * @param readings line_sensor_array struct

 * @return esp_err_t i.e it shows if LSA bar is displayed successfully or not
 **/
esp_err_t display_lsa(line_sensor_array readings);

/**
 * @brief displays pitch and roll readings on OLED screen

 * @param pitch Value of pitch in float
 * @param roll Value of Roll in float

 * @return esp_err_t i.e it shows if Pitch and Roll values are displayed successfully or not
 **/
esp_err_t display_mpu(float pitch, float roll);

/**
 * @brief displays kp, ki and kd reading on the OLED screen

 * @param kp value of kp in float
 * @param ki value of ki in float
 * @param kd value of kd in float

 * @return esp_err_t i.e it shows if Pitch and Roll values are displayed successfully or not
 **/
esp_err_t display_pid_values(float kp, float ki, float kd);

/**
 * @brief displays Servo angles on the OLED screen

 * @param s1 value of Servo_A in float
 * @param s2 value of Servo_B in float
 * @param s3 value of Servo_C in float
 * @param s4 value of Servo_D in float

 * @return esp_err_t i.e it shows if Pitch and Roll values are displayed successfully or not
 **/
esp_err_t display_servo_values(int s1, int s2, int s3, int s4);

#endif