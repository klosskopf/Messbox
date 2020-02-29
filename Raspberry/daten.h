#ifndef DATEN_H
#define DATEN_H

class Daten
{
public:
    Daten(float n_zeitpunkt, float n_messwert):zeitpunkt(n_zeitpunkt), messwert(n_messwert) {};
    virtual ~Daten(){}
    const float zeitpunkt;
    const float messwert;
};

#endif // DATEN_H
