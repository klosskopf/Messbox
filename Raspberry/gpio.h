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
    static void set_SS(Karte* karte);
};

#endif // GPIO_H
