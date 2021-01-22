/*
 * MIT License
 *
 * Copyright (c)  2021 Society of Robotics and Automation
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

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
