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
     // Enable swtiches
     enable_switches();
    
    
    	while (1) 
    	{
    		//Print the state of Switches , 1 if it is pressed and 0 if it is not pressed.
        	printf("Switch_1 : %d || Switch_2 : %d || Switch_3 : %d || Switch_4 : %d " , read_switch(SWITCH_1) , read_switch(SWITCH_2) , read_switch(SWITCH_3) , read_switch(SWITCH_4) );
        	
        	// wait for 1000ms
        	vTaskDelay(1000 / portTICK_PERIOD_MS);
        }
    
}
