#ifndef BUTTONS_H
#define BUTTONS_H

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
esp_err_t enable_buttons();

/**
 * @brief Reads the state of one of the four buttons on sra board. 
 * 
 * @param button_id id of the button which needs to be read, we can pass the following macros: BUTTON_1, BUTTON_2, BUTTON_3, BUTTON_4. 
 * @return int - returns state of the button, 1 if it is pressed and 0 if it is not pressed.
 **/
int read_button(int button_id);

#endif