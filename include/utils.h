#ifndef _UTILS_H
#define _UTILS_H

/**
 * @brief Bound a value in a range [min, max]
 * 
 * @param val  value to be bound
 * @param min  min value which the variable can take
 * @param max  max value which the variable can take
 */
float bound(float val, float min, float max);

/**
 * @brief Maps a value in range [input_lower_limit, input_higher_limit] to [output_lower_limit, output_higher_limit]
 * 
 * @param val			value to be mapped
 * @param input_lower_limit	min value of val 
 * @param input_higher_limit	max value of val
 * @param output_lower_limit	input_lower_limit is mapped to output_lower_limit
 * @param output_higher_limit	input_higher_limit is mapped to output_higher_limit
 */
float map(float val, float input_lower_limit, float input_higher_limit, float output_lower_limit, float output_higher_limit);

#endif
