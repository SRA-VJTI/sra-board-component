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

#ifndef _ADC_H
#define _ADC_H

#include <stdio.h>
#include <stdlib.h>

#include "driver/adc.h"
#include "esp_adc_cal.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"

#include "esp_log.h"
#include "esp_err.h"
#include "pin_defs.h"

#define DEFAULT_VREF 1100

/**
 * @brief Configure width and attenuation of ADC 1
 * 
 * @return esp_err_t - returns ESP_OK if 11dB attenuation and 12 bit width configured to ADC 1, else it returns ESP_ERR_INVALID_ARG.  
 **/
esp_err_t config_adc1();

/**
 * @brief Characterize ADC 1 using either Vref or Two Point
 * 
 * @return esp_err_t 
 **/
esp_err_t characterize_adc1();

/**
 * @brief call function config_adc1() and characterize_adc1().
 * 
 * @return esp_err_t - returns ESP_OK if Configuration and Characterization of adc1 is successful, else it returns ESP_ERR_INVALID_ARG.  
 **/
esp_err_t enable_adc1();

/**
 * @brief Read raw adc value of given adc pin.
 * 
 * @param adc_pin One of the GPIO pin to which LSA is connected (36/39/35/34)
 * 
 * @return esp_err_t -returns raw reading of adc pin if lsa pin is passed to function, else it returns ESP_ERR_INVALID_ARG.
 **/
int read_adc(int adc_pin);

#endif
