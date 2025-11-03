# Motor Driver Example

## Table of Contents  
- [Overview](#overview)  
- [Hardware Setup](#hardware-setup)  
- [Logic Followed](#logic-followed)  
- [Working Principle](#working-principle)  
- [Software Usage](#software-usage)  
- [Running](#running)  
- [Applications](#applications)

## Overview

This example demonstrates the basic usage of the SRA Board's motor driver functionality. 
It controls a motor connected to motor channel A1, cycling through forward, backward, and stop operations in a loop.

---

## Hardware Setup  
- Connect a DC motor to the motor channel A1 on the SRA Board.  
- Ensure the motor is properly powered and the connections for VCC, GND, and control signals are secure.

---

## Logic Followed

1. **Initialise Motor Driver** - Create a motor handle and enable the motor driver for a specific motor channel (MOTOR_A_1).

2. **Set Forward Motion** - Run the motor in the forward direction at 60% speed for 1 second.

3. **Set Backward Motion** - Run the motor in the backward direction at 60% speed for 1 second.

4. **Loop Execution** - Repeat the forward–backward–stop cycle indefinitely with log outputs at each step using ESP_LOGI().

---

## Working Principle  
The motor driver controls DC motors using PWM (Pulse Width Modulation) signals to regulate speed and direction. PWM modulates the duty cycle to control the average voltage applied to the motor, thus regulating speed. Direction is achieved via an H-bridge configuration that reverses the polarity of the voltage across the motor terminals.

---

## Software Usage  

The SRA Board component provides built-in functions for motor control, found in the `include/` directory under `motor_driver.h`.

### Function Descriptions  

#### `esp_err_t enable_motor_driver(motor_handle_t *motor_handle, motor_channel_t channel)`  
**Description:** Initializes and enables the motor driver for the specified channel.  
**Parameters:**  
- `motor_handle`: Pointer to the motor handle  
- `channel`: Motor channel (e.g., MOTOR_A_1)  
**Returns:** `ESP_OK` on success, error code otherwise.

---

#### `esp_err_t set_motor_speed(motor_handle_t motor_handle, motor_direction_t direction, uint8_t speed)`  
**Description:** Sets the motor speed and direction.  
**Parameters:**  
- `motor_handle`: Motor handle  
- `direction`: MOTOR_FORWARD, MOTOR_BACKWARD, or MOTOR_STOP  
- `speed`: Speed percentage (0-100)  
**Returns:** `ESP_OK` on success.

---

#### `void vTaskDelay(const TickType_t xTicksToDelay)`  
**Description:** Delays a FreeRTOS task for a specified number of ticks.  
**Parameter:**  
- `xTicksToDelay`: Time in ticks to block the calling task.

---

## Running

1. Navigate to the example directory:  
   ```bash
   cd examples/motor_driver_normal
   ```
2. Set up the ESP-IDF environment:
   ```bash
   source $IDF_PATH/export.sh
   ```
3. Build the project:
   ```bash
   idf.py build
   ```
4. Flash to your ESP32 device and monitor output:
   ```bash
   idf.py flash monitor
   ```

---

## Applications

Motor drivers act as an interface between the motors and the control circuits. Motors require high amounts of current whereas the controller circuit works on low current signals. So the function of motor drivers is to take a low-current control signal and then turn it into a higher-current signal that can drive a motor.
Common applications of controlled motors include:
- Wheeled robots for locomotion
- Robotic arms and grippers for manipulation
- Conveyor systems in manufacturing
- Drones and unmanned aerial vehicles
- Home automation devices like smart locks or curtains
