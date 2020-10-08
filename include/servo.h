#ifndef SERVO_H
#define SERVO_H

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "driver/mcpwm.h"
#include "soc/mcpwm_periph.h"
#include "esp_attr.h"

#include "sdkconfig.h"
#include "esp_log.h"
#include "esp_err.h"
#include "pin_defs.h"

#define SERVO_A_MIN_PULSEWIDTH CONFIG_SERVO_A_MIN_PULSEWIDTH 
#define SERVO_A_MAX_PULSEWIDTH CONFIG_SERVO_A_MAX_PULSEWIDTH
#define SERVO_A_MAX_DEGREE     CONFIG_SERVO_A_MAX_DEGREE

#define SERVO_B_MIN_PULSEWIDTH CONFIG_SERVO_B_MIN_PULSEWIDTH 
#define SERVO_B_MAX_PULSEWIDTH CONFIG_SERVO_B_MAX_PULSEWIDTH
#define SERVO_B_MAX_DEGREE     CONFIG_SERVO_B_MAX_DEGREE

#define SERVO_C_MIN_PULSEWIDTH CONFIG_SERVO_C_MIN_PULSEWIDTH 
#define SERVO_C_MAX_PULSEWIDTH CONFIG_SERVO_C_MAX_PULSEWIDTH
#define SERVO_C_MAX_DEGREE     CONFIG_SERVO_C_MAX_DEGREE

esp_err_t enable_servo();

esp_err_t set_angle_servo(int servo_id, unsigned int degree_of_rotation);

#endif