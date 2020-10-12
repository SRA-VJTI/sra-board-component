#ifndef BAR_GRAPH_H
#define BAR_GRAPH_H

#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "driver/gpio.h"

#include "esp_log.h"
#include "esp_err.h"
#include "pin_defs.h"
#include "motor_driver.h"

esp_err_t enable_bar_graph();

esp_err_t set_bar_graph(uint8_t data);

#endif