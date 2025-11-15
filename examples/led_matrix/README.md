# LED Matrix - SRA Board

## Table of Contents
- [Overview](#overview)
- [Shift Registers](#shift-registers)
- [Description of Functions](#description-of-functions)

---

## Overview

The **SRA Board** features a **5x6 LED matrix** — a grid of 30 LEDs arranged in 5 rows and 6 columns.  
This LED matrix allows users to light up individual LEDs, form patterns, and create dynamic visual effects through software control.

By interfacing the LED matrix with shift registers, the board can efficiently control all LEDs using only a few GPIO pins.  
This setup is ideal for embedded applications that require digital output visualization without using excessive hardware lines.

---

## Shift Registers

The LED matrix is driven by **serial-in, parallel-out shift registers** 
These components allow the ESP32 to send data serially (one bit at a time) and update multiple LEDs simultaneously.

The communication uses three main pins:
- **SDATA** → Serial Data Input  
- **SRCLK** → Shift Clock (moves data through the register)  
- **RCLK** → Latch Clock (transfers data to outputs)

### Working Principle
1. The ESP32 sends a stream of bits (each bit represents one LED state).  
2. Each clock pulse on `SRCLK` shifts the data into the register.  
3. Once all bits are loaded, a pulse on `RCLK` latches and displays the data across the LEDs.  
4. This process repeats whenever the LED pattern changes, enabling full control of the matrix with minimal wiring.

---

## Description of Functions

```led_matrix_set_data_raw(xMyLEDMatrix, pattern)```

Writes a **raw 32-bit bitmask** to the LED matrix buffer.  
Each bit represents the ON/OFF state of a single LED — `1` for ON and `0` for OFF.  
For example, using `UINT32_MAX` turns all LEDs ON, while `0` turns them all OFF.

---

```led_matrix_set_data(xMyLEDMatrix, data)```

Sets a specific LED based on its logical position by shifting a `1` into the correct bit position.  
This is typically used to light up LEDs one by one or to display specific patterns.

---

```led_matrix_write(xMyLEDMatrix, LED_MATRIX_OUTPUT_PAR)```

Sends the buffered LED data to the shift register outputs.  
This function handles the actual **data shifting and latching**, updating all LED states simultaneously.

---

```CONFIG_LED_MATRIX_ROWS * CONFIG_LED_MATRIX_COLUMNS```

Represents the **total number of LEDs** in the matrix.  
Used to loop through all LED indices when performing sequential operations.
Here it is 6x5 matrix.

---

```while (1) { ... }```

Runs the LED control logic in an infinite loop, ensuring the LED patterns repeat continuously.

---

#TODO: Add schematic diagram and pinouts
