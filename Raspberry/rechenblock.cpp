#include "rechenblock.h"
#include <QtWidgets>
#include "control.h"

Rechenblock::Rechenblock(int n_eingaengenr) : eingaengenr(n_eingaengenr)
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
