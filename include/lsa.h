#ifndef LSA_H
#define LSA_H

#include "pin_defs.h"
#include "adc.h"

#define NO_OF_SAMPLES   64 


typedef struct line_sensor_array
{
    int adc_readings[4]={0};
}line_sensor_array;


/**
 * @brief enables and configures adc
 
 * @param lsa_pins[] array that contains all lsa pins that are to be initialised
 
 * @return esp_err_t i.e it shows if adc is initialised successfully or not
 **/
esp_err_t init_line_sensor(int* line_sensor_pins);


/**
 * @brief Get multisampled line sensor array readings
 
 * @param lsa_pins[] array that contains all lsa pins
 
 * @return Returns a pointer to a struct that contains multisampled adc readings 
           for all the pins specified in lsa_pins array
**/
line_sensor_array* get_line_sensor_array_readings(int* line_sensor_pins);

#endif