#include "utils.h"

float bound(float val, float min, float max)
{
    if (val > max)
        val = max;
    else if (val < min)
        val = min;
    return val;
}

float map(float val, float input_lower_limit, float input_higher_limit, float output_lower_limit, float output_higher_limit)
{
    return (output_lower_limit + ((val - input_lower_limit) * ((output_higher_limit - output_lower_limit) / (input_higher_limit - input_lower_limit))));
}
