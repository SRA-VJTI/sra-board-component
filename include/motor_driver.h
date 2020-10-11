#ifndef MOTOR_DRIVER_H
#define MOTOR_DRIVER_H

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "driver/mcpwm.h"
#include "soc/mcpwm_periph.h"
#include "esp_attr.h"

#include "esp_log.h"
#include "esp_err.h"
#include "pin_defs.h"

#define PARALLEL_MODE 1
#define NORMAL_MODE 2

#define MOTOR_FORWARD 200
#define MOTOR_BACKWARD 201
#define MOTOR_STOP 202

/**
 * @brief Enables Motor driver in Parallel or Normal Mode
 * 
 * @param id for id=a, enables motor driver a and for id=b, enables motor driver b
 * @param mode if mode = 1 is passed, motor driver is operated in parallel mode, if mode = 2 is passed, motor driver is operated in normal mode
 */
#define enable_motor_driver(id, mode) enable_motor_driver_##id(mode) 

/**
 * @brief Reads mode of motor driver 
 * 
 * @param id for id=a, returns mode of motor driver a, for id=b, returns mode of motor driver b
 * @return int returns mode of motor driver B, 1 = Parallel mode, 2 = Normal mode
 */
#define read_motor_driver_mode(id) read_motor_driver_mode_##id()

/**
 * @brief Enables Motor driver A in Parallel or Normal Mode
 * 
 * @param mode if mode = 1 is passed, motor driver is operated in parallel mode, if mode = 2 is passed, motor driver is operated in normal mode
 * @return esp_err_t returns ESP_OK if motor driver initialised properly, else it returns ESP_ERR_INVALID_ARG 
 */
esp_err_t enable_motor_driver_a(int mode);

/**
 * @brief Enables Motor driver B in Parallel or Normal Mode
 * 
 * @param mode if mode = 1 is passed, motor driver is operated in parallel mode, if mode = 2 is passed, motor driver is operated in normal mode
 * @return esp_err_t returns ESP_OK if motor driver initialised properly, else it returns ESP_ERR_INVALID_ARG 
 */
esp_err_t enable_motor_driver_b(int mode);

/**
 * @brief Set the speed of motors
 * 
 * @param motor_id set it as MOTOR_A_0, MOTOR_A_1, MOTOR_B_0, MOTOR_B_1 to select the appropriate motor to set its speed and direction
 * @param direction set is as MOTOR_FORWARD for forward motion, MOTOR_BACKWARD for backward motion, MOTOR_STOP to stop the motor
 * @param duty_cycle set the duty cycle of the motor driver PWM
 * @return esp_err_t returns ESP_OK if speed correctly, ESP_FAIL if any error occurs
 */
esp_err_t set_motor_speed(int motor_id, int direction, float duty_cycle);

/**
 * @brief Reads mode of motor driver A
 * 
 * @return int returns mode of motor driver A, 1 = Parallel mode, 2 = Normal mode
 */
int read_motor_driver_mode_a();

/**
 * @brief Reads mode of motor driver B
 * 
 * @return int returns mode of motor driver B, 1 = Parallel mode, 2 = Normal mode
 */
int read_motor_driver_mode_b();

#endif