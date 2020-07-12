#include "post.h"

std::list<Paket*> Post::Briefkasten;

void Post::spi_thread()
{
    Paket* currentpaket;
    Spi::init_spi();
    while(1)
    {
        if (Briefkasten.size())
        {
            currentpaket=Briefkasten.front();
            switch(currentpaket->befehl)
            {
            case GET_PARAMETER:
            {
                uint8_t ask_for[5]={(uint8_t)GET_PARAMETER,0,0,0,0};
                Gpio::enable_slave(currentpaket->empfaengerindex);
                Spi::txrx(ask_for, 5);
                currentpaket->laenge = ((uint32_t)ask_for[1]) + ((uint32_t)ask_for[2]<<8) +((uint32_t)ask_for[3]<<16) + ((uint32_t)ask_for[4]<<24);
                if (currentpaket->laenge < 20000 && currentpaket->laenge!=0)
                {
                    currentpaket->daten = new uint8_t[currentpaket->laenge];
                    Spi::txrx(currentpaket->daten, currentpaket->laenge);
                }
                else
                {
                    currentpaket->laenge=0;
                }
                Gpio::disable_slave(currentpaket->empfaengerindex);
                break;
            }
            case SET_PARAMETER:
            {
                uint8_t ask_for[5];
                ask_for[0]=(uint8_t)SET_PARAMETER;
                ask_for[4]=(currentpaket->laenge)>>24;
                ask_for[3]=(currentpaket->laenge)>>16;
                ask_for[2]=(currentpaket->laenge)>>8;
                ask_for[1]=currentpaket->laenge;
                Gpio::enable_slave(currentpaket->empfaengerindex);
                Spi::txrx(ask_for, 5);
                Spi::txrx(currentpaket->daten, currentpaket->laenge);
                Gpio::disable_slave(currentpaket->empfaengerindex);
                break;
            }
            case GET_DATEN:
            {
                uint8_t nummer[4];
                memcpy(nummer,currentpaket->daten,4);
                uint8_t ask_for[9];
                ask_for[0]=(uint8_t)GET_DATEN;
                ask_for[1]=currentpaket->daten[0];
                ask_for[2]=currentpaket->daten[1];
                ask_for[3]=currentpaket->daten[2];
                ask_for[4]=currentpaket->daten[3];
                Gpio::enable_slave(currentpaket->empfaengerindex);
                Spi::txrx(ask_for, 9);
                currentpaket->laenge = ((uint32_t)ask_for[5]) + ((uint32_t)ask_for[6]<<8) +((uint32_t)ask_for[7]<<16) + ((uint32_t)ask_for[8]<<24);
                delete [] currentpaket->daten;
                currentpaket->daten = new uint8_t[currentpaket->laenge + 8];
                Spi::txrx(currentpaket->daten, currentpaket->laenge + 4);
                Gpio::disable_slave(currentpaket->empfaengerindex);
                currentpaket->daten[currentpaket->laenge+4]=nummer[0];
                currentpaket->daten[currentpaket->laenge+5]=nummer[1];
                currentpaket->daten[currentpaket->laenge+6]=nummer[2];
                currentpaket->daten[currentpaket->laenge+7]=nummer[3];
                break;
            }
            case START_KONT:
            {
                uint8_t ask_for[1];
                ask_for[0]=(uint8_t)START_KONT;
                Gpio::enable_slave(currentpaket->empfaengerindex);
                Spi::txrx(ask_for, 1);
                Gpio::disable_slave(currentpaket->empfaengerindex);
                break;
            }
            case START_STARTSTOP:
            {
                uint8_t ask_for[1];
                ask_for[0]=(uint8_t)START_STARTSTOP;
                Gpio::enable_slave(currentpaket->empfaengerindex);
                Spi::txrx(ask_for, 1);
                Gpio::disable_slave(currentpaket->empfaengerindex);
                break;
            }
            case SET_SAMPLE_FREQ:
            {
                uint8_t ask_for[5];
                ask_for[0]=(uint8_t)SET_SAMPLE_FREQ;
                ask_for[1]=currentpaket->daten[0];
                ask_for[2]=currentpaket->daten[1];
                ask_for[3]=currentpaket->daten[2];
                ask_for[4]=currentpaket->daten[3];
                Gpio::enable_slave(currentpaket->empfaengerindex);
                Spi::txrx(ask_for, 5);
                Gpio::disable_slave(currentpaket->empfaengerindex);
                break;
            }
            case GET_STATUS:
            {
                uint8_t ask_for[1]={GET_STATUS};
                Gpio::enable_slave(currentpaket->empfaengerindex);
                Spi::txrx(ask_for, 1);
                Spi::txrx(currentpaket->daten,24);
                Gpio::disable_slave(currentpaket->empfaengerindex);
                break;
            }
            default:
            {
                break;
            }
            }

            Post::Briefkasten.pop_front();
            Decoder::add_paket(currentpaket);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}
void Post::send_get_parameter(int index)
{
    Paket* getparameterpaket = new Paket();
    getparameterpaket->befehl=GET_PARAMETER;
    getparameterpaket->empfaengerindex=index;
    Briefkasten.push_back(getparameterpaket);
}
void Post::send_set_parameter(int index, uint32_t nummer, std::string wert)
{
    Paket* setparameterpaket = new Paket();
    setparameterpaket->laenge = 4 + wert.length();
    setparameterpaket->befehl=SET_PARAMETER;
    setparameterpaket->daten=new uint8_t[setparameterpaket->laenge];
    setparameterpaket->daten[3]= nummer>>24;
    setparameterpaket->daten[2]= nummer>>16;
    setparameterpaket->daten[1]= nummer>>8;
    setparameterpaket->daten[0]= nummer;
    for(uint32_t i=0;i<wert.length();i++)setparameterpaket->daten[i+4]= wert.at(i);
    setparameterpaket->empfaengerindex=index;
    Briefkasten.push_back(setparameterpaket);
}
void Post::send_get_daten(int index, uint32_t nummer)
{
    Paket* getdatenpaket = new Paket();
    getdatenpaket->daten = new uint8_t[4];
    getdatenpaket->befehl=GET_DATEN;
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
    startkontpaket->befehl=START_KONT;
    startkontpaket->empfaengerindex=-1;
    Briefkasten.push_back(startkontpaket);
}
void Post::send_start_startstop()
{
    Paket* startstartstoppaket = new Paket();
    startstartstoppaket->befehl=START_STARTSTOP;
    startstartstoppaket->empfaengerindex=-1;
    Briefkasten.push_back(startstartstoppaket);
}
void Post::send_set_sample_freq(float freq)
{
    union {
        float asfloat;
        uint8_t asbytes[4];
      } frequency;

    frequency.asfloat=freq;

    Paket* setsamplefreqpaket = new Paket();
    setsamplefreqpaket->daten = new uint8_t[4];
    setsamplefreqpaket->befehl=SET_SAMPLE_FREQ;
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
    getstatuspaket->befehl=GET_STATUS;
    getstatuspaket->empfaengerindex=0;
    getstatuspaket->daten=new uint8_t[24];
    Briefkasten.push_back(getstatuspaket);
}
