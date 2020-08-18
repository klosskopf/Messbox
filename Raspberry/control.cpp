#include "control.h"
#include <QtDebug>
void Control::control_thread(mainWindow* n_gui)
{
    gui=n_gui;

    std::this_thread::sleep_for(std::chrono::seconds(2));
    while(1)
    {
        check_karten();
      //  Post::send_get_status();
        Post::briefkasten_mutex.lock();
//        qDebug()<<"Controlthread nimmt briefkasten";
        if (Post::Briefkasten.size() < 10)
        {
            gui->rechenfeld->rechenfeld_mutex.lock();
//            qDebug()<<"Controllthread nimmt rechenfeld";
            if (gui->rechenfeld->activeparameter.size())
            {
                Parameter* parameter=gui->rechenfeld->activeparameter.front();
                gui->rechenfeld->activeparameter.pop_front();
                Post::send_get_daten(parameter->karte->index,parameter->nummer);
                gui->rechenfeld->activeparameter.push_back(parameter);
            }

 //           qDebug()<<"Controlthread gibt rechenfeld";
            gui->rechenfeld->rechenfeld_mutex.unlock();
        }
 //       qDebug()<<"Control gibt Briefkasten";
        Post::briefkasten_mutex.unlock();
        std::this_thread::sleep_for(std::chrono::microseconds(1));
    }
}

void Control::start()
{
    gui->graphersteller->clear_graph();
    zustand = MESS;
    for (Karte* karte : Kartenset)
    {
        for (Parameter* param : *(karte->parameter))
        {
            param->delete_daten();
        }
    }
    Gpio::set_led(LED_RUN);
    if (modus==CONT)
        Post::send_start_kont();
    else
    {
        Post::send_start_startstop();
        timeouttimer.singleShot(timeframe*1000,stop);
        //timeouttimer.start(Control::timeframe*1000);
    }
    gui->startstopbutton->setText("misst...");
    gui->startstopbutton->setStyleSheet("background-color: rgb(255, 0, 0); color: rgb(0, 0, 0)");
}

void Control::stop()
{
    //if(Control::timeouttimer.isActive())
        //timeouttimer.stop();
    zustand = STOP;
    Post::send_stop();
    Gpio::set_led(LED_STOP);
    gui->startstopbutton->setText("Stop");
    gui->startstopbutton->setStyleSheet("background-color: rgb(0, 255, 0); color: rgb(0, 0, 0)");
}

void Control::check_karten()
{
    std::list<int> n_Karten = Gpio::get_new_karten();

    kartenset_mutex.lock();
//    qDebug()<<"checkkarten nimmt kartenset";
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
            qDebug("Ausgesteckt:%d",(*karte)->index);
            delete *karte;
            karte=Kartenset.erase(karte);
            gui->rechenfeld->update();

        }
        else
        {
            ++karte;
        }
    }
 //   qDebug()<<"Checkkarten gibt kartenset";
    kartenset_mutex.unlock();

    for (int index : n_Karten)
    {
        if(!Control::newkarte)
        {
            qDebug("Eingesteckt:%d",index);
            Post::send_get_parameter(index);
            Control::newkarte=true;
        }
    }
}

Karte* Control::findkarte(int karte)
{
    for (Karte* moeglichekarte : Kartenset)
    {
        if (moeglichekarte->index == karte)
        {
            return moeglichekarte;
        }
    }
    return NULL;
}

mainWindow* Control::gui;
std::list<Karte*> Control::Kartenset;
QMutex Control::kartenset_mutex;
Modus Control::modus=STARTSTOP;
Zustand Control::zustand=STOP;
Rechenblock* Control::xAchse=new Axis_Block;
Rechenblock* Control::yAchse=new Axis_Block;
float Control::samplefreq=1000;
float Control::timeframe=1;
//std::list<Kennliniendaten*> Control::kennlinie;
bool Control::newkarte=false;
float Control::vcc5V = -1;
float Control::vcc33V = -1;
float Control::icharge = -1;
float Control::vbat = -1;
float Control::vlade = -1;
float Control::vin = -1;
