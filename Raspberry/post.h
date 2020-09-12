#pragma once

#include <list>
#include <QMutex>
#include "paket.h"
#include "decoder.h"
#include <bcm2835.h>
class Paket;

#define DATA_BAUD BCM2835_SPI_CLOCK_DIVIDER_1024           //the STM32 are fast. use that
#define CONTROL_BAUD BCM2835_SPI_CLOCK_DIVIDER_8192        //The Atmega is mentally a bit slow, so it must be talked to in a slow and clear manner

class Post
{
public:
    static std::list<Paket*> Briefkasten;

    static void spi_thread();
    static void send_get_parameter(int index);
    static void send_set_parameter(int index, uint32_t nummer, std::string wert);
    static void send_get_daten(int index, uint32_t nummer, bool savepaket);
    static void send_start_kont();
    static void send_start_startstop();
    static void send_stop();
    static void send_set_sample_freq(float freq);
    static void send_get_status();
    static QMutex briefkasten_mutex;
};
