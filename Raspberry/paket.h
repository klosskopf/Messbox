#ifndef PAKET_H
#define PAKET_H

#include "karte.h"
#include <list>

enum Befehl {GET_PARAMETER, SET_PARAMETER, GET_DATEN, START_KONT, START_STARTSTOP};

class Paket
{
public:
    Paket();
    Karte* empfaenger;
    Befehl befehl;
    int laenge;
    std::list<char> daten;
};

#endif // PAKET_H
