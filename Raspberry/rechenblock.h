#ifndef RECHENBLOCK_H
#define RECHENBLOCK_H
#include <list>
#include <stdint.h>
#include <QLabel>
#include <QPushButton>
#include <QGridLayout>
#include <QString>
class Node;

class Rechenblock
{
    Q_OBJECT
public:
    Rechenblock(int eingaengenr);
    virtual ~Rechenblock();
    const int eingaengenr;
    std::list<Rechenblock*> eingaenge;
    virtual double get_data(uint32_t time)=0;
    virtual uint32_t newest();
};
#endif // RECHENBLOCK_H
