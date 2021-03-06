#include "karte.h"
#include "parameterauswahl.h"
#include <QString>
#include <QObject>
#include <list>
#include "parameter.h"
#include "mainwindow.h"
#include "karte_gui.h"

Karte::Karte(mainWindow* n_gui, int n_index, std::string n_name, std::list<Parameter*> *n_parameter)
    : gui(n_gui), index(n_index), name(n_name), parameter(n_parameter)
{
    for (Parameter* param : *parameter)
    {
        param->karte=this;
    }
    karte_gui=NULL;
    QObject::connect(this, &Karte::let_delete_karte_gui, gui->parameterauswahl, &Parameterauswahl::delete_karte);
    QObject::connect(this, &Karte::let_create_karte_gui, gui->parameterauswahl, &Parameterauswahl::create_karte);
    emit let_create_karte_gui(this);
}

Karte::~Karte()
{
    if (karte_gui)emit let_delete_karte_gui(karte_gui);
    for (Parameter* param : *parameter)
    {
        delete param->auswahlliste;
        for (Daten* datum : *param->daten)
        {
            delete datum;
        }
        delete param;
    }
}

Parameter* Karte::find_parameter(std::string name)
{
    for(Parameter* param : *parameter)
    {
        if(param->name==name)
            return param;
    }
    return NULL;
}

Parameter* Karte::find_parameter(uint32_t nummer)
{
    for(Parameter* param : *parameter)
    {
        if(param->nummer==nummer)
            return param;
    }
    return NULL;
}
