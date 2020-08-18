#include "parameter.h"
#include <QtDebug>
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
    daten_mutex.lock();
//    qDebug()<<"adddatum nimmt daten";
    daten->push_back(new Daten(time,datum));
//    qDebug()<<"adddatum gibt daten";
    daten_mutex.unlock();
}

void Parameter::delete_daten()
{
    daten_mutex.lock();
 //   qDebug()<<"deletedaten nimmt daten";
    while (daten->size())
    {
        Daten* datum=daten->back();
        daten->pop_back();
        delete datum;
    }
//    qDebug()<<"deletedaten gibt daten";
    daten_mutex.unlock();
}

double ParameterViewer::get_data(uint32_t time)
{
    double wert=0;
    Daten* datum;
    parameter->daten_mutex.lock();
//    qDebug()<<"getdaten nimmt daten";
    uint32_t data_size=parameter->daten->size();
    if (index>time || index>=data_size)
        index=0;
    if (data_size==0)
    {
        parameter->daten_mutex.unlock();
        return 0;
    }
    do{
        datum=parameter->daten->at(index);
        if (datum->zeitpunkt >= time) break;
        index++;
    }while(index < data_size);
    wert=(double)(datum->messwert);
 //   qDebug()<<"getdaten gibt daten";
    parameter->daten_mutex.unlock();
    return(wert);
}

uint32_t ParameterViewer::newest()
{
    parameter->daten_mutex.lock();
  //  qDebug()<<"newest nimmt daten";
    uint32_t size=parameter->daten->size();
    if(size)
    {
        Daten* datum=parameter->daten->back();
        uint32_t newestime=datum->zeitpunkt;
   //     qDebug()<<"newest gibt daten";
        parameter->daten_mutex.unlock();
        return newestime;
    }
//    qDebug()<<"newest gibt daten";
    parameter->daten_mutex.unlock();
    return 0;
}

QString ParameterViewer::print()
{
    return(QString::fromStdString(parameter->name));
}
