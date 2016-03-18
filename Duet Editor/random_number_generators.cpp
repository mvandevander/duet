#include "random_number_generators.hpp"

int random(int low, int high)
{
    float l = low;
    float h = high;
    float total_range = h-l;
    float rand_percent = float(rand())/float(RAND_MAX);

    return (int)((rand_percent*(total_range+1))+l);
    //return ((rand() * (high - low + 1))/RAND_MAX) + low;
}

float random(float low, float high)
{
    float l = low;
    float h = high;
    float total_range = h-l;
    float rand_percent = float(rand())/float(RAND_MAX);

    return (rand_percent*(total_range))+l;
    //return ((rand() * (high - low))/RAND_MAX) + low;
}
