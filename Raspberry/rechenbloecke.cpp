#include "rechenbloecke.h"
#include "control.h"

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
    uint32_t ausgabe=(eingang - lasteingang) / (time - lasttime) * Control::samplefreq;
    lasttime=time;
    lasteingang=eingang;
    return ausgabe;
}

uint32_t Time_Block::newest()
{
    return -1;
}

uint32_t Constant_Block::newest()
{
    return -1;
}

void Constant_Block::set_constant()
{
    QLabel* label=new QLabel();
    QPixmap map= QPixmap(":images/const.png").scaled(30, 30);
    // map.fill(QColor("transparent"));
    QPainter painter(&map);
    painter.setPen(Qt::black);
    painter.setFont(QFont("Arial", 10));
    painter.drawText(4,23,QString::number(constant));
    label->setPixmap(map);
    setup_Rechenblock(label);
}


