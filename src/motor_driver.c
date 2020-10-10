#include "motor_driver.h"

static const char* TAG_MOTOR_DRIVER = "motor_driver";
static int MODE = 0; //motor driver is disabled by default




esp_err_t enable_motor_driver(int MODE)
{
    esp_err_t err;
    

    if(MODE == 1)  //Initialize motor driver in Parallel mode
    {
        err = mcpwm_gpio_init(MCPWM_UNIT_1, MCPWM1A, MDA_PARALLEL_IN_1_2);
        if (err != ESP_OK)
        {
            ESP_LOGE(TAG_MOTOR_DRIVER, "error: %s", esp_err_to_name(err));
            return err;    
        }
        err = mcpwm_gpio_init(MCPWM_UNIT_1, MCPWM1B, MDA_PARALLEL_IN_3_4);
        if (err != ESP_OK)
        {
            ESP_LOGE(TAG_MOTOR_DRIVER, "error: %s", esp_err_to_name(err));
            return err;    
        }
        err = mcpwm_gpio_init(MCPWM_UNIT_1, MCPWM2A, MDA_PARALLEL_IN_5_7);
        if (err != ESP_OK)
        {
            ESP_LOGE(TAG_MOTOR_DRIVER, "error: %s", esp_err_to_name(err));
            return err;    
        }
        err = mcpwm_gpio_init(MCPWM_UNIT_1, MCPWM2B, MDA_PARALLEL_IN_6_8);
        if (err != ESP_OK)
        {
            ESP_LOGE(TAG_MOTOR_DRIVER, "error: %s", esp_err_to_name(err));
            return err;    
        }
    }
    else if(MODE == 2)   //Initialize motor driver in Normal mode
    {
        err = mcpwm_gpio_init(MCPWM_UNIT_1, MCPWM0A, MDA_NORMAL_IN_1);
        if (err != ESP_OK)
        {
            ESP_LOGE(TAG_MOTOR_DRIVER, "error: %s", esp_err_to_name(err));
            return err;    
        }
        err = mcpwm_gpio_init(MCPWM_UNIT_1, MCPWM0B, MDA_NORMAL_IN_2);
        if (err != ESP_OK)
        {
            ESP_LOGE(TAG_MOTOR_DRIVER, "error: %s", esp_err_to_name(err));
            return err;    
        }
        err = mcpwm_gpio_init(MCPWM_UNIT_1, MCPWM1A, MDA_NORMAL_IN_3);
        if (err != ESP_OK)
        {
            ESP_LOGE(TAG_MOTOR_DRIVER, "error: %s", esp_err_to_name(err));
            return err;    
        } 
        err = mcpwm_gpio_init(MCPWM_UNIT_1, MCPWM1B, MDA_NORMAL_IN_4);
        if (err != ESP_OK)
        {
            ESP_LOGE(TAG_MOTOR_DRIVER, "error: %s", esp_err_to_name(err));
            return err;    
        }
        err = mcpwm_gpio_init(MCPWM_UNIT_1, MCPWM2A, MDA_NORMAL_IN_5);
        if (err != ESP_OK)
        {
            ESP_LOGE(TAG_MOTOR_DRIVER, "error: %s", esp_err_to_name(err));
            return err;    
        }
        err = mcpwm_gpio_init(MCPWM_UNIT_1, MCPWM2B, MDA_NORMAL_IN_6);
        if (err != ESP_OK)
        {
            ESP_LOGE(TAG_MOTOR_DRIVER, "error: %s", esp_err_to_name(err));
            return err;    
        }
        err = mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM0B, MDA_NORMAL_IN_7);
        if (err != ESP_OK)
        {
            ESP_LOGE(TAG_MOTOR_DRIVER, "error: %s", esp_err_to_name(err));
            return err;    
        }
        err = mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM1B, MDA_NORMAL_IN_8);
        if (err != ESP_OK)
        {
            ESP_LOGE(TAG_MOTOR_DRIVER, "error: %s", esp_err_to_name(err));
            return err;    
        }
    }


 
    ESP_LOGI(TAG_MOTOR_DRIVER, "Configuring Initial Parameters of mcpwm...\n");
    mcpwm_config_t pwm_config;
    pwm_config.frequency = 20000;    //frequency = 500Hz,
    pwm_config.cmpr_a = 0;    //duty cycle of PWMxA = 0
    pwm_config.cmpr_b = 0;    //duty cycle of PWMxb = 0
    pwm_config.counter_mode = MCPWM_UP_COUNTER;
    pwm_config.duty_mode = MCPWM_DUTY_MODE_0;
    ESP_LOGI(TAG_MOTOR_DRIVER, "Configuring pwm_config...\n");

    if (MODE ==1)
    {
        esp_err_t err1_1 = mcpwm_init(MCPWM_UNIT_1, MCPWM_TIMER_1, &pwm_config);
        esp_err_t err1_2 = mcpwm_init(MCPWM_UNIT_1, MCPWM_TIMER_2, &pwm_config);
        if (err_1_1 == ESP_OK && err1_2 == ESP_OK )
        {
            MODE = 1;
            ESP_LOGI(TAG_MOTOR_DRIVER, "enabled motors");

            return ESP_OK;
        }
        else
        {
            MODE = 0;
            return ESP_FAIL;
        }
    }
    else if(MODE == 2)
    {
        esp_err_t err0_0 = mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_0, &pwm_config);    
        esp_err_t err0_1 = mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_1, &pwm_config);    
        esp_err_t err1_0 = mcpwm_init(MCPWM_UNIT_1, MCPWM_TIMER_0, &pwm_config);    
        esp_err_t err1_1 = mcpwm_init(MCPWM_UNIT_1, MCPWM_TIMER_1, &pwm_config);    
        esp_err_t err1_2 = mcpwm_init(MCPWM_UNIT_1, MCPWM_TIMER_2, &pwm_config);  
        if (err0_0 == ESP_OK && err0_1 == ESP_OK && err1_0 == ESP_OK && err1_1 == ESP_OK && err1_2 == ESP_OK )
        {
            MODE = 2;
            ESP_LOGI(TAG_MOTOR_DRIVER, "enabled motors");

            return ESP_OK;
        }
        else
        {
            MODE = 0;
            return ESP_FAIL;
        }  
                 
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

int read_motor_driver_mode() 
{
        return MODE;
} 