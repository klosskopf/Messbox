#include "gpio.h"
#include <wiringPi.h>
#include "spi.h"
void Gpio::init()
{
    Gpio::gpiomutex.lock();
    wiringPiSetupGpio();

    for (int i =1; i<11;i++) pinMode (slave_to_gpio(i), INPUT);
    for (int i =1; i<11;i++) pullUpDnControl(slave_to_gpio(i), PUD_DOWN);

    pinMode(slave_to_gpio(0), OUTPUT);
    digitalWrite(slave_to_gpio(0),1);

    pinMode (2, OUTPUT);//Red Led
    pinMode (3, OUTPUT);//Green Led
    pinMode (4, OUTPUT);//Blue Led
    Gpio::set_led(LED_STOP);
    Gpio::gpiomutex.unlock();
}
std::list<int> Gpio::get_new_karten()
{
    Gpio::gpiomutex.lock();
    std::list<int> n_Karten;

    for(int i=1; i<11;i++)
    {
        if(digitalRead(slave_to_gpio(i)))
        {
            n_Karten.push_back(i);
        }
    }

    Gpio::gpiomutex.unlock();
    return n_Karten;
}
void Gpio::enable_slave(int index)
{
    Gpio::gpiomutex.lock();
    if (index==-1)
    {
        Spi::init_spi(CONTROL_BAUD);
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
        Spi::init_spi(DATA_BAUD);
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
    Gpio::gpiomutex.unlock();
}

int Gpio::slave_to_gpio(int slave)
{
    const int lookup[11]={5,24,23,22,27,18,26,16,19,13,12};
    return(lookup[slave]);
}

void Gpio::set_led(led_state_t state)
{
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
}

QMutex Gpio::gpiomutex;
