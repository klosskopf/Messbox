#ifndef RECHENBLOCK_H
#define RECHENBLOCK_H
#include <list>
#include <stdint.h>
class Rechenblock
{
public:
    Rechenblock();
    std::list<Rechenblock*> eingaenge;
    virtual float get_data(uint32_t time)=0;
    virtual uint32_t newest()=0;
};

#endif // RECHENBLOCK_H
