#include "rechenblock.h"
#include <QtWidgets>
#include "control.h"

Rechenblock::Rechenblock(int n_eingaengenr, uint32_t n_priority) : eingaengenr(n_eingaengenr), priority(n_priority)
{

}

Rechenblock::~Rechenblock()
{
}

uint32_t Rechenblock::newest()
{
    uint32_t newest=-1;
    for (Rechenblock* block : eingaenge)
    {
        if (block->newest() < newest)
            newest=block->newest();
    }
    if (eingaenge.size()==0) newest=0;
    return newest;
}

void Rechenblock::von_oben(Rechenblock* neuerblock,uint32_t eingangsindex)
{

}

void Rechenblock::von_unten(Rechenblock* neuerblock)
{

}
