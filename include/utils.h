/*
 * MIT License
 *
 * Copyright (c)  2021 Society of Robotics and Automation
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

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
