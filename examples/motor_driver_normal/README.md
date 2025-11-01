# Motor Driver Example

## Table of Contents  
- [Overview](#overview)  
- [Logic followed](#logic-followed)  
- [Applications](#applications)
- [Building and Running](#building-and-running)

## Overview

This example demonstrates the basic usage of the SRA Board's motor driver functionality. 
It controls a motor connected to motor channel A1, cycling through forward, backward, and stop operations in a loop.

## Logic Followed

1. **Initialise Motor Driver** - reate a motor handle and enable the motor driver for a specific motor channel (MOTOR_A_1).

2. **Set Forward Motion** - Run the motor in the forward direction at 60% speed for 1 second.

3. **Set Backward Motion** - Run the motor in the backward direction at 60% speed for 1 second.

4. **Loop Execution** - Repeat the forward–backward–stop cycle indefinitely with log outputs at each step using ESP_LOGI().

## Applications

The application initializes the motor driver for motor A1 and continuously:
- Sets the motor to move forward at 60% speed for 1 second
- Sets the motor to move backward at 60% speed for 1 second
- Stops the motor for 1 second


## Building and Running

1. Set up the ESP-IDF environment:
   ```bash
   source $IDF_PATH/export.sh
   ```
2. Build the project:
   ```bash
   idf.py build
   ```
3. Flash to your ESP32 device and monitor output:
   ```bash
   idf.py flash moitor
   ```