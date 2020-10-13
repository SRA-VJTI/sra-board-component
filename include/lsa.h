#ifndef LSA_H
#define LSA_H

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
    }lsa;

    int adc_reading[4];
}line_sensor_array;


/**
 * @brief enables and configures adc
 
 * @param none

 * @return esp_err_t i.e it shows if adc is initialised successfully or not
 **/
esp_err_t init_line_sensor();


/**
 * @brief Get multisampled line sensor array readings
 
 * @param none
 
 * @return Returns a pointer to a struct that contains multisampled adc readings 
           for all the pins specified in lsa_pins array
**/
line_sensor_array read_line_sensor();

#endif
