#include "karte.h"

Karte::Karte(Parameterauswahl* parameterliste, int n_index, std::string n_name)
    :index(n_index), name(n_name)
{
    parameterliste->add_karte(this);
}
