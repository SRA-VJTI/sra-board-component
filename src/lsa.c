#include "lsa.h"

//int lsa_pins = {LSA_A0, LSA_A1, LSA_A2, LSA_A3};

int multisampling(int lsa_pin)
{
    int adc_reading = 0;
    for( int j = 0 ; j < NO_OF_SAMPLES; j++)
    {
        adc_reading += read_adc(lsa_pin);
    }
    adc_reading /= NO_OF_SAMPLES;
    return adc_reading;
}


int get_lsa_readings(int lsa_pins[])
{
    int adc_readings[4] = {0};

    for( int i = 0 ; i < 4 ; i++)
    {
        adc_readings[i] = multisampling(lsa_pins[i]);
    }
    return adc_readings;
}