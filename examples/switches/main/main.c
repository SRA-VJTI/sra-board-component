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
