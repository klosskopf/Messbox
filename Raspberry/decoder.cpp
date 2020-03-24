#include "decoder.h"

std::list<Paket*> Decoder::Sammelzentrum;

void Decoder::add_paket(Paket* paket)
{
    Sammelzentrum.push_back(paket);
}

void Decoder::decoder_thread()
{
    while(1)
    {
        if (Sammelzentrum.size())
        {
            Paket* currentpaket=Sammelzentrum.front();
            switch(currentpaket->befehl)
            {
            case GET_PARAMETER:
                decode_get_parameter(currentpaket);
                break;
            case SET_PARAMETER:
                break;
            case GET_DATEN:
                decode_get_daten(currentpaket);
                break;
            case START_KONT:
                decode_start_cont();
                break;
            case START_STARTSTOP:
                decode_start_startstop();
                break;
            case SET_SAMPLE_FREQ:
                break;
            case GET_STATUS:
                decode_get_status(currentpaket);
                break;
            }

            Sammelzentrum.pop_front();
            delete currentpaket;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

}

typedef enum {KARTENNAME,NUMMER,NAME,TYP,PARAMETRIERBAR,MIN,MAX,WAHL}GET_PARAMETER_STATES;
void Decoder::decode_get_parameter(Paket* paket)
{
    std::string* wort = NULL;
    char limiter;
    GET_PARAMETER_STATES state=KARTENNAME;

    std::string* kartenname = NULL;

    std::list<Parameter*> *parameterliste = new std::list<Parameter*>;

    uint32_t parameternummer;
    bool floatnotstring;
    std::string* parametername = NULL;
    Parametrierbar parametrierbar;
    float min;
    float max;
    Parameter* parameter;

    while(paket->laenge > paket->ausgewaertet)
    {
        switch(state)
        {
        case KARTENNAME:
            if (kartenname!= NULL)delete kartenname;
            kartenname=new std::string();
            limiter = get_next_word(paket, kartenname);
            state=NUMMER;
            break;
        case NUMMER:
            if (wort)delete wort;
            wort=new std::string();
            limiter = get_next_word(paket, wort);
            parameternummer=std::atoi((*wort).c_str());
            state=NAME;
            break;
        case NAME:
            if (parametername)delete parametername;
            parametername=new std::string();
            limiter = get_next_word(paket, parametername);
            state=TYP;
            break;
        case TYP:
            if (wort!= NULL)delete wort;
            wort=new std::string();
            limiter = get_next_word(paket, wort);
            floatnotstring = (*wort == "f") ? true : false;
            state=PARAMETRIERBAR;
            break;
        case PARAMETRIERBAR:
            if (wort!= NULL)delete wort;
            wort=new std::string();
            limiter = get_next_word(paket, wort);
            if (*wort == "f") parametrierbar = FREI;
            else if (*wort == "l") parametrierbar = LISTE;
            else parametrierbar = NEIN;
            state=MIN;
            break;
        case MIN:
            if (wort!= NULL)delete wort;
            wort=new std::string();
            limiter = get_next_word(paket, wort);
            min = std::atof((*wort).c_str());
            state=MAX;
            break;
        case MAX:
            if (wort!= NULL)delete wort;
            wort=new std::string();
            limiter = get_next_word(paket, wort);
            max = std::atof((*wort).c_str());
            parameter= new Parameter(parameternummer,floatnotstring,*parametername,parametrierbar,min,max);
            parameterliste->push_back(parameter);
            if (limiter == '{')state=WAHL;
            else state=NUMMER;
            break;
        case WAHL:
            if (wort!= NULL)delete wort;
            wort=new std::string();
            limiter = get_next_word(paket, wort);
            parameter->add_auswahl(*wort);
            if (limiter == '}')state=NUMMER;
            else state=WAHL;
            break;
        }
    }
    Karte* karte = new Karte(Control::gui, paket->empfaengerindex, *kartenname, parameterliste);
    Control::Kartenset.push_back(karte);

    if (wort != NULL)delete wort;
    if (kartenname != NULL)delete kartenname;
    if (parametername != NULL)delete parametername;
    Control::newkarte=false;
}
void Decoder::decode_get_daten(Paket* paket)
{


}
void Decoder::decode_start_cont()
{

}
void Decoder::decode_start_startstop()
{

}
void Decoder::decode_get_status(Paket* paket)
{

}


char Decoder::get_next_word(Paket* paket, std::string* wort)
{
    while(1)
    {
        if (paket->ausgewaertet == paket->laenge)return EOF;
        else if( paket->daten[paket->ausgewaertet] == ','){paket->ausgewaertet++;return ',';}
        else if( paket->daten[paket->ausgewaertet] == '{'){paket->ausgewaertet++;return '{';}
        else if( paket->daten[paket->ausgewaertet] == '}'){paket->ausgewaertet++;return '}';}
        else if( paket->daten[paket->ausgewaertet] == '}'){paket->ausgewaertet++;return '{';}
        else{*wort += paket->daten[paket->ausgewaertet];paket->ausgewaertet++;}
    }
}
