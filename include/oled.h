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
#include "u8g2.h"
#include "u8g2_esp32_hal.h"
#endif

/**
 * @brief enables and configures OLED display
 
 * @param pointer to empty u8g2_t struct

 * @return esp_err_t i.e it shows if OLED is initialised successfully or not
 **/
esp_err_t init_oled(u8g2_t* u8g2);

/**
 * @brief displays SRA logo on OLED screen
 
 * @param pointer to u8g2_t struct

 * @return esp_err_t i.e it shows if SRA logo is displayed successfully or not
 **/
esp_err_t display_sra_logo(u8g2_t* u8g2);

/**
 * @brief displays Wall-E Bitmap logo on OLED screen
 
 * @param pointer to u8g2_t struct

 * @return esp_err_t i.e it shows if Wall-E logo is displayed successfully or not
 **/
esp_err_t display_walle_logo(u8g2_t* u8g2);

/**
 * @brief displays LSA bar on OLED screen
 
 * @param pointer to u8g2_t struct

 * @return esp_err_t i.e it shows if LSA bar is displayed successfully or not
 **/
esp_err_t display_lsa(line_sensor_array readings,u8g2_t *u8g2);

/**
 * @brief displays pitch and roll readings on OLED screen
 
 * @param pointer to u8g2_t struct

 * @return esp_err_t i.e it shows if Pitch and Roll values are displayed successfully or not
 **/
esp_err_t display_mpu(float pitch,float roll,u8g2_t *u8g2);

#endif