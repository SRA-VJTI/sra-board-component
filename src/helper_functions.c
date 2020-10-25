#include "helper_functions.h"

float bound(float val, float min, float max)
{
    if (val > max)
        val = max;
    else if (val < min)
        val = min;
    return val;
}