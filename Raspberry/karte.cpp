#include "karte.h"

Karte::Karte(mainWindow* n_gui, int n_index, std::string n_name)
    : gui(n_gui), index(n_index), name(n_name)
{

}


void Karte::add_parameter(Parameter* parameter)
{
    this->parameter.push_back(parameter);
    gui->parameterauswahl->addparameter(name,parameter->name);
    //karte_gui->parameter->addItem(QString::fromUtf8(parameter->name.c_str()));
}

Parameter* Karte::find_parameter(std::string name)
{
    for(Parameter* param : parameter)
    {
        if(param->name==name)
            return param;
    }
    return new Parameter(false,"",NEIN);
}
