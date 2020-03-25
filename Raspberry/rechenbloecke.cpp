#include "rechenbloecke.h"

float Time::get_data(uint32_t time)
{
    return (float)time / Control::samplefreq;
}
uint32_t Time::newest()
{
    return -1;
}
