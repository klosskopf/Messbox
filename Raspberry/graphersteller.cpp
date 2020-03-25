#include "graphersteller.h"

Graphersteller::Graphersteller(QObject *parent) : QObject(parent)
{

}

void Graphersteller::draw()
{
    emit create_graph();
}
