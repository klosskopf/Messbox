#pragma once

#include "paket.h"
#include <list>
class Paket;

class Decoder
{
private:
    static std::list<Paket*> Sammelzentrum;
    static void decode_get_parameter(Paket*);
    static void decode_get_daten(Paket*);
    static void decode_start_cont();
    static void decode_start_startstop();
    static void decode_get_status(Paket*);
    static char get_next_word(Paket*, std::string*);
    static float get_next_float(Paket* paket);
public:
    static void add_paket(Paket* paket);
    static void decoder_thread();
};
