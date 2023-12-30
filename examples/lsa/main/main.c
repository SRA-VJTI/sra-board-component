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

#define BLACK_MARGIN 4095
#define WHITE_MARGIN 0
#define CONSTRAIN_LSA_LOW 0
#define CONSTRAIN_LSA_HIGH 1000

static const char* TAG = "LSA_READINGS";

void app_main(void)
{
    // enable line sensor
    adc_handle_t adc_handle;
    ESP_ERROR_CHECK(enable_line_sensor(&adc_handle));
    line_sensor_array line_sensor_readings;
    while (1)
    {
        // get line sensor readings
        line_sensor_readings = read_line_sensor(adc_handle);
        for(int i = 0; i < 5; i++)
        {
            // constrain lsa readings between BLACK_MARGIN and WHITE_MARGIN
            line_sensor_readings.adc_reading[i] = bound(line_sensor_readings.adc_reading[i], WHITE_MARGIN, BLACK_MARGIN);
            // map readings from (BLACK_MARGIN, WHITE_MARGIN) to (CONSTRAIN_LSA_LOW, CONSTRAIN_LSA_HIGH)
            line_sensor_readings.adc_reading[i] = map(line_sensor_readings.adc_reading[i], WHITE_MARGIN, BLACK_MARGIN, CONSTRAIN_LSA_LOW, CONSTRAIN_LSA_HIGH);
            line_sensor_readings.adc_reading[i] = 1000 - line_sensor_readings.adc_reading[i];
            vTaskDelay(10);
        }

        // log final lsa readings
        ESP_LOGI(TAG, "LSA_0: %d \t LSA_1: %d \t LSA_2: %d \t LSA_3: %d \t LSA_4: %d",line_sensor_readings.adc_reading[0], line_sensor_readings.adc_reading[1], line_sensor_readings.adc_reading[2], line_sensor_readings.adc_reading[3], line_sensor_readings.adc_reading[4]);
    }
}