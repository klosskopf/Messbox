#include "rechenbloecke.h"
#include "control.h"

double Axis_Block::get_data(uint32_t time)
{
    if(eingaenge.size()>0)
    {
        return eingaenge.front()->get_data(time);
    }
    else
    {
        return 0;
    }
}

double Plus_Block::get_data(uint32_t time)
{
    double ausgang=0;
    for (Rechenblock* eingang : eingaenge)
    {
        ausgang+=eingang->get_data(time);
    }
    return ausgang;
}

double Minus_Block::get_data(uint32_t time)
{
    if(eingaenge.size()>0)
    {
        double ausgang=2*eingaenge.front()->get_data(time);
        for (Rechenblock* eingang : eingaenge)
        {
            ausgang-=eingang->get_data(time);
        }
        return ausgang;
    }
    else
    {
        return 0;
    }
}

double Mal_Block::get_data(uint32_t time)
{
    if(eingaenge.size()>0)
    {
        double ausgang=1;
        for (Rechenblock* eingang : eingaenge)
        {
            ausgang *= eingang->get_data(time);
        }
        return ausgang;
    }
    else
    {
        return 0;
    }
}

double Geteilt_Block::get_data(uint32_t time)
{
    if(eingaenge.size()>0)
    {
        double ausgang=eingaenge.front()->get_data(time);
        ausgang *= ausgang;
        for (Rechenblock* eingang : eingaenge)
        {
            ausgang/=eingang->get_data(time);
        }
        return ausgang;
    }
    else
    {
        return 0;
    }
}

double Constant_Block::get_data(uint32_t time)
{
    return constant;
}

double Time_Block::get_data(uint32_t time)
{
    return (double)time / Control::samplefreq;
}

double Integrate_Block::get_data(uint32_t time)
{
    lastausgabe+=(eingaenge.front()->get_data(time) * (time-lasttime) / Control::samplefreq);
    lasttime=time;
    return lastausgabe;
}

double Derivate_Block::get_data(uint32_t time)
{
    double eingang=eingaenge.front()->get_data(time);
    double ausgabe=(eingang - lasteingang) * Control::samplefreq / (time - lasttime);
    lasttime=time;
    lasteingang=eingang;
    return ausgabe;
}

double Klammerauf_Block::get_data(uint32_t time)
{
    return eingaenge.front()->get_data(time);
}

double Klammerzu_Block::get_data(uint32_t time)
{
    return 0;
}

uint32_t Time_Block::newest()
{
    return -2;
}

uint32_t Constant_Block::newest()
{
    return -2;
}


