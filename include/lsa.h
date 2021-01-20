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
    } lsa;

    int adc_reading[4];
} line_sensor_array;

/**
 * @brief enables and configures adc
 
 * @param none

 * @return esp_err_t i.e it shows if adc is initialised successfully or not
 **/
esp_err_t enable_line_sensor();

/**
 * @brief Get multisampled line sensor array readings
 
 * @param none
 
 * @return Returns a pointer to a struct that contains multisampled adc readings 
           for all the pins specified in lsa_pins array
**/
line_sensor_array read_line_sensor();

#endif
