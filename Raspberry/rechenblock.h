#ifndef RECHENBLOCK_H
#define RECHENBLOCK_H
#include <list>
#include <stdint.h>
#include <QString>
class Node;

class Rechenblock
{
public:
    Rechenblock(int eingaengenr, int n_priority);
    virtual ~Rechenblock();
    const int eingaengenr;
    std::list<Rechenblock*> eingaenge;
    Rechenblock* ausgang;
    int priority;
    int von_oben(Rechenblock*, Rechenblock* obererblock);
    int von_unten(Rechenblock*);
    virtual double get_data(uint32_t time)=0;
    virtual uint32_t newest();
    virtual QString print()=0;
};
#endif // RECHENBLOCK_H
