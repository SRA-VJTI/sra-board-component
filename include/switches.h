#ifndef SWITCHES_H
#define SWITCHES_H

#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_err.h"
#include "pin_defs.h"

/**
 * @brief Enables tactile switches on the sra board, sets up required gpio's in input pull-up mode.
 * 
 * @return esp_err_t - returns ESP_OK if gpio's initialised, else it returns ESP_ERR_INVALID_ARG 
 **/
esp_err_t enable_switches();

/**
 * @brief Reads the state of one of the four switches on sra board. 
 * 
 * @param switch_id id of the switch which needs to be read, we can pass the following macros: SWITCH_1, SWITCH_2, SWITCH_3, SWITCH_4. 
 * @return int - returns state of the switch, 1 if it is pressed and 0 if it is not pressed.
 **/
int read_switch(int switch_id);

#endif