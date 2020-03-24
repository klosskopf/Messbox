#ifndef PAKET_H
#define PAKET_H

#include "karte.h"
#include <list>

enum Befehl {GET_PARAMETER=0x01, SET_PARAMETER=0x02, GET_DATEN=0x03, START_KONT=0x06, START_STARTSTOP=0x07, SET_SAMPLE_FREQ=0x08, GET_STATUS=0x09};

class Paket
{
public:
    Paket();
    virtual ~Paket();
    uint8_t empfaengerindex;
    Befehl befehl;
    uint32_t laenge;
    uint32_t ausgewaertet;
    uint8_t * daten;
};

#endif // PAKET_H
