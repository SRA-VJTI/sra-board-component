#ifndef MOTOR_DRIVER_H
#define MOTOR_DRIVER_H

#include "pin_defs.h"

static int PARALLEL_MODE ;

// https://github.com/espressif/esp-idf/tree/master/examples/peripherals/mcpwm/mcpwm_brushed_dc_control
// look this link for mcpwm example code

void set_motor_driver_mode(int mode);


//Initialise GPIOs for MCPWM
void mcpwm_gpio_initialize(int PARALLEL_MODE);

//Intialise MCPWM 
void mcpwm_initialize(int PARALLEL_MODE);

//Functions to control motor motion
void motor_forward(mcpwm_unit_t mcpwm_num, mcpwm_timer_t timer_num , float duty_cycle);

void motor_backward(mcpwm_unit_t mcpwm_num, mcpwm_timer_t timer_num , float duty_cycle);

void motor_stop(mcpwm_unit_t mcpwm_num, mcpwm_timer_t timer_num);

void read_motor_driver_mode() ;

#endif