#include "lsa.h"

static const int line_sensor_pins[4] = {LSA_A0, LSA_A1, LSA_A2, LSA_A3};

esp_err_t init_line_sensor(int* line_sensor_pins)
{
    esp_err_t err = enable_adc1(line_sensor_pins);
    return err;
}


line_sensor_array read_line_sensor()
{
    line_sensor_array line_sensor_readings;
    
    for (int i = 0; i < NUMBER_OF_SAMPLES; i++)
    {
        line_sensor_readings.adc_reading[i] = 0;
    }
    
    for(int i = 0; i < NUMBER_OF_SAMPLES; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            line_sensor_readings.adc_reading[j] = line_sensor_readings.adc_reading[j] + read_adc(line_sensor_pins[j]);
        }
    }

    for(int i = 0; i < 4; i++)
    {
        line_sensor_readings.adc_reading[i] = line_sensor_readings.adc_reading[i] / NUMBER_OF_SAMPLES;
    }
    
    return line_sensor_readings;
}