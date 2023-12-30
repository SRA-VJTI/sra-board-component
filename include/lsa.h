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

#ifndef LSA_H
#define LSA_H

#include "sdkconfig.h"
#include "pin_defs.h"
#include "adc.h"

#define NUMBER_OF_SAMPLES CONFIG_NUMBER_OF_SAMPLES

typedef union line_sensor_array
{
    struct line_sensor
    {
        int A0;
        int A1;
        int A2;
        int A3;
        int A4;
    } lsa;

    int adc_reading[5];
} line_sensor_array;

/**
 * @brief enables and configures adc
 
 * @param none

 * @return esp_err_t i.e it shows if adc is initialised successfully or not
 **/
esp_err_t enable_line_sensor(adc_handle_t *adc_handle);

/**
 * @brief Get multisampled line sensor array readings
 
 * @param none
 
 * @return Returns a pointer to a struct that contains multisampled adc readings 
           for all the pins specified in lsa_pins array
**/
line_sensor_array read_line_sensor(adc_handle_t adc_handle);

#endif
