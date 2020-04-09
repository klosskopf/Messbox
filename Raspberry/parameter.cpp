#include "parameter.h"
Parameter::Parameter(uint32_t n_nummer,bool n_f_nots, std::string n_name, Parametrierbar n_parametrierar, float n_min, float n_max)
    :Rechenblock(0), nummer(n_nummer), f_nots(n_f_nots), name(n_name), parametrierbar(n_parametrierar), min(n_min), max(n_max)
{
    auswahlliste = new std::list<std::string>;
    daten = new std::vector<Daten*>;
}

double Parameter::get_data(uint32_t time)
{   if(daten->size())
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
        return (double)(daten->at(time)->messwert);
    }
    return 0;
}

Parameter* Parameter::copy()
{
    Parameter* parameter=new Parameter(nummer,f_nots,name,parametrierbar,min,max);
    parameter->daten=daten;
    parameter->auswahlliste=auswahlliste;
    parameter->daten=daten;
    parameter->karte=karte;

    QLabel* label=new QLabel();
    QPixmap map = QPixmap(":images/const.png").scaled(30, 30);
    QPainter painter(&map);
    painter.setPen(Qt::black);
    painter.setFont(QFont("Arial", 10));
    painter.drawText(4,23,QString::number(karte->index).append(":").append(QString::number(nummer)));
    label->setPixmap(map);
    label->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    setup_Rechenblock(label);
    parameter->setToolTip(QString("Karte ").append(QString::number(karte->index)).append(" : ").append(QString::fromLocal8Bit(karte->name. c_str())).append(" : ").append(QString::fromLocal8Bit(name. c_str())));
    parameter->setup_Rechenblock(label);
    return parameter;
}

uint32_t Parameter::newest()
{
    uint32_t size=daten->size();
    if(size)
    {
        Daten* datum=daten->end()[-2];
        uint32_t newestime=datum->zeitpunkt;

     return newestime;
    }
    return 0;
}

void Parameter::add_auswahl(std::string auswahl)
{
    auswahlliste->push_back(auswahl);

}

void Parameter::add_datum(uint32_t time, float datum)
{
    daten->push_back(new Daten(time,datum));
}
