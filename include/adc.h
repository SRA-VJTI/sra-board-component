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

#define DEFAULT_VREF    1100        
#define NO_OF_SAMPLES   64 

esp_adc_cal_characteristics_t *adc_chars;

/**
 * @brief Configure width and attenuation of ADC 1
 * 
 * @param channel[] Array of ESP32 pins to which LSA is connected.
 * 
 * @return esp_err_t - returns ESP_OK if 11dB attenuation and 12 bit width configured to ADC 1, else it returns ESP_ERR_INVALID_ARG.  
 * 
 **/
esp_err_t config_adc1(int channel[]);

/**
 * @brief Characterize ADC 1 using either Vref or Two Point
 * 
 * @return esp_err_t 
 **/
esp_err_t char_adc1();

/**
 * @brief Read raw adc value of given adc pin.
 * 
 * @param Esp32 One of the GPIO pin to which LSA is connected (36/39/35/34)
 * 
 * @return esp_err_t -returns raw reading of adc pin if lsa pin is passed to function, else it returns ESP_ERR_INVALID_ARG.
 **/
esp_err_t read_adc(int adc_pin);

#endif