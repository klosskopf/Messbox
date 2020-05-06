#ifndef RECHENBLOCK_H
#define RECHENBLOCK_H
#include <list>
#include <stdint.h>
class Node;

class Rechenblock
{
public:
    Rechenblock(int eingaengenr, uint32_t n_priority);
    virtual ~Rechenblock();
    const int eingaengenr;
    std::list<Rechenblock*> eingaenge;
    Rechenblock* ausgang;
    uint32_t ausgangsindex;
    const uint32_t priority;
    void von_oben(Rechenblock*,uint32_t);
    void von_unten(Rechenblock*);
    virtual double get_data(uint32_t time)=0;
    virtual uint32_t newest();
};
#endif // RECHENBLOCK_H
