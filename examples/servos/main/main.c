#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "sra_board.h"

void app_main(void)
{
    // enable servo modules
    ESP_ERROR_CHECK(enable_servo());

    while(1){
        //For loop to sweep all servos from 0 degree to 180 degree
        for(int i = 0 ; i < 180 ; i++)
        {   
            //Servo id : SERVO_A(5) :: SERVO_B(18) :: SERVO_C(19) {DEFINED in pin_defs.h}
            //Passing servo id and angle to set_angle_servo function 
            set_angle_servo(SERVO_A,i);
            set_angle_servo(SERVO_B,i);
            set_angle_servo(SERVO_C,i);
            vTaskDelay(3/portTICK_PERIOD_MS);   
        }
            
        //For loop to sweep all servos from 180 degree to 0 degree
        for(int i = 180 ; i > 0; i--)
        {   
            //Passing servo id and angle to set_angle_servo function
            set_angle_servo(SERVO_A,i);
            set_angle_servo(SERVO_B,i);
            set_angle_servo(SERVO_C,i);
            vTaskDelay(3/portTICK_PERIOD_MS);   
        }
        //Delay of one second
        vTaskDelay(1000/portTICK_PERIOD_MS);  
    }
}