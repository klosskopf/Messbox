#include "karte.h"

Karte::Karte(mainWindow* n_gui, int n_index, std::string n_name, std::list<Parameter*> *n_parameter)
    : gui(n_gui), index(n_index), name(n_name), parameter(n_parameter)
{
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
    return new Parameter(false,"",NEIN);
}
