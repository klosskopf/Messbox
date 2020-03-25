#ifndef DATEN_H
#define DATEN_H
#include <stdint.h>

class Daten
{
public:
    Daten(uint32_t n_zeitpunkt, float n_messwert):zeitpunkt(n_zeitpunkt), messwert(n_messwert) {};
    virtual ~Daten(){}
    const uint32_t zeitpunkt;
    const float messwert;
};

#endif // DATEN_H
