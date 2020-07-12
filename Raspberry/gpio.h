#pragma once

#include <list>
#include <QMutex>
#include "karte.h"
class Karte;
typedef enum {LED_STOP,LED_RUN,LED_ERROR}led_state_t;

class Gpio
{
public:
    static void init();
    static std::list<int> get_new_karten();
    static void enable_slave(int index);
    static void disable_slave(int index);
    static void set_led(led_state_t state);
    static int slave_to_gpio(int);
    static QMutex gpiomutex;
};
