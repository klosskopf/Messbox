#ifndef KARTE_H
#define KARTE_H
#include <QString>
#include <list>
#include "parameter.h"
#include "parameterauswahl.h"
class Parameterauswahl;

class Karte
{
public:
    Karte(Parameterauswahl* parameterliste, int n_index, std::string n_name);
    std::list<Parameter*> parameter;
    int index;
    std::string name;
};

#endif // KARTE_H
