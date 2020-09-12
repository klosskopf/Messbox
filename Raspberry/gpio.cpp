#include "gpio.h"
#include <bcm2835.h>
#include <QtDebug>
void Gpio::init()
{
    interface_mutex.lock();
//    qDebug()<<"gpio nimmt interface";
    bcm2835_init();
    bcm2835_spi_begin();

    bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);
    bcm2835_spi_setDataMode(BCM2835_SPI_MODE0);
    bcm2835_spi_setClockDivider(DATA_BAUD);
    bcm2835_spi_chipSelect(BCM2835_SPI_CS0);
    bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, LOW);

    for (int i =1; i<11;i++) bcm2835_gpio_fsel(slave_to_gpio(i), BCM2835_GPIO_FSEL_INPT);
    for (int i =1; i<11;i++) bcm2835_gpio_set_pud(slave_to_gpio(i), BCM2835_GPIO_PUD_DOWN);

    bcm2835_gpio_fsel(slave_to_gpio(0), BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_set(slave_to_gpio(0));

    bcm2835_gpio_fsel (2, BCM2835_GPIO_FSEL_OUTP);//Red Led
    bcm2835_gpio_fsel (3, BCM2835_GPIO_FSEL_OUTP);//Green Led
    bcm2835_gpio_fsel (4, BCM2835_GPIO_FSEL_OUTP);//Blue Led

    bcm2835_gpio_fsel (21, BCM2835_GPIO_FSEL_INPT);//Button is input
    bcm2835_gpio_set(21);
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
        if(bcm2835_gpio_lev(slave_to_gpio(i)))
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
        bcm2835_spi_setClockDivider(CONTROL_BAUD);
        for (int i=1; i<11;i++)
        {
            bcm2835_gpio_clr(Gpio::slave_to_gpio(i));
            bcm2835_gpio_fsel(slave_to_gpio(i),BCM2835_GPIO_FSEL_OUTP);
        }
        bcm2835_gpio_clr(Gpio::slave_to_gpio(0));
    }
    else if (index==0)
    {
        bcm2835_spi_setClockDivider(CONTROL_BAUD);
        bcm2835_gpio_clr(slave_to_gpio(0));
    }
    else
    {
    bcm2835_gpio_clr(Gpio::slave_to_gpio(index));
    bcm2835_gpio_fsel(slave_to_gpio(index),BCM2835_GPIO_FSEL_OUTP);
    }
}

void Gpio::disable_slave(int index)
{
    if (index==-1)
    {
        bcm2835_spi_setClockDivider(DATA_BAUD);
        for (int i=1; i<11;i++)
        {
            bcm2835_gpio_set(Gpio::slave_to_gpio(i));
            bcm2835_gpio_fsel(slave_to_gpio(i),BCM2835_GPIO_FSEL_INPT);
        }
        bcm2835_gpio_set(Gpio::slave_to_gpio(0));
    }
    else if (index==0)
    {
        bcm2835_spi_setClockDivider(DATA_BAUD);
        bcm2835_gpio_set(slave_to_gpio(0));
    }
    else
    {
        bcm2835_gpio_set(Gpio::slave_to_gpio(index));
        bcm2835_gpio_fsel(slave_to_gpio(index),BCM2835_GPIO_FSEL_INPT);
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
        bcm2835_gpio_clr(2);
        bcm2835_gpio_clr(3);
        bcm2835_gpio_set(4);
        break;
    case LED_RUN:
        bcm2835_gpio_clr(2);
        bcm2835_gpio_set(3);
        bcm2835_gpio_clr(4);
        break;
    case LED_ERROR:
        bcm2835_gpio_set(2);
        bcm2835_gpio_clr(3);
        bcm2835_gpio_clr(4);
        break;
    }
//    qDebug()<<"led gibt interface";
    interface_mutex.unlock();
}

bool Gpio::read_button()
{
    bool retval=false;
    static int buttoncnt=0;
    if(!bcm2835_gpio_lev(21))
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
