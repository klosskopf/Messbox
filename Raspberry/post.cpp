#include "post.h"


void Post::send_get_parameter(int index)
{
    std::list<Parameter*> *parameterliste = new std::list<Parameter*>;
    Parameter* parameter= new Parameter(true,"Begrenzer Wert",FREI);
    parameterliste->push_back(parameter);
    parameter = new Parameter(true,"Leistung",NEIN);
    parameterliste->push_back(parameter);
    parameter= new Parameter(true,"Spannung",NEIN);
    parameterliste->push_back(parameter);
    parameter= new Parameter(true,"Strom",NEIN);
    parameterliste->push_back(parameter);
    parameter= new Parameter(false,"Begrenzung",LISTE);
    parameter->add_auswahl("Spannung");
    parameter->add_auswahl("Strom");
    parameter->add_auswahl("Leistung");
    parameterliste->push_back(parameter);
    Karte* karte = new Karte(Control::gui, index, "Elektronische Last",parameterliste);
    Control::Kartenset.push_back(karte);
}


std::list<Paket*> Post::Briefkasten;
