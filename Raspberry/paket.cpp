#include "paket.h"

Paket::Paket()
{
    ausgewaertet=0;
    daten=NULL;
}

Paket::~Paket()
{
    if (daten!=NULL) delete [] daten;
}
