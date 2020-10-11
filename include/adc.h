#ifndef ADC_H
#define ADC_H

#include <stdio.h>
#include <stdlib.h>

#include "driver/adc.h"
#include "esp_adc_cal.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"

#include "sdkconfig.h"
#include "logger.h"
#include "esp_err.h"
#include "pin_defs.h"

#define DEFAULT_VREF    1100        
#define NO_OF_SAMPLES   64 

esp_adc_cal_characteristics_t *adc_chars;

/**
 * @brief Check Vref or Two Point burned into efuse
 * 
 **/
void check_efuse(void);

/**
 * @brief Configure width and attenuation of ADC 1
 * 
 * @param channel[] Array of ADC 1 channels to which LSA is connected.
 * 
 * @return esp_err_t - returns ESP_OK if 11dB attenuation and 12 bit width configured to ADC 1, else it returns ESP_ERR_INVALID_ARG.  
 * 
 **/
esp_err_t config_adc1(adc1_channel_t channel[]);

/**
 * @brief Characterize ADC 1 using either Vref or Two Point
 * 
 * @return esp_err_t 
 **/
esp_err_t char_adc1();


#endif