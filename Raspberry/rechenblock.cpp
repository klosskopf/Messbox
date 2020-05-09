#include "rechenblock.h"
#include <QtWidgets>
#include "control.h"

Rechenblock::Rechenblock(int n_eingaengenr, int n_priority) : eingaengenr(n_eingaengenr), ausgang(NULL), priority(n_priority)
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

int Rechenblock::von_oben(Rechenblock* neuerblock,Rechenblock* obererblock)
{
    if (neuerblock->priority==-1 && priority==1)
    {
        priority=5;
        Rechenfeld::activeblock=this;
        return(0);
    }
    else if(neuerblock->priority >= priority)
    {
        obererblock->ausgang=neuerblock;
        neuerblock->eingaenge.push_back(obererblock);
        neuerblock->ausgang=this;
        std::replace (eingaenge.begin(), eingaenge.end(), obererblock, neuerblock);
        Rechenfeld::activeblock=neuerblock;
        return(0);
    }
    else if (ausgang)
    {
        return(ausgang->von_oben(neuerblock,this));
    }
    else
    {
        return(-1);
    }
}

int Rechenblock::von_unten(Rechenblock* neuerblock)
{
    if (neuerblock->priority==-1 && priority==1)
    {
        priority=5;
        Rechenfeld::activeblock=this;
        return(0);
    }
    else if(neuerblock->priority >= priority || neuerblock->priority==1)
    {
        neuerblock->ausgang=this;
        eingaenge.push_back(neuerblock);
        Rechenfeld::activeblock=neuerblock;
        return(0);
    }
    else if (ausgang)
    {
        return(ausgang->von_oben(neuerblock,this));
    }
    else
    {
        return(-1);
    }
}
