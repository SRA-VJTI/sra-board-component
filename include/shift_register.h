#ifndef SHIFT_REGISTER_H
#define SHIFT_REGISTER_H

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "pin_defs.h"
#include "utils.h"

#include "driver/gpio.h"
#include "soc/gpio_num.h"
#include "hal/gpio_types.h"
#include "esp_err.h"
#include "esp_check.h"
#include "esp_heap_caps.h"
#include "esp_rom_sys.h"

typedef union {
    struct {
        gpio_num_t ds;
        gpio_num_t shcp;
        gpio_num_t stcp;
    };

    struct {
        gpio_num_t sdata;
        gpio_num_t srclk;
        gpio_num_t rclk;
    };
} shift_register_t;

esp_err_t shift_register_gpio_init(shift_register_t *conf);
esp_err_t shift_register_write_byte(const shift_register_t *sreg, const uint8_t data, const bool instant);
esp_err_t shift_register_write_int(const shift_register_t *sreg, const uint32_t data, const bool instant);

#endif