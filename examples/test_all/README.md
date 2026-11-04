# Test All - SRA Board

## Table of Contents
- [Overview](#overview)
- [What to expect](#what-to-expect)
- [Hardware](#hardware)
- [How it works](#how-it-works)
- [Build and flash](#build-and-flash)
- [Troubleshooting](#troubleshooting)

---

## Overview

This example runs the main peripherals of the SRA Board at the same time so a
board can be checked in one flash instead of one example per component. It
exercises:

- **OLED** - shows the live LSA readings as bars
- **LSA** (light sensing array) - all five sensors are read continuously
- **Motor driver and motors** - both normal mode motors run forward and backward, one after the other
- **Servos** - all four servo ports sweep from 0 to 90 degrees and back
- **LED matrix** - all 64 LEDs blink together three times, then repeat
- **MPU6050** - the accelerometer readings are logged on the serial monitor

Each component runs in its own FreeRTOS task, so a fault in one (for example
an MPU that does not respond) does not stop the others from being tested.

## What to expect

After flashing, the OLED shows the SRA and Wall-E logos and then:

| Component | Working correctly |
|-----------|-------------------|
| OLED + LSA | Five bars, one per LSA sensor. Moving a white or black surface under a sensor changes the height of its bar. The leftmost bar is `LSA_4` and the rightmost is `LSA_0`. |
| Motor driver + motors | `MOTOR_A_0` runs forward for 2 s, stops for 1 s, runs backward for 2 s and stops for 1 s. `MOTOR_A_1` then does the same, and the cycle repeats. |
| Servos | All four servos sweep slowly from 0 to 90 degrees, back to 0, pause for 1 s, and repeat. |
| LED matrix | All 64 LEDs turn on for 0.5 s and off for 0.5 s, three times, then pause for 2 s and repeat. An LED that never lights, or never turns off, is faulty. |
| MPU | The serial monitor prints `Accel (g) X: ... Y: ... Z: ...` twice a second. Lying flat, Z reads about 1.00 and X and Y about 0.00; tilting the board moves the 1 g towards X or Y. |

The serial monitor also logs the LSA readings, each motor step, each servo
sweep and each round of LED blinks, which helps tell a display fault apart
from a sensor fault.

> **Note:** Keep the wheels off the ground (or the motors free to spin) and
> make sure nothing blocks the servo horns while running this example, since
> the motors and servos start moving as soon as it boots.

## Hardware

All connections are fixed on the SRA Board, so no wiring is needed apart from
plugging in the motors, servos, LSA, OLED and MPU modules.

| Component | Pins |
|-----------|------|
| OLED (SSD1306, I2C) | SDA: GPIO 14, SCL: GPIO 21 |
| MPU6050 (I2C) | SDA: GPIO 14, SCL: GPIO 21 |
| LSA | GPIO 4, 5, 6, 7, 8 |
| Motor `MOTOR_A_0` | IN1: GPIO 13, IN2: GPIO 12 |
| Motor `MOTOR_A_1` | IN3: GPIO 11, IN4: GPIO 10 |
| Servos A, B, C, D | GPIO 1, 2, 42, 41 |
| LED matrix (MAX7219) | CLK: GPIO 18, DIN: GPIO 16, LOAD: GPIO 17 |

The OLED and the MPU share the same I2C bus. The board component guards the
bus with a mutex, so both can be used from different tasks.

The motors and the servos are both driven by MCPWM group 0, which has three
timers. The servos use two of them and each motor needs one, so the motors
are tested one at a time: each motor is released after its test, which frees
its timer for the other motor.

## How it works

`app_main` checks that the OLED answers on the I2C bus and, if it does,
initializes it with `init_oled()`. It then starts five tasks:

1. **`lsa_oled_task`** reads the LSA every 50 ms, maps each reading from the
   raw ADC range (0 - 4095) to 0 - 1000, and draws it with `display_lsa()`.
   All OLED calls are kept in this task because LVGL is not thread safe.
2. **`motor_task`** enables one motor in normal mode, cycles it through
   forward, stop, backward and stop with `set_motor_speed()` at 60 % duty
   cycle, then disables and deletes it and moves on to the other motor.
3. **`servo_task`** enables the four servo ports with `enable_servo()` and
   sweeps them together from 0 to 90 degrees and back with
   `set_angle_servo()`, one degree every 50 ms. The pulse widths come from
   the servo settings in `idf.py menuconfig`, as in the
   [servos example](../servos).
4. **`led_matrix_task`** initializes the LED matrix and turns every LED on
   with `led_matrix_draw_bitmap()` and `led_matrix_show()`, then off with
   `led_matrix_clear()`, three times in a row before pausing.
5. **`mpu_task`** waits until the MPU6050 answers on the I2C bus (checking
   every second), wakes it with `enable_mpu6050()`, then reads the
   accelerometer with `mpu6050_read_acce()` and logs each axis in g every
   500 ms.

The OLED and MPU checks run before the board component's init functions, so a
missing module is reported on the serial monitor instead of stopping the
program. This example does not change the board component itself.

The motor speed, servo sweep range, blink count and all timings are `#define`s
at the top of [main.c](main/main.c) and can be changed as needed.

## Build and flash

`sdkconfig.defaults` sets the target to the ESP32-S3 used on the SRA Board
(`CONFIG_IDF_TARGET="esp32s3"`), so `idf.py set-target` is not needed. It also
enables the OLED (`CONFIG_ENABLE_OLED=y`); if you disable it in
`idf.py menuconfig`, the LSA readings are still logged on the serial monitor.

Build and flash this example from its directory with ESP-IDF:

```sh
idf.py build
idf.py -p <PORT> flash monitor
```

These defaults only apply when there is no `sdkconfig` yet. If you built this
example for another target before, delete `sdkconfig` and the `build` folder
first (or run `idf.py fullclean` and delete `sdkconfig`).

To keep the build fast, [CMakeLists.txt](CMakeLists.txt) sets
`COMPONENTS main`, so only the ESP-IDF components this example needs are
built instead of all of them (Wi-Fi, Bluetooth, HTTP and so on).

## Troubleshooting

- **`MPU did not respond at I2C address 0x68, retrying...` on the serial
  monitor** - the MPU6050 did not answer on the I2C bus. The example checks
  again every second; check that the MPU is seated properly.
- **Some LEDs on the matrix never light** - check the LED matrix module and
  its connection; the same LEDs failing in the [led_matrix example](../led_matrix)
  confirms a hardware fault.
- **`OLED did not respond at I2C address 0x3C` on the serial monitor** - the
  OLED did not answer on the I2C bus, so it is not initialized. The other
  tests keep running and the LSA
  readings are still logged. Check that the OLED is plugged in the right way
  round (VCC, GND, SCL and SDA in the order printed on the board) and that the
  module uses address 0x3C. If the MPU also fails to initialize, the problem
  is on the shared I2C bus (GPIO 14 and 21) rather than the OLED itself.
- **OLED stays blank without an error** - make sure `CONFIG_ENABLE_OLED` is
  enabled in `idf.py menuconfig`.
- **A bar on the OLED never changes** - that LSA sensor or its connection is
  faulty; compare with the readings on the serial monitor.
- **A motor does not move, or moves only one way** - check the motor
  connector and the motor driver for that channel.
- **A servo does not move** - check that it is plugged into the right port
  with the signal wire the right way round, and that the servo supply is on.
  If it moves but not over the full range, adjust its pulse widths in
  `idf.py menuconfig` under *SRA Board Component Config*.
