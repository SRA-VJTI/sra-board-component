#include "motor_driver.h"

int MODE = 0 ;  //motor driver is disabled

void set_motor_driver_mode(int mode)
{
    MODE = mode ;  //set motor driver mode 

}


void mcpwm_gpio_initialize(int MODE)
{
    if(MODE == 1)  //Initialize motor driver in Parallel mode
    {
        mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM1A, MDA_PARALLEL_IN_1_2);
        mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM1B, MDA_PARALLEL_IN_3_4);
        mcpwm_gpio_init(MCPWM_UNIT_1, MCPWM0A, MDA_PARALLEL_IN_5_7);
        mcpwm_gpio_init(MCPWM_UNIT_1, MCPWM0B, MDA_PARALLEL_IN_6_8);
    }
    else if(MODE == 2)   //Initialize motor driver in Normal mode
    {
        mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM0A, MDA_NORMAL_IN_1);
        mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM0B, MDA_NORMAL_IN_2);
        mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM1A, MDA_NORMAL_IN_3);
        mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM1B, MDA_NORMAL_IN_4);
        mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM2A, MDA_NORMAL_IN_5);
        mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM2B, MDA_NORMAL_IN_6);
        mcpwm_gpio_init(MCPWM_UNIT_1, MCPWM0A, MDA_NORMAL_IN_7);
        mcpwm_gpio_init(MCPWM_UNIT_1, MCPWM0B, MDA_NORMAL_IN_8);
    }
}


//Intialise MCPWM 
 void mcpwm_initialize()
 {
    printf("Configuring Initial Parameters of mcpwm...\n");
    mcpwm_config_t pwm_config;
    pwm_config.frequency = 20000;    //frequency = 500Hz,
    pwm_config.cmpr_a = 0;    //duty cycle of PWMxA = 0
    pwm_config.cmpr_b = 0;    //duty cycle of PWMxb = 0
    pwm_config.counter_mode = MCPWM_UP_COUNTER;
    pwm_config.duty_mode = MCPWM_DUTY_MODE_0;
    printf("Configuring pwm_config...\n");
    if(MODE == 1)
    {
        mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_1, &pwm_config);    //Configure UNIT0 PWM1A & PWM1B with above settings
        mcpwm_init(MCPWM_UNIT_1, MCPWM_TIMER_0, &pwm_config);    //Configure UNIT1 PWM0A & PWM0B with above settings
    }
    mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_0, &pwm_config);    //Configure UNIT0 PWM0A & PWM0B with above settings
    mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_2, &pwm_config);    //Configure UNIT0 PWM2A & PWM2B with above settings
    printf("Initialize pwm_init...\n");
}


// Functions to control motors
void motor_forward(mcpwm_unit_t mcpwm_num, mcpwm_timer_t timer_num , float duty_cycle)
{
    mcpwm_set_duty(mcpwm_num, timer_num, MCPWM_OPR_A, 0);
    mcpwm_set_duty_type(mcpwm_num, timer_num, MCPWM_OPR_A, MCPWM_DUTY_MODE_0); 
    mcpwm_set_duty(mcpwm_num, timer_num, MCPWM_OPR_B, duty_cycle);
    mcpwm_set_duty_type(mcpwm_num, timer_num, MCPWM_OPR_B, MCPWM_DUTY_MODE_0); 
}
void motor_backward(mcpwm_unit_t mcpwm_num, mcpwm_timer_t timer_num , float duty_cycle)
{
    mcpwm_set_duty(mcpwm_num, timer_num, MCPWM_OPR_A, duty_cycle);
    mcpwm_set_duty_type(mcpwm_num, timer_num, MCPWM_OPR_A, MCPWM_DUTY_MODE_0); 
    mcpwm_set_duty(mcpwm_num, timer_num, MCPWM_OPR_B, 0);
    mcpwm_set_duty_type(mcpwm_num, timer_num, MCPWM_OPR_B, MCPWM_DUTY_MODE_0); 
}
void motor_stop(mcpwm_unit_t mcpwm_num, mcpwm_timer_t timer_num)
{
    mcpwm_set_duty(mcpwm_num, timer_num, MCPWM_OPR_A, 0);
    mcpwm_set_duty_type(mcpwm_num, timer_num, MCPWM_OPR_A, MCPWM_DUTY_MODE_0); 
    mcpwm_set_duty(mcpwm_num, timer_num, MCPWM_OPR_B, 0);
    mcpwm_set_duty_type(mcpwm_num, timer_num, MCPWM_OPR_B, MCPWM_DUTY_MODE_0); 
}

// Read motor driver mode
void read_motor_driver_mode() 
{
        switch (MODE)
        {
        case 0: printf("Motor driver is DISABLED");
            break;
        case 1: printf("Motor driver is enabled in PARALLEL Mode");
            break;
        case 2: printf("Motor driver is enabled in NORMAL Mode");
            break;
        default: printf("wrong mode is selected")
            break;
        }
} 