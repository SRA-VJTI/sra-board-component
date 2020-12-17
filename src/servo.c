#include "servo.h"

#define CHECK(x) do { esp_err_t __; if ((__ = x) != ESP_OK) return __; } while (0)
#define CHECK_LOGE(err, x, tag, msg, ...) do { \
        if ((err = x) != ESP_OK) { \
            ESP_LOGE(tag, msg, ## __VA_ARGS__); \
            return err; \
        } \
    } while (0)

static const char* TAG_SERVO = "servo";
static int enabled_servo_flag = 0;

esp_err_t enable_servo()
{
    esp_err_t err;
    CHECK_LOGE(err, mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM0A, SERVO_A), "error: servo A: %s", esp_err_to_name(err));
    CHECK_LOGE(err, mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM0B, SERVO_B), "error: servo B: %s", esp_err_to_name(err));
    CHECK_LOGE(err, mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM1A, SERVO_C), "error: servo C: %s", esp_err_to_name(err));

    mcpwm_config_t pwm_config;
    // sets the pwm frequency = 50
    pwm_config.frequency = 50;
    // sets the initial duty cycle of PWMxA = 0    
    pwm_config.cmpr_a = 0;
    // sets the initial duty cycle of PWMxB = 0
    pwm_config.cmpr_b = 0;
    // sets the pwm counter mode
    pwm_config.counter_mode = MCPWM_UP_COUNTER;
    // sets the pwm duty mode
    pwm_config.duty_mode = MCPWM_DUTY_MODE_0;

    // init pwm 0a, 1a, 2a with the above settings
    esp_err_t err_A = mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_0, &pwm_config);
    esp_err_t err_B = mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_1, &pwm_config);

    if (err_A == ESP_OK && err_B == ESP_OK)
    {
        enabled_servo_flag = 1;
        ESP_LOGI(TAG_SERVO, "enabled servos");

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
            degree_of_rotation = degree_of_rotation > SERVO_A_MAX_DEGREE ? SERVO_A_MAX_DEGREE : degree_of_rotation;

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
            degree_of_rotation = degree_of_rotation > SERVO_B_MAX_DEGREE ? SERVO_B_MAX_DEGREE : degree_of_rotation;

            uint32_t cal_pulsewidth = 0;
            cal_pulsewidth = (SERVO_B_MIN_PULSEWIDTH + (((SERVO_B_MAX_PULSEWIDTH - SERVO_B_MIN_PULSEWIDTH) * (degree_of_rotation)) / (SERVO_B_MAX_DEGREE)));

            esp_err_t err_B = mcpwm_set_duty_in_us(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_B, cal_pulsewidth);
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
            degree_of_rotation = degree_of_rotation > SERVO_C_MAX_DEGREE ? SERVO_C_MAX_DEGREE : degree_of_rotation;

            uint32_t cal_pulsewidth = 0;
            cal_pulsewidth = (SERVO_C_MIN_PULSEWIDTH + (((SERVO_C_MAX_PULSEWIDTH - SERVO_C_MIN_PULSEWIDTH) * (degree_of_rotation)) / (SERVO_C_MAX_DEGREE)));

            esp_err_t err_C = mcpwm_set_duty_in_us(MCPWM_UNIT_0, MCPWM_TIMER_1, MCPWM_OPR_A, cal_pulsewidth);
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
