#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "sra_board.h"

void app_main(void)
{
    // enable bar graph modules
    ESP_ERROR_CHECK(enable_bar_graph());

    // set bool array to 11001100, this will be shown on the bar graph
    bool arr[8] = {true, true, false, false, true, true, false, false};
    // this way we can set the bar graph using a boolean array, instead of 8-bit integer
    set_bar_graph(bool_to_uint8(arr));
    // wait for 3000ms
    vTaskDelay(3000 / portTICK_PERIOD_MS);

    // initialise a 8-bit interger to 0x01 - 0000 0001 in binary, this variable represents the bar graph 
    uint8_t var = 0x01;

    // loop in while(1) to show a cool effect on the bar graph indefinitely
    while(1)
    {
        // when variable becomes 0 due to rightshifting it 8 times, we reset the variable to 0x01
        if (var == 0x00)
        {
            var = 0x01;
        }
        
        // set bar graph according to the 8-bit integer variable var
        ESP_ERROR_CHECK(set_bar_graph(var));
        // right shift variable var once
        var = var << 1;
        // wait for 1000ms
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
