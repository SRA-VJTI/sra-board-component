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
#ifndef BAR_GRAPH_H
#define BAR_GRAPH_H

#include <stdlib.h>
#include <stdbool.h>
#include "freertos/FreeRTOS.h"
#include "driver/gpio.h"

#include "esp_log.h"
#include "esp_err.h"
#include "pin_defs.h"
#include "motor_driver.h"

/**
 * It will check for state of motor driver A and B, and accordingly init the free gpios.
 * __________________________________________
 * | Motor Driver A | Motor Driver B | Mode |
 * |----------------|----------------|------|
 * |     off        |      off       |   1  |
 * |     off        |      parallel  |   2  |
 * |     off        |      normal    |   3  |
 * |     parallel   |      off       |   4  |
 * |     parallel   |      parallel  |   5  |
 * |     parallel   |      normal    |   6  |
 * |     normal     |      off       |   7  |
 * |     normal     |      parallel  |   8  |
 * |     normal     |      normal    |   0  |
 * 
 * enabled_bar_graph_flag is set to the value of Mode accordingly.
 * 
 * @brief enables the bar graph led's gpio pins
 * 
 * @return esp_err_t returns ESP_OK if enabled correctly, and ESP_FAIL if it failed to initialise
 */
esp_err_t enable_bar_graph();

/**
 * The exact working of this is a bit hard to understand, so this is example. Below are the given states of motor drivers
 * 
 * Motor Driver A: Normal
 * Motor Driver B: Off
 * Mode = 7, so for mode = 7,
 * mask = bitmask[7] = 0x0F = 00001111
 * data = 0xAA = 10101010
 * 
 * Since, motor driver A is in normal mode and motor driver B is off, we can only use IN5 - IN8 pins of the bar graph LED
 * 
 * ```
 * LED1 <-----> Motor Driver A (IN1) ----- 0    
 * LED2 <-----> Motor Driver A (IN2) ----- 0    
 * LED3 <-----> Motor Driver A (IN3) ----- 0    
 * LED4 <-----> Motor Driver A (IN4) ----- 0    
 * LED5 <--X--> Motor Driver B (IN5) ----- 1    
 * LED6 <--X--> Motor Driver B (IN6) ----- 1    
 * LED7 <--X--> Motor Driver B (IN7) ----- 1    
 * LED8 <--X--> Motor Driver B (IN8) ----- 1    
 * ```
 *   
 * Mask ==> 00001111 => 0x0F
 * 
 * Now, if data is 10101010, then even though pins IN1-IN4 are being used by motor driver, it will be set by gpio_set_level, which will
 * crash the esp32 device. So, we check in the bitmask if the following pin is free or not, if bit is 1, it can be used, if it is 0, then
 * it can't be used.
 * 
 * So, to achieve this we bitwise-and the mask with 0x80(10000000), if result is 0x80, then leftmost bit is 1, else it is 0.
 * If leftmost bit is 1, means we can use this pin of bar graph, then we do the same thing with data, bitwise-and it with 0x80,
 * to find the leftmost bit, if it is 1, the pin is set to 1, else 0. Now we left shift the data and mask variable.
 * 
 * @brief Set the value of bar graph LED
 * 
 * @param data 8bit binary in hexadecimal form, i.e, the pattern that is needed to be shown on the bar graph LED
 * @return esp_err_t returns ESP_OK
 */
esp_err_t set_bar_graph(uint8_t data);

/**
 * @brief converts a boolean array of size 8 into 8-bit unsigned integer
 * 
 * @param input_array boolean input array of size 8
 * @return uint8_t returns a 8 bit number formed after combining the boolean array
 */
uint8_t bool_to_uint8(bool *input_array);

#endif