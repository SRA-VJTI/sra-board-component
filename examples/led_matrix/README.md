# LED Matrix - SRA Board

## Table of Contents
- [Overview](#overview)
- [Hardware](#hardware)
- [API](#api)
- [Example](#example)

---

## Overview

The SRA Board has a built-in 8x8 LED matrix for displaying characters, icons,
and pixel patterns. This example is for debugging the display: it blinks all
64 LEDs together, then displays random pixel patterns. This helps check for
pixels that stay off, pixels that stay on, and unexpected row or column behavior.
The board component also provides a font API for printable ASCII characters.

## Hardware

The LED matrix is connected to fixed PCB pins, so applications do not provide
GPIO numbers when initializing it:

- **CLK**: GPIO 18
- **DIN**: GPIO 16
- **LOAD**: GPIO 17

The board's **MAX7219** driver chip sits between the ESP32 and the 8x8 LEDs.
The ESP32 sends pixel and brightness settings to the chip over SPI using CLK,
DIN, and LOAD. The MAX7219 stores the pixel rows in its display memory and
continuously scans the matrix, switching rows on and off fast enough that the
image appears steady to us. This lets the chip refresh the LEDs on its own;
the application updates the image through the driver API instead of scanning
the rows itself. See the [MAX7219 datasheet](https://www.analog.com/media/en/technical-documentation/data-sheets/MAX7219-MAX7221.pdf).

## API

Include `sra_board.h` to use the LED matrix API.

### Display control

- `led_matrix_init(&matrix)` initializes the display with the SRA Board
  connections and brightness 4.
- `led_matrix_set_brightness(&matrix, value)` sets brightness from 0 to 15
  and remembers the level on success.
- `led_matrix_set_pixel(&matrix, x, y, on)` changes one buffered pixel.
- `led_matrix_set_row(&matrix, row, pixels)` changes one buffered row.
- `led_matrix_draw_bitmap(&matrix, rows)` copies an 8-row bitmap into the
  framebuffer.
- `led_matrix_show(&matrix)` sends the framebuffer to the display.
- `led_matrix_clear(&matrix)` clears the framebuffer and display.
- `led_matrix_set_enabled(&matrix, enabled)` turns display scanning on or
  off.
- `led_matrix_deinit(&matrix)` releases the display resources.

### Fonts and text

- `led_matrix_font_display_char(&matrix, 'A')` displays one character.
- `led_matrix_font_draw_char(&matrix, 'A')` draws a character into the
  framebuffer without sending it yet.
- `led_matrix_font_display_string(&matrix, "SRA", wait_ms)` displays the
  characters in order, waiting `wait_ms` after each one.
- `led_matrix_font_slide_text(&matrix, "SRA", step_delay_ms)` scrolls text
  from right to left.
- `led_matrix_font_animate_text(&matrix, "SRA", character_delay_ms)`
  reveals, holds, and wipes each character. Each revealed row pulses at
  brightness 15 for 40 ms, then returns to the saved level for 70 ms.
  The third argument sets the pause between characters; the animation pauses
  for one second after the whole string. On an error, it attempts to restore
  brightness before returning the original error. If restoration also fails,
  it logs that failure and keeps the saved setting for recovery. Serialize
  access to the matrix while the animation runs.

## Example

The [example code](main/main.c) initializes the display at brightness level 4,
then repeats this sequence:

1. Blink all LEDs three times, with 1000 ms on and 500 ms off.
2. Show 24 random bitmaps, holding each for 150 ms. Each bit controls one LED.
3. Clear the display and pause for 800 ms before repeating.

The random patterns use `esp_random()`. This debugging example uses the bitmap
API directly; it does not display text.

Build and flash this example from its directory with ESP-IDF:

```sh
idf.py build
idf.py -p <PORT> flash monitor
```
