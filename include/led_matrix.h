/*
 * MIT License
 *
 * Copyright (c) 2026 Society of Robotics and Automation
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


#ifndef LED_MATRIX_H
#define LED_MATRIX_H

#include <stdbool.h>
#include <stdint.h>

#include "esp_err.h"
#include "driver/spi_master.h"

#define LED_MATRIX_WIDTH  8
#define LED_MATRIX_HEIGHT 8

/**
 * @brief Handle and framebuffer for one MAX7219-driven 8x8 LED matrix.
 *
 * The framebuffer uses logical bitmap orientation: rows are ordered top to
 * bottom, and in each row bit 7 is the leftmost pixel while bit 0 is the
 * rightmost. On the SRA board, MAX7219 digit registers 0..7 address columns
 * from left to right, with bit 7 at the top and bit 0 at the bottom.
 * led_matrix_show() transposes the framebuffer rows into these columns,
 * so callers should not transpose bitmaps before drawing them.
 */
typedef struct {
    uint8_t framebuffer[LED_MATRIX_HEIGHT];
    uint8_t configured_brightness;
    spi_device_handle_t spi_device;
    bool initialized;
} led_matrix_t;

/** Initialize using the SRA board MAX7219 pins and default brightness 4. */
esp_err_t led_matrix_init(led_matrix_t *matrix);

/** Set one buffered pixel; x and y are in the range 0..7. Call show to update. */
esp_err_t led_matrix_set_pixel(led_matrix_t *matrix,
                                  uint8_t x, uint8_t y, bool on);

/** Set one buffered logical row. Bit 7 is the leftmost pixel; call show to update. */
esp_err_t led_matrix_set_row(led_matrix_t *matrix,
                                 uint8_t row, uint8_t pixels);

/** Copy a top-to-bottom logical bitmap to the framebuffer. Call show to update. */
esp_err_t led_matrix_draw_bitmap(led_matrix_t *matrix,
                                     const uint8_t rows[LED_MATRIX_HEIGHT]);

/** Map the logical framebuffer and update the display. */
esp_err_t led_matrix_show(const led_matrix_t *matrix);

/** Clear the framebuffer and immediately blank the physical display. */
esp_err_t led_matrix_clear(led_matrix_t *matrix);

/** Set intensity from 0 (dim) through 15 (bright). */
esp_err_t led_matrix_set_brightness(led_matrix_t *matrix,
                                        uint8_t brightness);

/** Enable or shut down display scanning while retaining framebuffer contents. */
esp_err_t led_matrix_set_enabled(const led_matrix_t *matrix,
                                     bool enabled);

/** Release the SPI device and pins from driver use. */
esp_err_t led_matrix_deinit(led_matrix_t *matrix);

#endif /* LED_MATRIX_H */
