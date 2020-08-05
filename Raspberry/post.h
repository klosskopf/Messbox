#pragma once

#include <list>
#include <QMutex>
#include "paket.h"
#include "spi.h"
#include "decoder.h"
class Paket;

class Post
{
public:
    static std::list<Paket*> Briefkasten;

    static void spi_thread();
    static void send_get_parameter(int index);
    static void send_set_parameter(int index, uint32_t nummer, std::string wert);
    static void send_get_daten(int index, uint32_t nummer);
    static void send_start_kont();
    static void send_start_startstop();
    static void send_stop();
    static void send_set_sample_freq(float freq);
    static void send_get_status();
    static QMutex briefkasten_mutex;
};
