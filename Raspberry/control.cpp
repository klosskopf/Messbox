#include "control.h"

void Control::controlThread(mainWindow* n_gui)
{
    gui=n_gui;
    std::list<Parameter*> *parameterliste = new std::list<Parameter*>;
    Parameter* parameter = new Parameter(true,"Spannung",LISTE);
    parameter->add_auswahl("5");
    parameter->add_auswahl("3,3");
    parameterliste->push_back(parameter);
    parameter= new Parameter(true,"Frequenz",FREI);
    parameter->add_auswahl("1000");
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
    parameter = new Parameter(true,"Spannung",NEIN);
    parameterliste->push_back(parameter);
    parameter = new Parameter(true,"RMS",NEIN);
    parameterliste->push_back(parameter);
    karte = new Karte(gui, 2, "Spannungsmesser",parameterliste);
    Kartenset.push_back(karte);

    parameterliste = new std::list<Parameter*>;
    parameter = new Parameter(true,"Temperatur",NEIN);
    parameterliste->push_back(parameter);
    parameter = new Parameter(true,"R0",FREI);
    parameter->add_auswahl("10000");
    parameter->add_auswahl("1000");
    parameterliste->push_back(parameter);
    parameter = new Parameter(true,"alpha",FREI);
    parameter->add_auswahl("1");
    parameterliste->push_back(parameter);
    parameter = new Parameter(true,"beta",FREI);
    parameter->add_auswahl("1");
    parameterliste->push_back(parameter);
    karte = new Karte(gui, 3, "Temperaturmesser",parameterliste);
    Kartenset.push_back(karte);
    std::this_thread::sleep_for(std::chrono::seconds(5));
    while(1)
    {
        check_karten();
    }

}

void Control::check_karten()
{
    std::list<int> n_Karten = Gpio::get_new_karten();

    std::list<Karte*>::iterator karte = Kartenset.begin();
    while (karte != Kartenset.end())
    {
        bool found=false;
        for (int index : n_Karten)
        {
            if ((*karte)->index==index)
            {
                found=true;
                n_Karten.remove(index);
                break;
            }
        }
        if (!found)
        {
            delete *karte;
            karte=Kartenset.erase(karte);
        }
        else
        {
            ++karte;
        }
    }

    for (int index : n_Karten)
    {
        Post::send_get_parameter(index);
    }
}

mainWindow* Control::gui;
std::list<Karte*> Control::Kartenset;
Modus Control::modus=STARTSTOP;
Zustand Control::zustand=STOP;
Rechenblock* Control::xAchse=NULL;
Rechenblock* Control::yAchse=NULL;
std::list<Daten*> Control::kennlinie;
