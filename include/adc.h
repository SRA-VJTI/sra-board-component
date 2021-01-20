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