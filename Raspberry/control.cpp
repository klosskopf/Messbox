#include "control.h"

void Control::control_thread(mainWindow* n_gui)
{
    gui=n_gui;
    std::this_thread::sleep_for(std::chrono::seconds(2));

    while(1)
    {
        check_karten();
        Post::send_get_daten(1,1);
        Post::send_get_status();


        std::this_thread::sleep_for(std::chrono::milliseconds(100));
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
        if(!Control::newkarte)
        {
            Post::send_get_parameter(index);
            Control::newkarte=true;
        }
    }
}

Karte* Control::findkarte(int karte)
{
    for (Karte* moeglichekarte : Kartenset)
    {
        if (moeglichekarte->index == karte)return moeglichekarte;
    }
    return NULL;
}

mainWindow* Control::gui;
std::list<Karte*> Control::Kartenset;
Modus Control::modus=STARTSTOP;
Zustand Control::zustand=STOP;
Rechenblock* Control::xAchse=new Axis_Block;
Rechenblock* Control::yAchse=new Axis_Block;
float Control::samplefreq=1000;
float Control::timeframe=1;
std::list<Kennliniendaten*> Control::kennlinie;
bool Control::newkarte=false;
float Control::vcc5V = -1;
float Control::vcc33V = -1;
float Control::icharge = -1;
float Control::vbat = -1;
float Control::vlade = -1;
float Control::vin = -1;
QMutex Control::datenmutex;
