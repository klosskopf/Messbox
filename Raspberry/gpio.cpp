#include "gpio.h"
void Gpio::init()
{

}
std::list<int> Gpio::get_new_karten()
{
    std::list<int> n_Karten;
    n_Karten.push_back(1);
    n_Karten.push_back(2);
    return n_Karten;
}
void Gpio::enable_slave(uint8_t index)
{
    Spi::i=0;
}

void Gpio::disable_slave(uint8_t index)
{

}
