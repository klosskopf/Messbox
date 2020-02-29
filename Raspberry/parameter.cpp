#include "parameter.h"
Parameter::Parameter(bool n_f_nots, std::string n_name, Parametrierbar n_parametrierar)
    :f_nots(n_f_nots), name(n_name), parametrierbar(n_parametrierar)
{

}

Parameter::~Parameter()
{
    for (Daten* datum : daten)
    {
        delete datum;
    }
}


float Parameter::get_data(float time)
{
    return time*time;
}

void Parameter::add_auswahl(std::string auswahl)
{
    auswahlliste.push_back(auswahl);

}
