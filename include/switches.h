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
