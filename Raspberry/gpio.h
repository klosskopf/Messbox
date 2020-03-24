#ifndef GPIO_H
#define GPIO_H

#include <list>
#include "karte.h"
class Karte;

class Gpio
{
public:
    static void init();
    static std::list<int> get_new_karten();
    static void enable_slave(uint8_t index);
    static void disable_slave(uint8_t index);
};

#endif // GPIO_H
