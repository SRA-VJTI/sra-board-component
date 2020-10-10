#include "motor_driver.h"

static const char* TAG_MOTOR_DRIVER = "motor_driver";
static int MODE = 0; //motor driver is disabled by default

void set_motor_driver_mode(int mode)
{
    if (mode == 1)
    {
        ESP_LOGI(TAG_MOTOR_DRIVER, "enabling in PARALLEL mode");
        MODE = mode;
    }
    else if (mode == 2)
    {
        ESP_LOGI(TAG_MOTOR_DRIVER, "enabling in NORMAL mode");
        MODE = mode;
    }
    else if (mode == 0)
    {
        ESP_LOGE(TAG_MOTOR_DRIVER, "motor driver is disabled");
        MODE = mode;
    } 
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

 void mcpwm_initialize()
 {
    ESP_LOGI(TAG_MOTOR_DRIVER, "Configuring Initial Parameters of mcpwm...\n");
    mcpwm_config_t pwm_config;
    pwm_config.frequency = 20000;    //frequency = 500Hz,
    pwm_config.cmpr_a = 0;    //duty cycle of PWMxA = 0
    pwm_config.cmpr_b = 0;    //duty cycle of PWMxb = 0
    pwm_config.counter_mode = MCPWM_UP_COUNTER;
    pwm_config.duty_mode = MCPWM_DUTY_MODE_0;
    ESP_LOGI(TAG_MOTOR_DRIVER, "Configuring pwm_config...\n");
    if(MODE == 1)
    {
        err0_1 = mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_1, &pwm_config);    
        if (err0_1 == ESP_OK)
        {
            ESP_LOGI(TAG_MOTOR_DRIVER, "Configured UNIT0 PWM1A & PWM1B with above settings");
        }
        else 
        {
            ESP_LOGE(TAG_MOTOR_DRIVER, "Failed to configure UNIT0 PWM1A & PWM1B with above settings");
            MODE = -1 
        }
        err1_0 = mcpwm_init(MCPWM_UNIT_1, MCPWM_TIMER_0, &pwm_config);    
        if (err1_0 == ESP_OK)
        {
            ESP_LOGI(TAG_MOTOR_DRIVER, "Configured UNIT1 PWM0A & PWM0B with above settings");
        }
        else 
        {
            ESP_LOGE(TAG_MOTOR_DRIVER, "Failed to configure UNIT1 PWM0A & PWM0B with above settings");
            MODE = -1 
        }         
    }
    err0_0 = mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_0, &pwm_config);    
    if (err0_0 == ESP_OK)
    {
        ESP_LOGI(TAG_MOTOR_DRIVER, "Configured UNIT0 PWM0A & PWM0B with above settings");
    }
    else 
    {
        ESP_LOGE(TAG_MOTOR_DRIVER, "Failed to configure UNIT0 PWM0A & PWM0B with above settings");
        MODE = -1 
    }
    err0_2 = mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_2, &pwm_config);  
    if (err0_2 == ESP_OK)
    {
        ESP_LOGI(TAG_MOTOR_DRIVER, "Configured UNIT0 PWM2A & PWM2B with above settings");
    }
    else 
    {
        ESP_LOGE(TAG_MOTOR_DRIVER, "Failed to configure UNIT0 PWM2A & PWM2B with above settings");
        MODE = -1;
    }
    if (MODE != -1)
    {
        ESP_LOGI(TAG_MOTOR_DRIVER, "Initialize pwm_init...\n");
    }
}

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

void read_motor_driver_mode() 
{
        switch (MODE)
        {
        case 0: ESP_LOGE(TAG_MOTOR_DRIVER, "motor driver is disabled, call set_motor_driver_mode() before reading motor driver mode");
            break;
        case 1: ESP_LOGI(TAG_MOTOR_DRIVER, "Motor driver is enabled in PARALLEL Mode");
            break;
        case 2: ESP_LOGI(TAG_MOTOR_DRIVER, "Motor driver is enabled in NORMAL Mode");
            break;
        case -1: ESP_LOGE(TAG_MOTOR_DRIVER, "Error initializing/configuring mcpwm");
            break;
        default: ESP_LOGE(TAG_MOTOR_DRIVER, "Invalid mode selected");
            break;
        }
} 