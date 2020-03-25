#include "control.h"

static Graphersteller graphersteller;

void Control::control_thread(mainWindow* n_gui)
{
    gui=n_gui;
    QObject::connect(&graphersteller,&Graphersteller::create_graph,gui,&mainWindow::draw_graph);
    xAchse=new Time();
    yAchse=new Time();
    std::this_thread::sleep_for(std::chrono::seconds(2));

    while(1)
    {
        check_karten();
        create_kennlinie();
        graphersteller.draw();
        Post::send_get_daten(1,1);

        if (findkarte(1))
        {
            xAchse=findkarte(1)->find_parameter(1);
        }
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

void Control::create_kennlinie()
{
    if(xAchse && yAchse)
    {
        uint32_t newestime=(xAchse->newest() > yAchse->newest()) ? yAchse->newest() :xAchse->newest();
        uint32_t number = samplefreq * timeframe;
        for (Kennliniendaten* datum : kennlinie)
        {
            delete datum;
        }
        kennlinie.clear();
        for (uint32_t time=newestime; time>(newestime-number); time--)
        {
            float xvalue=xAchse->get_data(time);
            float yvalue=yAchse->get_data(time);
            kennlinie.push_back(new Kennliniendaten(xvalue, yvalue));
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
Rechenblock* Control::xAchse=NULL;
Rechenblock* Control::yAchse=NULL;
float Control::samplefreq=1;
float Control::timeframe=1000;
std::list<Kennliniendaten*> Control::kennlinie;
bool Control::newkarte=false;
float Control::vcc5V = -1;
float Control::vcc33V = -1;
float Control::icharge = -1;
float Control::vbat = -1;
float Control::vlade = -1;
float Control::vin = -1;
