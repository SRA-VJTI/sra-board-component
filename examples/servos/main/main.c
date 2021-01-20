/*
 * Copyright (c) 2021 Society of Robotics and Automation.
 *
 * This file is part of SRA Board Component 
 * (see https://www.sravjti.in).
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
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