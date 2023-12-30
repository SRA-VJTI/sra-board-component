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

#include "lsa.h"

static const int line_sensor_pins[5] = {LSA_A0, LSA_A1, LSA_A2, LSA_A3, LSA_A4};

esp_err_t enable_line_sensor(adc_handle_t *adc_handle)
{
    ESP_ERROR_CHECK(enable_adc1(adc_handle));
    return ESP_OK;
}

line_sensor_array read_line_sensor(adc_handle_t adc_handle)
{
    line_sensor_array line_sensor_readings;

    for (int i = 0; i < 5; i++)
    {
        line_sensor_readings.adc_reading[i] = 0;
    }

    for (int i = 0; i < NUMBER_OF_SAMPLES; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            line_sensor_readings.adc_reading[j] = line_sensor_readings.adc_reading[j] + read_adc(adc_handle, line_sensor_pins[j]);
        }
    }

    for (int i = 0; i < 5; i++)
    {
        line_sensor_readings.adc_reading[i] = line_sensor_readings.adc_reading[i] / NUMBER_OF_SAMPLES;
    }

    return line_sensor_readings;
}
