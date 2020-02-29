#ifndef KARTE_H
#define KARTE_H
#include <QString>
#include <list>
#include "parameter.h"
#include "mainwindow.h"
#include "parameterauswahl.h"
#include "karte_gui.h"
class mainWindow;
class Karte_GUI;
class Parameterauswahl;

class Karte
{
public:
    Karte(mainWindow* gui, int n_index, std::string n_name);
    mainWindow* gui;
    std::list<Parameter*> parameter;
    int index;
    std::string name;
    void add_parameter(Parameter* parameter);
    Parameter* find_parameter(std::string name);
};

#endif // KARTE_H
