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
 * @brief Sets motors in forward direction
 * 
 * @param mcpwm_num set MCPWM unit(0-1)
 * @param timer_num set timer number(0-2) of MCPWM, each MCPWM unit has 3 timers
 * @param duty_cycle set duty_cycle
 */
void motor_forward(mcpwm_unit_t mcpwm_num, mcpwm_timer_t timer_num , float duty_cycle);

/**
 * @brief Sets motors in reverse direction
 * 
 * @param mcpwm_num set MCPWM unit(0-1)
 * @param timer_num set timer number(0-2) of MCPWM, each MCPWM unit has 3 timers
 * @param duty_cycle set duty_cycle
 */
void motor_backward(mcpwm_unit_t mcpwm_num, mcpwm_timer_t timer_num , float duty_cycle);

/**
 * @brief Stops Motors
 * 
 * @param mcpwm_num set MCPWM unit(0-1)
 * @param timer_num set timer number(0-2) of MCPWM, each MCPWM unit has 3 timers
 */
void motor_stop(mcpwm_unit_t mcpwm_num, mcpwm_timer_t timer_num);

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