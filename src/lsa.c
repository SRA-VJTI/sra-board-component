#include "lsa.h"

static const int line_sensor_pins[4] = {LSA_A0, LSA_A1, LSA_A2, LSA_A3};

esp_err_t init_line_sensor(int* line_sensor_pins)
{
    esp_err_t err = enable_adc1(line_sensor_pins);
    return err
}


line_sensor_array read_line_sensor(int* line_sensor_pins)
{
    line_sensor_array line_sensor_readings;
    int adc_reading[4] = {0};

    for( int i = 0; i < NO_OF_SAMPLES; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            adc_reading[j] = adc_reading[j] + read_adc(line_sensor_pins[j]);
        }
    }

    line_sensor_readings.A0 = adc_reading[0] / NO_OF_SAMPLES;
    line_sensor_readings.A1 = adc_reading[1] / NO_OF_SAMPLES;
    line_sensor_readings.A2 = adc_reading[2] / NO_OF_SAMPLES;
    line_sensor_readings.A3 = adc_reading[3] / NO_OF_SAMPLES;

    return line_sensor_readings;
}