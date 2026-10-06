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
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 */

#ifndef LED_MATRIX_FONT_H
#define LED_MATRIX_FONT_H

#include <stdint.h>

#include "esp_err.h"
#include "led_matrix.h"

#define LED_MATRIX_FONT_GLYPH_COUNT 128

/**
 * @brief 8x8 glyph bitmaps indexed by ASCII value.
 *
 * Printable ASCII (indices 32..126) uses a bold style; S, R, and A retain
 * their existing display-specific designs. Glyph rows run top to bottom,
 * with bit 7 as the leftmost pixel. Index 127 is blank.
 */
extern const uint8_t led_matrix_chars[LED_MATRIX_FONT_GLYPH_COUNT]
                                         [LED_MATRIX_HEIGHT];

/** Copy one glyph into the framebuffer; call led_matrix_show() to display it. */
esp_err_t led_matrix_font_draw_char(led_matrix_t *matrix,
                                        uint8_t character);

/** Draw and immediately display one glyph. */
esp_err_t led_matrix_font_display_char(led_matrix_t *matrix,
                                           uint8_t character);

/** Display a string one character at a time, waiting wait_ms after each glyph. */
esp_err_t led_matrix_font_display_string(led_matrix_t *matrix,
                                              const char *text,
                                              uint32_t wait_ms);

/** Slide a string from right to left one pixel column at a time. */
esp_err_t led_matrix_font_slide_text(led_matrix_t *matrix,
                                         const char *text,
                                         uint32_t step_delay_ms);

/**
 * Reveal each character row by row, pulsing at intensity 15 for 40 ms and
 * returning to the saved brightness for 70 ms after each row. Hold the
 * character, then wipe it column by column.
 * Wait character_delay_ms between characters and one second after the string.
 * On failure, attempt to restore brightness and return the original error.
 * Restoration failures are logged; the saved brightness is always preserved.
 * Serialize access to the matrix while this blocking animation runs.
 */
esp_err_t led_matrix_font_animate_text(led_matrix_t *matrix,
                                           const char *text,
                                           uint32_t character_delay_ms);

#endif /* LED_MATRIX_FONT_H */
