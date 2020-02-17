#ifndef RECHENBLOCK_H
#define RECHENBLOCK_H
#include <list>

class Rechenblock
{
public:
    Rechenblock();
    std::list<Rechenblock*> eingaenge;
    virtual float get_data(float time)=0;
};

#endif // RECHENBLOCK_H
