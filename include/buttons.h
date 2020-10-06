#ifndef BUTTONS_H
#define BUTTONS_H

#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_err.h"
#include "pin_defs.h"

esp_err_t enable_buttons();
int read_button(int pin_number);

#endif