#ifndef KARTE_H
#define KARTE_H
#include <QString>
#include <QObject>
#include <list>
#include "parameter.h"
#include "mainwindow.h"
#include "parameterauswahl.h"
#include "karte_gui.h"
class mainWindow;
class Karte_GUI;
class Parameterauswahl;

class Karte : public QObject
{
    Q_OBJECT
public:
    Karte(mainWindow* gui, int n_index, std::string n_name, std::list<Parameter*> *n_parameter);
    virtual ~Karte();
    mainWindow* gui;
    Karte_GUI* karte_gui;
    int index;
    std::string name;
    std::list<Parameter*> *parameter;
    Parameter* find_parameter(std::string name);
    Parameter* find_parameter(uint32_t nummer);
signals:
    void let_create_karte_gui(Karte* karte);
    void let_delete_karte_gui(Karte_GUI* karte_gui);
};

#endif // KARTE_H
