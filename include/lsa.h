#ifndef LSA_H
#define LSA_H

#include "pin_defs.h"
#include "adc.h"

#define NO_OF_SAMPLES   64 

/**
 * @brief apply multisampling( Average over NO_OF SAMPLE readings ) to get stable readings 
 
 * @param lsa_pin The adc pin on which multisampling is to be applied
 
 * @return adc_reading- The multisampled reading of given lsa_pin
 **/
int multisampling(int lsa_pin);


/**
 * @brief Get multisampled lsa readings
 
 * @param lsa_pins[] array that contains all lsa pins
 
 * @return Returns an int array of lsa readings for the given pins.
**/
int get_lsa_readings(int lsa_pins[]);

#endif