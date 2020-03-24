#ifndef POST_H
#define POST_H

#include <list>
#include "paket.h"
#include "spi.h"
#include "decoder.h"
class Paket;

class Post
{
private:
    static std::list<Paket*> Briefkasten;
public:
    static void spi_thread();
    static void send_get_parameter(int index);
    static void send_set_parameter(int index, uint32_t nummer, std::string wert);
    static void send_get_daten(int index, uint32_t nummer);
    static void send_start_kont();
    static void send_start_startstop();
    static void send_set_sample_freq(float freq);
    static void send_get_status();
};

#endif // POST_H
