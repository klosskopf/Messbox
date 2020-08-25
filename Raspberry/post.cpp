#include "post.h"
#include <QtDebug>
#include <wiringPiSPI.h>

void Post::spi_thread()
{
    Paket* currentpaket;
    wiringPiSPISetup (0, DATA_BAUD);
    while(1)
    {
        briefkasten_mutex.lock();
//        qDebug()<<"post nimmt briefkasten";
        if (Briefkasten.size())
        {
            currentpaket=Briefkasten.front();
            Briefkasten.pop_front();
//            qDebug()<<"post gibt briefkasten";
            briefkasten_mutex.unlock();
            switch(currentpaket->befehl)
            {
            case COM_GET_PARAMETER:
            {
                std::this_thread::sleep_for(std::chrono::seconds(1));
                uint8_t ask_for[5]={(uint8_t)COM_GET_PARAMETER,0,0,0,0};
                Gpio::enable_slave(currentpaket->empfaengerindex);
                wiringPiSPIDataRW (0, ask_for, 5) ;
                currentpaket->laenge = ((uint32_t)ask_for[1]) + ((uint32_t)ask_for[2]<<8) +((uint32_t)ask_for[3]<<16) + ((uint32_t)ask_for[4]<<24);
                if (currentpaket->laenge < 20000 && currentpaket->laenge!=0)
                {
                    wiringPiSPIDataRW (0, currentpaket->daten, currentpaket->laenge);
                }
                else
                {
                    currentpaket->laenge=0;
                }
                Gpio::disable_slave(currentpaket->empfaengerindex);
                break;
            }
            case COM_SET_PARAMETER:
            {
                uint8_t ask_for[5];
                ask_for[0]=(uint8_t)COM_SET_PARAMETER;
                ask_for[4]=(currentpaket->laenge)>>24;
                ask_for[3]=(currentpaket->laenge)>>16;
                ask_for[2]=(currentpaket->laenge)>>8;
                ask_for[1]=currentpaket->laenge;
                Gpio::enable_slave(currentpaket->empfaengerindex);
                wiringPiSPIDataRW (0, ask_for, 5);
                wiringPiSPIDataRW (0, currentpaket->daten, currentpaket->laenge);
                Gpio::disable_slave(currentpaket->empfaengerindex);
                break;
            }
            case COM_GET_DATEN:
            {
                uint32_t parameternummer;
                memcpy(&parameternummer,currentpaket->daten,4);
                uint8_t befehl=COM_GET_DATEN;
                Gpio::enable_slave(currentpaket->empfaengerindex);
                wiringPiSPIDataRW(0,&befehl,1);
                wiringPiSPIDataRW (0, (uint8_t*)&parameternummer, 4);
                std::this_thread::sleep_for(std::chrono::microseconds(2000));        //The STM32 takes up to 900us to check the flash metadata
                wiringPiSPIDataRW (0, (uint8_t*)&(currentpaket->laenge), 4);
                if (currentpaket->laenge < 5000)
                {
                    //Spi::txrx(currentpaket->daten, currentpaket->laenge + 4);
                    if (currentpaket->laenge > 3000)
                    {
                        wiringPiSPIDataRW (0, currentpaket->daten+4, 3000) ;
                        wiringPiSPIDataRW (0, currentpaket->daten+3004, currentpaket->laenge - 3000 + 4) ;
                    }
                    else
                    {
                        wiringPiSPIDataRW (0, currentpaket->daten+4, currentpaket->laenge + 4);
                    }
                }
                else {
                    currentpaket->laenge=0;
                }
                Gpio::disable_slave(currentpaket->empfaengerindex);
                break;
            }
            case COM_START_KONT:
            {
                uint8_t ask_for[1];
                ask_for[0]=(uint8_t)COM_START_KONT;
                Gpio::enable_slave(currentpaket->empfaengerindex);
                wiringPiSPIDataRW (0, ask_for, 1);
                Gpio::disable_slave(currentpaket->empfaengerindex);
                break;
            }
            case COM_START_STARTSTOP:
            {
                uint8_t ask_for[1];
                ask_for[0]=(uint8_t)COM_START_STARTSTOP;
                Gpio::enable_slave(currentpaket->empfaengerindex);
                wiringPiSPIDataRW (0, ask_for, 1);
                Gpio::disable_slave(currentpaket->empfaengerindex);
                break;
            }
            case COM_STOP:
            {
                uint8_t ask_for[1];
                ask_for[0]=(uint8_t)COM_STOP;
                Gpio::enable_slave(currentpaket->empfaengerindex);
                wiringPiSPIDataRW (0, ask_for, 1);
                Gpio::disable_slave(currentpaket->empfaengerindex);
                break;
            }
            case COM_SET_SAMPLE_FREQ:
            {
                uint8_t ask_for[5];
                ask_for[0]=(uint8_t)COM_SET_SAMPLE_FREQ;
                ask_for[1]=currentpaket->daten[0];
                ask_for[2]=currentpaket->daten[1];
                ask_for[3]=currentpaket->daten[2];
                ask_for[4]=currentpaket->daten[3];
                Gpio::enable_slave(currentpaket->empfaengerindex);
                wiringPiSPIDataRW (0, ask_for, 5);
                Gpio::disable_slave(currentpaket->empfaengerindex);
                break;
            }
            case COM_GET_STATUS:
            {
                uint8_t ask_for[1]={COM_GET_STATUS};
                Gpio::enable_slave(currentpaket->empfaengerindex);
                wiringPiSPIDataRW (0, ask_for, 1);
                wiringPiSPIDataRW (0, currentpaket->daten, 24);
                Gpio::disable_slave(currentpaket->empfaengerindex);
                break;
            }
            default:
            {
                break;
            }
            }
            Decoder::add_paket(currentpaket);
        }
        else
        {
//            qDebug()<<"post gibt briefkasten";
            briefkasten_mutex.unlock();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}
void Post::send_get_parameter(int index)
{
    Paket* getparameterpaket = new Paket();
    getparameterpaket->befehl=COM_GET_PARAMETER;
    getparameterpaket->empfaengerindex=index;
    Briefkasten.push_back(getparameterpaket);
}
void Post::send_set_parameter(int index, uint32_t nummer, std::string wert)
{
    Paket* setparameterpaket = new Paket();
    setparameterpaket->laenge = 4 + wert.length();
    setparameterpaket->befehl=COM_SET_PARAMETER;
    setparameterpaket->daten[3]= nummer>>24;
    setparameterpaket->daten[2]= nummer>>16;
    setparameterpaket->daten[1]= nummer>>8;
    setparameterpaket->daten[0]= nummer;
    for(uint32_t i=0;i<wert.length();i++)setparameterpaket->daten[i+4]= wert.at(i);
    setparameterpaket->empfaengerindex=index;
    Briefkasten.push_back(setparameterpaket);
}
void Post::send_get_daten(int index, uint32_t nummer, bool savepaket)
{
    Paket* getdatenpaket = new Paket();
    getdatenpaket->savepaket=savepaket;
    getdatenpaket->befehl=COM_GET_DATEN;
    getdatenpaket->daten[3]= nummer>>24;
    getdatenpaket->daten[2]= nummer>>16;
    getdatenpaket->daten[1]= nummer>>8;
    getdatenpaket->daten[0]= nummer;
    getdatenpaket->empfaengerindex=index;
    Briefkasten.push_back(getdatenpaket);
}
void Post::send_start_kont()
{
    Paket* startkontpaket = new Paket();
    startkontpaket->befehl=COM_START_KONT;
    startkontpaket->empfaengerindex=-1;
    Briefkasten.push_back(startkontpaket);
}
void Post::send_start_startstop()
{
    Paket* startstartstoppaket = new Paket();
    startstartstoppaket->befehl=COM_START_STARTSTOP;
    startstartstoppaket->empfaengerindex=-1;
    Briefkasten.push_back(startstartstoppaket);
}
void Post::send_stop()
{
    Paket* stoppaket = new Paket();
    stoppaket->befehl=COM_STOP;
    stoppaket->empfaengerindex=-1;
    Briefkasten.push_back(stoppaket);
}
void Post::send_set_sample_freq(float freq)
{
    union {
        float asfloat;
        uint8_t asbytes[4];
      } frequency;

    frequency.asfloat=freq;

    Paket* setsamplefreqpaket = new Paket();
    setsamplefreqpaket->befehl=COM_SET_SAMPLE_FREQ;
    setsamplefreqpaket->daten[0]=frequency.asbytes[0];
    setsamplefreqpaket->daten[1]=frequency.asbytes[1];
    setsamplefreqpaket->daten[2]=frequency.asbytes[2];
    setsamplefreqpaket->daten[3]=frequency.asbytes[3];
    setsamplefreqpaket->empfaengerindex=-1;
    Briefkasten.push_back(setsamplefreqpaket);
}
void Post::send_get_status()
{
    Paket* getstatuspaket = new Paket();
    getstatuspaket->befehl=COM_GET_STATUS;
    getstatuspaket->empfaengerindex=0;
    Briefkasten.push_back(getstatuspaket);
}

std::list<Paket*> Post::Briefkasten;
QMutex Post::briefkasten_mutex;
