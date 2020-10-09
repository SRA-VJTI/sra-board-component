#include "servo.h"

static const char* TAG_SERVO = "servo";
static int enabled_servo_flag = 0;

esp_err_t enable_servo()
{
    esp_err_t err_A = mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM0A, SERVO_A);
    if (err_A == ESP_OK)
    {
        ESP_LOGI(TAG_SERVO, "enabled servo A");
    }
    else
    {
        ESP_LOGE(TAG_SERVO, "error: servo A: %s", esp_err_to_name(err_A));
        return err_A;    
    }
    
    esp_err_t err_B = mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM1A, SERVO_B);
    if (err_B == ESP_OK)
    {
        ESP_LOGI(TAG_SERVO, "enabled servo B");
    }
    else
    {
        ESP_LOGE(TAG_SERVO, "error: servo B: %s", esp_err_to_name(err_B));
        return err_B;    
    }

    esp_err_t err_C = mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM2A, SERVO_C);
    if (err_C == ESP_OK)
    {
        ESP_LOGI(TAG_SERVO, "enabled servo C");
    }
    else
    {
        ESP_LOGE(TAG_SERVO, "error: servo C: %s", esp_err_to_name(err_C));
        return err_C;    
    }

    mcpwm_config_t pwm_config;
    pwm_config.frequency = 50;    
    pwm_config.cmpr_a = 0;
    pwm_config.cmpr_b = 0;
    pwm_config.counter_mode = MCPWM_UP_COUNTER;
    pwm_config.duty_mode = MCPWM_DUTY_MODE_0;

    esp_err_t err__A = mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_0, &pwm_config);
    esp_err_t err__B = mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_1, &pwm_config);
    esp_err_t err__C = mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_2, &pwm_config);

    if (err_A == ESP_OK && err__B == ESP_OK && err_C == ESP_OK && err__A == ESP_OK && err__B == ESP_OK && err__C == ESP_OK)
    {
        enabled_servo_flag = 1;
        return ESP_OK;
    }
    else
    {
        enabled_servo_flag = 0;
        return ESP_FAIL;
    }
}

esp_err_t set_angle_servo(int servo_id, unsigned int degree_of_rotation)
{
    if (enabled_servo_flag)
    {
        if (servo_id == SERVO_A)
        {
            uint32_t cal_pulsewidth = 0;
            cal_pulsewidth = (SERVO_A_MIN_PULSEWIDTH + (((SERVO_A_MAX_PULSEWIDTH - SERVO_A_MIN_PULSEWIDTH) * (degree_of_rotation)) / (SERVO_A_MAX_DEGREE)));

            esp_err_t err_A = mcpwm_set_duty_in_us(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, cal_pulsewidth);
            if (err_A == ESP_OK)
            {
                ESP_LOGI(TAG_SERVO, "set servo A: %ud", degree_of_rotation);
            }
            else
            {
                ESP_LOGE(TAG_SERVO, "error: servo A: %s", esp_err_to_name(err_A));
            }           

            return err_A;
        }
        else if (servo_id == SERVO_B)
        {
            uint32_t cal_pulsewidth = 0;
            cal_pulsewidth = (SERVO_B_MIN_PULSEWIDTH + (((SERVO_B_MAX_PULSEWIDTH - SERVO_B_MIN_PULSEWIDTH) * (degree_of_rotation)) / (SERVO_B_MAX_DEGREE)));

            esp_err_t err_B = mcpwm_set_duty_in_us(MCPWM_UNIT_0, MCPWM_TIMER_1, MCPWM_OPR_A, cal_pulsewidth);
            if (err_B == ESP_OK)
            {
                ESP_LOGI(TAG_SERVO, "set servo B: %ud", degree_of_rotation);
            }
            else
            {
                ESP_LOGE(TAG_SERVO, "error: servo B: %s", esp_err_to_name(err_B));
            }           

            return err_B;
        }    
        else if (servo_id == SERVO_C)
        {
            uint32_t cal_pulsewidth = 0;
            cal_pulsewidth = (SERVO_C_MIN_PULSEWIDTH + (((SERVO_C_MAX_PULSEWIDTH - SERVO_C_MIN_PULSEWIDTH) * (degree_of_rotation)) / (SERVO_C_MAX_DEGREE)));

            esp_err_t err_C = mcpwm_set_duty_in_us(MCPWM_UNIT_0, MCPWM_TIMER_2, MCPWM_OPR_A, cal_pulsewidth);
            if (err_C == ESP_OK)
            {
                ESP_LOGI(TAG_SERVO, "set servo C: %ud", degree_of_rotation);
            }
            else
            {
                ESP_LOGE(TAG_SERVO, "error: servo C: %s", esp_err_to_name(err_C));
            }           

            return err_C;
        }    
        else
        {
            ESP_LOGE(TAG_SERVO, "error: incorrect servo pin passed to function");
            return ESP_FAIL;
        }
    }
    else
    {
        ESP_LOGE(TAG_SERVO, "error: servos not enabled, call enable_servo() before using servos");
        return ESP_FAIL;
    }   
}
