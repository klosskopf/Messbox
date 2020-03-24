#include "parameter.h"
Parameter::Parameter(bool n_f_nots, std::string n_name, Parametrierbar n_parametrierar, float n_min, float n_max)
    :f_nots(n_f_nots), name(n_name), parametrierbar(n_parametrierar), min(n_min), max(n_max)
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
    float lasttime = daten.front()->zeitpunkt;
    float lastwert = daten.front()->messwert;
    for (Daten* datum : daten)
    {
        if (datum->zeitpunkt > time)
        {
            return (lastwert*(lasttime-time) + datum->messwert * (time - datum->zeitpunkt) )/(lasttime - datum->zeitpunkt);
        }
    }
    return lastwert;
}

void Parameter::add_auswahl(std::string auswahl)
{
    auswahlliste.push_back(auswahl);

}

void Parameter::add_datum(float time, float datum)
{
    daten.push_back(new Daten(time,datum));
}
