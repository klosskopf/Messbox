#include "post.h"

std::list<Paket*> Post::Briefkasten;

void Post::spi_thread()
{
    Paket* currentpaket;
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
                currentpaket->laenge = ((uint32_t)ask_for[4]) + ((uint32_t)ask_for[3]<<8) +((uint32_t)ask_for[2]<<16) + ((uint32_t)ask_for[1]<<24);
                currentpaket->daten = new uint8_t[currentpaket->laenge];
                Spi::txrx(currentpaket->daten, currentpaket->laenge);
                Gpio::disable_slave(currentpaket->empfaengerindex);
                Post::Briefkasten.pop_front();
                Decoder::add_paket(currentpaket);
                break;
            }
            case SET_PARAMETER:
            {
                break;
            }
            case GET_DATEN:
            {
                break;
            }
            case START_KONT:
            {
                break;
            }
            case START_STARTSTOP:
            {
                break;
            }
            case SET_SAMPLE_FREQ:
            {
                break;
            }
            case GET_STATUS:
            {
                break;
            }
            default:
            {
                break;
            }
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void Post::send_get_parameter(int index)
{
    Paket* getparameterpaket = new Paket();
    getparameterpaket->befehl=GET_PARAMETER;
    getparameterpaket->empfaengerindex=index;
    Briefkasten.push_back(getparameterpaket);
}
