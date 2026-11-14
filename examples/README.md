# SRA Board examples

Example Codes for SRA Board

----------------------------    

**ESP-IDF Component**: https://github.com/SRA-VJTI/sra-board-component  
**SRA Board PCB Design**: https://github.com/SRA-VJTI/sra-board-hardware-design

---------------------------     
## Examples

### LED Matrix

The SRA Board's 8x8 LED matrix can display characters, bitmaps, and pixel
patterns through the board component API.

- [Example](https://github.com/SRA-VJTI/sra-board-component/tree/main/examples/led_matrix) for initializing the display and using its drawing and font APIs

### Light sensing array

LSA aka Light Sensing Array is used mainly in line following bot

- [Example](https://github.com/SRA-VJTI/sra-board-component/tree/main/examples/lsa) for using four LED Light sensing array

### Motor driver 

Example for using two motor drivers present on SRA Board in both parallel and normal mode

- [normal](https://github.com/SRA-VJTI/sra-board-component/tree/main/examples/motor_driver_normal)

- [parallel](https://github.com/SRA-VJTI/sra-board-component/tree/main/examples/motor_driver_parallel)


### MPU6050

- MPU6050 combines 3-axis Gyroscope, 3-axis Accelerometer and Digital Motion Processor all in small package.

- [Example](https://github.com/SRA-VJTI/sra-board-component/tree/main/examples/mpu6050) for reading values from mpu6050 accelerometer

### Servos

- Servos are used in the locomotion of bots like [Wall-E](https://github.com/SRA-VJTI/Wall-E_v2.1)

- [Example](https://github.com/SRA-VJTI/sra-board-component/tree/main/examples/servos) for using the servo port on SRA Board to run servos

### Test All

- Runs the OLED, LSA, motor driver, servos, MPU6050 and LED matrix together to check a whole board in one flash

- [Example](https://github.com/SRA-VJTI/sra-board-component/tree/main/examples/test_all) showing LSA readings on the OLED, running the motors in both directions, sweeping the servos, blinking every LED of the matrix and logging the MPU6050 accelerometer

### Switches

- Manual Switches provide the traditional comfort to toggle the input variables

- [Example](https://github.com/SRA-VJTI/sra-board-component/tree/main/examples/switches) for using four user switches present on SRA Board

## Build and flash

Build and flash an example from its directory with ESP-IDF:

```sh
idf.py build
idf.py -p <PORT> flash monitor
```

The [Test All](test_all) example has a `sdkconfig.defaults` that sets the
target to the ESP32-S3 used on the SRA Board (`CONFIG_IDF_TARGET="esp32s3"`),
so `idf.py set-target` is not needed for it. It also enables the OLED
(`CONFIG_ENABLE_OLED=y`); if you disable it in `idf.py menuconfig`, the LSA
readings are still logged on the serial monitor.

These defaults only apply when there is no `sdkconfig` yet. If you built the
example for another target before, delete `sdkconfig` and the `build` folder
first (or run `idf.py fullclean` and delete `sdkconfig`).

To keep the build fast, its [CMakeLists.txt](test_all/CMakeLists.txt) sets
`COMPONENTS main`, so only the ESP-IDF components the example needs are
built instead of all of them (Wi-Fi, Bluetooth, HTTP and so on).
