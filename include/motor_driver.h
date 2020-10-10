#ifndef MOTOR_DRIVER_H
#define MOTOR_DRIVER_H

#include "pin_defs.h"

// https://github.com/espressif/esp-idf/tree/master/examples/peripherals/mcpwm/mcpwm_brushed_dc_control
// look this link for mcpwm example code

// Set motor driver mode:
// 1 - Parallel mode
// 2 - Normal mode
void set_motor_driver_mode(int mode);

//Initialise GPIOs for MCPWM
void mcpwm_gpio_initialize();

//Intialise MCPWM 
void mcpwm_initialize();

//Functions to control motor motion
void motor_forward(mcpwm_unit_t mcpwm_num, mcpwm_timer_t timer_num , float duty_cycle);

void motor_backward(mcpwm_unit_t mcpwm_num, mcpwm_timer_t timer_num , float duty_cycle);

void motor_stop(mcpwm_unit_t mcpwm_num, mcpwm_timer_t timer_num);

// Read motor driver mode
void read_motor_driver_mode() ;

#endif