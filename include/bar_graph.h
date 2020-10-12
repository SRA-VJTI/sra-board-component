#ifndef BAR_GRAPH_H
#define BAR_GRAPH_H

#include <stdlib.h>
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
 * |----------------|----------------|------|
 * 
 * @brief enables the bar graph led's gpio pins
 * 
 * @return esp_err_t returns ESP_OK if enabled correctly, and ESP_FAIL if it failed to initialise
 */
esp_err_t enable_bar_graph();

/**
 * @brief Set the value of bar graph LED
 * 
 * @param data 8bit binary in hexadecimal form, i.e, the pattern that is needed to be shown on the bar graph LED
 * @return esp_err_t returns ESP_OK
 */
esp_err_t set_bar_graph(uint8_t data);

#endif