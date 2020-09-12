#include "control.h"
#include <QtDebug>
#include <QFileDialog>
#include <QDateTime>

void Control::control_thread(mainWindow* n_gui)
{
    gui=n_gui;

    std::this_thread::sleep_for(std::chrono::seconds(2));
    while(1)
    {
        if(zustand!=SAVE)
        {
            Post::briefkasten_mutex.lock();

            static uint32_t zaehler=0;
            if (zaehler++ == 10000)
            {
                gui->status_5V->setText(QString("5V: ") + QString::number(Control::vcc5V) + QString("V"));
                gui->status_3V3->setText(QString("3.3V: ") + QString::number(Control::vcc33V) + QString("V"));
                gui->status_vbat->setText(QString("Battery: ") + QString::number(Control::vbat) + QString("V"));
                zaehler=0;
                Post::send_get_status();
                check_karten();
            }

    //        qDebug()<<"Controlthread nimmt briefkasten";
            if (Post::Briefkasten.size() < 10)
            {
                gui->rechenfeld->rechenfeld_mutex.lock();
    //            qDebug()<<"Controllthread nimmt rechenfeld";
                if (gui->rechenfeld->activeparameter.size())
                {
                    Parameter* parameter=gui->rechenfeld->activeparameter.front();
                    gui->rechenfeld->activeparameter.pop_front();
                    Post::send_get_daten(parameter->karte->index,parameter->nummer,false);
                    gui->rechenfeld->activeparameter.push_back(parameter);
                }

     //           qDebug()<<"Controlthread gibt rechenfeld";
                gui->rechenfeld->rechenfeld_mutex.unlock();
            }
     //       qDebug()<<"Control gibt Briefkasten";
            Post::briefkasten_mutex.unlock();
        }
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
  //  if(Control::timeouttimer.isActive())
  //      timeouttimer.stop();
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

void Control::saveprocedure()
{
    zustand = SAVE;
    gui->savebutton->setEnabled(false);
    gui->startstopbutton->setEnabled(false);
    gui->modebutton->setEnabled(false);
    gui->sample->setEnabled(false);
    gui->rechenfeld->xfeld->setEnabled(false);
    gui->rechenfeld->yfeld->setEnabled(false);

    kartenset_mutex.lock();
    for (Karte* karte : Kartenset)
    {
        for(Parameter* currentparameter:*(karte->parameter))
        {
            if (currentparameter->f_nots)
            {
                last_paket_to_save_received=false;
                do
                {
                    Post::briefkasten_mutex.lock();
                    Post::send_get_daten(karte->index,currentparameter->nummer,true);
                    Post::briefkasten_mutex.unlock();

                    uint32_t size;
                    while(Post::Briefkasten.size())std::this_thread::sleep_for(std::chrono::microseconds(5));;
                }while(!last_paket_to_save_received);
            }
        }
    }
    QString dir = QFileDialog::getExistingDirectory(gui, "Open Directory","/home");

    if (dir!="")
    {
        QString timestamp=  QDateTime::currentDateTime().toString("dd.MM.yyyy_hh:mm:ss");
        for (Karte* karte : Kartenset)
        {
            for(Parameter* currentparameter:*(karte->parameter))
            {
                currentparameter->daten_mutex.lock();
                if (currentparameter->f_nots)
                {
                    QString pathname=dir + "/" + timestamp + "/\"" + QString::number(karte->index) + ":" + QString::fromStdString(karte->name) + "\"/";
                    QString filename="\"" + QString::fromStdString(currentparameter->name) + ".csv\"";
                    QDir dir;

                    if (!dir.exists(pathname))
                        dir.mkpath(pathname);

                    QFile file(pathname + filename);
                    if(file.open(QIODevice::WriteOnly))
                    {
                        QTextStream stream(&file);
                        for (Daten* datum:*(currentparameter->daten))
                        {
                            stream<<QString::number((datum->zeitpunkt)/samplefreq);
                            stream<<";";
                            stream<<QString::number(datum->messwert);
                            stream<<"\n";
                        }
                        file.close();
                    }
                }
                currentparameter->daten_mutex.unlock();
            }
        }
    }


    kartenset_mutex.unlock();

    gui->startstopbutton->setEnabled(true);
    gui->modebutton->setEnabled(true);
    gui->timeframe->setEnabled(true);
    gui->sample->setEnabled(true);
    gui->rechenfeld->xfeld->setEnabled(true);
    gui->rechenfeld->yfeld->setEnabled(true);
    zustand = STOP;
    gui->savebutton->setEnabled(true);
}

mainWindow* Control::gui;
std::list<Karte*> Control::Kartenset;
QMutex Control::kartenset_mutex;
Modus Control::modus=CONT;
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
bool Control::last_paket_to_save_received=false;
