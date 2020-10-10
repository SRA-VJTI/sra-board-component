#ifndef MOTOR_DRIVER_H
#define MOTOR_DRIVER_H

#include "pin_defs.h"
#include "esp_log.h"
#include "esp_err.h"
#include <stdlib.h>
#include "freertos/FreeRTOS.h"

// https://github.com/espressif/esp-idf/tree/master/examples/peripherals/mcpwm/mcpwm_brushed_dc_control
// look this link for mcpwm example code

/**
 * @brief Enables Motor driver in Parallel or Normal Mode
 * 
 * @param MODE  if MODE = 1 (Parallel Mode) & MODE = 2 (Normal Mode)
 * @return esp_err_t returns ESP_OK if MCPWM initialised, else it returns ESP_ERR_INVALID_ARG 
 */
esp_err_t enable_motor_driver(int MODE);


//Functions to control motor motion
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
 * @brief Read motor driver mode
 * 
 * @return int returns mode of motor driver  1 = Parallel mode , 2 = Normal mode
 */
int read_motor_driver_mode() ;

#endif