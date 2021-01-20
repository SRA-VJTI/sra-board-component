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
#include "lsa.h"

static const int line_sensor_pins[4] = {LSA_A0, LSA_A1, LSA_A2, LSA_A3};

esp_err_t enable_line_sensor()
{
    esp_err_t err = enable_adc1(line_sensor_pins);
    return err;
}

line_sensor_array read_line_sensor()
{
    line_sensor_array line_sensor_readings;

    for (int i = 0; i < 4; i++)
    {
        line_sensor_readings.adc_reading[i] = 0;
    }

    for (int i = 0; i < NUMBER_OF_SAMPLES; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            line_sensor_readings.adc_reading[j] = line_sensor_readings.adc_reading[j] + read_adc(line_sensor_pins[j]);
        }
    }

    for (int i = 0; i < 4; i++)
    {
        line_sensor_readings.adc_reading[i] = line_sensor_readings.adc_reading[i] / NUMBER_OF_SAMPLES;
    }

    return line_sensor_readings;
}