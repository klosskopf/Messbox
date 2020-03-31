#ifndef DATEN_H
#define DATEN_H
#include <stdint.h>

class Daten
{
public:
    Daten(uint32_t n_zeitpunkt, float n_messwert):zeitpunkt(n_zeitpunkt), messwert(n_messwert) {}
    virtual ~Daten(){}
    const uint32_t zeitpunkt;
    const float messwert;
};

class Kennliniendaten
{
public:
    Kennliniendaten(double n_x, double n_y):x(n_x), y(n_y) {}
    virtual ~Kennliniendaten(){}
    const double x;
    const double y;
};

#endif // DATEN_H
