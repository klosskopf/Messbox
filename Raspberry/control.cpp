#include "control.h"

void Control::controlThread(mainWindow* gui)
{

    std::list<Parameter*> *parameterliste = new std::list<Parameter*>;
    Parameter* parameter= new Parameter(true,"Frequenz",FREI);
    parameterliste->push_back(parameter);
    parameter= new Parameter(false,"Form",LISTE);
    parameter->add_auswahl("Rechteck");
    parameter->add_auswahl("Sägezahn");
    parameter->add_auswahl("Dreieck");
    parameter->add_auswahl("Sinus");
    parameter->add_auswahl("Stufen");
    parameterliste->push_back(parameter);
    Karte* karte = new Karte(gui, 1, "Spannungsquelle",parameterliste);
    Kartenset.push_back(karte);

    parameterliste = new std::list<Parameter*>;
    parameter = new Parameter(true,"Spannung",LISTE);
    parameter->add_auswahl("5");
    parameter->add_auswahl("3,3");
    parameterliste->push_back(parameter);
    parameter = new Parameter(true,"strom",FREI);
    parameter->add_auswahl("1");
    parameter->add_auswahl("2");
    parameter->add_auswahl("3");
    parameterliste->push_back(parameter);
    karte = new Karte(gui, 2, "Spannungsmesser",parameterliste);
    Kartenset.push_back(karte);
    for (Karte* karte : Kartenset)
    {
        delete karte;
    }
    while(1)
    {

    }

}


std::list<Karte*> Control::Kartenset;
Modus Control::modus=STARTSTOP;
Zustand Control::zustand=STOP;
Rechenblock* Control::xAchse=NULL;
Rechenblock* Control::yAchse=NULL;
std::list<Daten*> Control::kennlinie;
