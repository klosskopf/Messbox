#pragma once

#include "karte.h"
#include <list>

enum Befehl {COM_GET_PARAMETER=0x01, COM_SET_PARAMETER=0x02, COM_GET_DATEN=0x03, COM_START_KONT=0x04, COM_START_STARTSTOP=0x05, COM_STOP=0x06, COM_SET_SAMPLE_FREQ=0x07, COM_GET_STATUS=0x08};

class Paket
{
public:
    Paket();
    virtual ~Paket();
    int empfaengerindex;
    Befehl befehl;
    uint32_t laenge;
    uint32_t ausgewaertet;
    uint8_t * daten=NULL;
};
