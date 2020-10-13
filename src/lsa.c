#include "lsa.h"


esp_err_t init_line_sensor(int* line_sensor_pins)
{
    esp_err_t err = enable_adc1(line_sensor_pins);
    return err
}


line_sensor_array* get_line_sensor_array_readings(int* line_sensor_pins)
{
    line_sensor_array* line_sensor_readings;

    for( int i = 0; i < NO_OF_SAMPLES; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            line_sensor_readings->adc_readings[i] = line_sensor_readings->adc_readings[i] + read_adc(line_sensor_pins[i]);
        }
    }

    for( int i = 0; i < 4; i++)
    {
        line_sensor_readings->adc_readings[i] = line_sensor_readings->adc_readings[i]/NO_OF_SAMPLES;
    }

    return line_sensor_readings;
}x