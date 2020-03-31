#include "rechenbloecke.h"

double Time::get_data(uint32_t time)
{
    return (double)time / Control::samplefreq;
}
uint32_t Time::newest()
{
    return -1;
}
