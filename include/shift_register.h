/*
 * MIT License
 *
 * Copyright (c)  2025 Society of Robotics and Automation
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

#ifndef SHIFT_REGISTER_H
#define SHIFT_REGISTER_H

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "pin_defs.h"
#include "utils.h"

#include "driver/gpio.h"
#include "hal/gpio_types.h"
#include "esp_err.h"
#include "esp_check.h"
#include "esp_rom_sys.h"

/**
 * @brief The handle used for representing the shift register
 **/
typedef struct {
    gpio_num_t sdata;
    gpio_num_t srclk;
    gpio_num_t rclk;
} shift_register_t;

/**
 * @brief Initialises the GPIO Pins for the Shift Register Present on the SRA Board
 * @param conf: The handle to use and represent the shift registers.
 * @return Returns an error if there is an error during configuring the GPIOs or if memory for the handle cannot be allocated, otherwise ESP_OK
 **/
esp_err_t shift_register_gpio_init(shift_register_t *conf);

/**
 * @brief Writes a single byte to the shift register and latches it (single shift register)
 * @param sreg: The handle to use and represent the shift registers.
 * @param data: The data to be sent to the shift register.
 * @param instant: This parameter defines the way in which the data is latched to the output register, all 8 bits at once or one-by-one.
 **/
esp_err_t shift_register_write_byte(const shift_register_t *sreg, const uint8_t data, const bool instant);

/**
 * @brief Writes a four bytes to the shift register and latches it (four cascaded shift registers)
 * @param sreg: The handle to use and represent the shift registers.
 * @param data: The data to be sent to the shift register.
 * @param instant: This parameter defines the way in which the data is latched to the output register, all 8 bits at once or one-by-one.
 **/
esp_err_t shift_register_write_int(const shift_register_t *sreg, const uint32_t data, const bool instant);

#endif
