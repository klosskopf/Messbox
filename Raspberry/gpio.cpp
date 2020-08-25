#include "gpio.h"
#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <QtDebug>
void Gpio::init()
{
    interface_mutex.lock();
//    qDebug()<<"gpio nimmt interface";
    wiringPiSetupGpio();

    for (int i =1; i<11;i++) pinMode (slave_to_gpio(i), INPUT);
    for (int i =1; i<11;i++) pullUpDnControl(slave_to_gpio(i), PUD_DOWN);

    pinMode(slave_to_gpio(0), OUTPUT);
    digitalWrite(slave_to_gpio(0),1);

    pinMode (2, OUTPUT);//Red Led
    pinMode (3, OUTPUT);//Green Led
    pinMode (4, OUTPUT);//Blue Led

    pinMode (21, INPUT);//Button is input
    digitalWrite(21,1);
    //pullUpDnControl(21,PUD_UP);

 //   qDebug()<<"gpio gibt interface";
    interface_mutex.unlock();
    Gpio::set_led(LED_STOP);
}
std::list<int> Gpio::get_new_karten()
{
    std::list<int> n_Karten;
    interface_mutex.lock();
//    qDebug()<<"control nimmt interface";
    for(int i=1; i<11;i++)
    {
        if(digitalRead(slave_to_gpio(i)))
        {
            n_Karten.push_back(i);
        }
    }
 //   qDebug()<<"control gibt interface";
    interface_mutex.unlock();
    return n_Karten;
}
void Gpio::enable_slave(int index)
{
    interface_mutex.lock();
  //  qDebug()<<"post nimmt interface";
    if (index==-1)
    {
        wiringPiSPISetup (0, CONTROL_BAUD);
        for (int i=1; i<11;i++)
        {
            digitalWrite(Gpio::slave_to_gpio(i),0);
            pinMode(slave_to_gpio(i),OUTPUT);
        }
        digitalWrite(Gpio::slave_to_gpio(0),0);
    }
    else
    {
    digitalWrite(Gpio::slave_to_gpio(index),0);
    pinMode(slave_to_gpio(index),OUTPUT);
    }
}

void Gpio::disable_slave(int index)
{
    if (index==-1)
    {
        wiringPiSPISetup (0, DATA_BAUD);
        for (int i=1; i<11;i++)
        {
            digitalWrite(Gpio::slave_to_gpio(i),1);
            pinMode(slave_to_gpio(i),INPUT);
        }
        digitalWrite(Gpio::slave_to_gpio(0),1);
    }
    else
    {
        digitalWrite(Gpio::slave_to_gpio(index),1);
        pinMode(slave_to_gpio(index),INPUT);
    }
//    qDebug()<<"post gibt interface";
    interface_mutex.unlock();
}

int Gpio::slave_to_gpio(int slave)
{
    const int lookup[11]={5,24,23,22,27,18,26,16,19,13,12};
    return(lookup[slave]);
}

void Gpio::set_led(led_state_t state)
{
    interface_mutex.lock();
//    qDebug()<<"led nimmt interface";
    switch (state)
    {
    case LED_STOP:
        digitalWrite(2,0);
        digitalWrite(3,0);
        digitalWrite(4,1);
        break;
    case LED_RUN:
        digitalWrite(2,0);
        digitalWrite(3,1);
        digitalWrite(4,0);
        break;
    case LED_ERROR:
        digitalWrite(2,1);
        digitalWrite(3,0);
        digitalWrite(4,0);
        break;
    }
//    qDebug()<<"led gibt interface";
    interface_mutex.unlock();
}

bool Gpio::read_button()
{
    bool retval=false;
    static int buttoncnt=0;
    if(!digitalRead(21))
    {
        if(buttoncnt)
            buttoncnt++;
        if (buttoncnt==50)  //in ms
        {
            buttoncnt=0;
            retval=1;
        }
    }
    else
    {
       buttoncnt=1;
    }
    return retval;
}

QMutex Gpio::interface_mutex;
