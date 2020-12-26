#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "sra_board.h"

static const char* TAG_SWITCH_CONTROL_MOTOR = "switch_control";

void app_main(void)
{
    enable_motor_driver(a,PARALLEL_MODE); // Enable motor driver A in Parallel Mode
	enable_motor_driver(b,PARALLEL_MODE); // Enable motor driver B in Parallel Mode
    while (1) 
    {
        // Rotate Motors A_0 and B_0 in FORWARD direction
        set_motor_speed(MOTOR_A_0, MOTOR_FORWARD, 80);
		ESP_LOGI(TAG_SWITCH_CONTROL_MOTOR, "MOTOR_A_0 FORWARD");
        set_motor_speed(MOTOR_B_0, MOTOR_FORWARD, 80);
		ESP_LOGI(TAG_SWITCH_CONTROL_MOTOR, "MOTOR_B_0 FORWARD");

        vTaskDelay(1000 / portTICK_PERIOD_MS);

        // Rotate Motors A_0 and B_0 in BACKWARD direction
        set_motor_speed(MOTOR_A_0, MOTOR_BACKWARD, 80);
		ESP_LOGI(TAG_SWITCH_CONTROL_MOTOR, "MOTOR_A_0 BACKWARD");
        set_motor_speed(MOTOR_B_0, MOTOR_BACKWARD, 80);
		ESP_LOGI(TAG_SWITCH_CONTROL_MOTOR, "MOTOR_B_0 BACKWARD");

        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
