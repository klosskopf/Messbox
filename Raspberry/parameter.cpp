#include "parameter.h"
#include <iostream>
Parameter::Parameter(uint32_t n_nummer,bool n_f_nots, std::string n_name, Parametrierbar n_parametrierar, float n_min, float n_max)
    :nummer(n_nummer), f_nots(n_f_nots), name(n_name), parametrierbar(n_parametrierar), min(n_min), max(n_max)
{
    auswahlliste = new std::list<std::string>;
    daten = new std::vector<Daten*>;
}

void Parameter::add_auswahl(std::string auswahl)
{
    auswahlliste->push_back(auswahl);

}

void Parameter::add_datum(uint32_t time, float datum)
{
    daten->push_back(new Daten(time,datum));
}

double ParameterViewer::get_data(uint32_t time)
{   if(parameter->daten->size())
    {

        //float lasttime = daten->front()->zeitpunkt;
       // float lastwert = daten->front()->messwert;

       /* for (Daten* datum : *daten)
        {
            if (datum->zeitpunkt > time)
            {
               // return (lastwert*(lasttime-time) + datum->messwert * (time - datum->zeitpunkt) )/(double)(datum->zeitpunkt - lasttime);
                return (double)datum->messwert;
            }
            lasttime=datum->zeitpunkt;
            lastwert=datum->messwert;
        }
        return (double)lastwert;*/
        return (double)(parameter->daten->at(time)->messwert);
    }
    return 0;
}

uint32_t ParameterViewer::newest()
{
    uint32_t size=parameter->daten->size();
    if(size)
    {
        Daten* datum=parameter->daten->end()[-2];
        uint32_t newestime=datum->zeitpunkt;

     return newestime;
    }
    return 0;
}

QString ParameterViewer::print()
{
    return("parameter->name");
}
