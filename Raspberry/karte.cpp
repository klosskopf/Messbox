#include "karte.h"

Karte::Karte(mainWindow* gui, int n_index, std::string n_name)
    :index(n_index), name(n_name)
{
    karte_gui = new Karte_GUI(this);
    ListItem* listitem = new ListItem(index);
    listitem->setSizeHint(QSize(200,97));

    gui->parameterauswahl->addItem(listitem);
    gui->parameterauswahl->setItemWidget(listitem, karte_gui);
}


void Karte::add_parameter(Parameter* parameter)
{
    this->parameter.push_back(parameter);
    karte_gui->parameter->addItem(QString::fromUtf8(parameter->name.c_str()));
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
