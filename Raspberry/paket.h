#pragma once

#include "karte.h"
#include <list>

enum Befehl {GET_PARAMETER=0x01, SET_PARAMETER=0x02, GET_DATEN=0x03, START_KONT=0x04, START_STARTSTOP=0x05, SET_SAMPLE_FREQ=0x06, GET_STATUS=0x07};

class Paket
{
public:
    Paket();
    virtual ~Paket();
    int empfaengerindex;
    Befehl befehl;
    uint32_t laenge;
    uint32_t ausgewaertet;
    uint8_t * daten;
};
