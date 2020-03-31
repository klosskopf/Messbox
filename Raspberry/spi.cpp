#include "spi.h"
#include "string.h"
#include "paket.h"

#define BLOCK

typedef struct{
    uint8_t befehl;
    uint32_t lenght;
    char daten[255];
}get_parameter;
union {
    uint8_t bytes[sizeof(get_parameter)];
    get_parameter simple = {0,0,"Spannungsmesser,0,Spannung,f,f,0,5{0,1,2,3,4,5}1,Strom,f,n,0,0,2,Form,s,l,0,0{Rechteck,Sinus,Dreieck,Saegezahn,Ich liebe Julia"};
} get_parameter_daten;

typedef struct{
    uint8_t befehl;
    uint32_t nummer;
    uint32_t lenght;
    uint32_t startzeit;
    float daten[0x400];
}get_daten;
union {
    uint8_t bytes[sizeof(get_daten)];
    get_daten simple;
} getdatendaten;

typedef struct{
    uint8_t befehl;
    float v5v;
    float v33v;
    float icharge;
    float vbat;
    float vlade;
    float vin;
}get_status;
union {
    uint8_t bytes[sizeof(get_status)];
    get_status simple;
} getstatusdaten;

void Spi::txrx(uint8_t * data, uint32_t laenge)
{
    get_parameter_daten.simple.lenght=strlen(get_parameter_daten.simple.daten);
    for(int o=0;o<0x400;o++)
    {
        getdatendaten.simple.daten[o]=((float)(o*o));
    }
    getdatendaten.simple.lenght=0x1000;
    getdatendaten.simple.startzeit=0xffff0000;
    getstatusdaten.simple.v5v=5.01;
    getstatusdaten.simple.vin=14.12;
    getstatusdaten.simple.v33v=3.4;
    getstatusdaten.simple.vbat=4.21;
    getstatusdaten.simple.vlade=5.00;
    getstatusdaten.simple.icharge=0.75;

    for (uint32_t n=0;n<laenge;n++)
    {
        if(i==0)
        {
            befehl=data[i];
        }
        switch(befehl)
        {
        case 0x01:
            if (i<1)data[n] = get_parameter_daten.bytes[i];
            else data[n] = get_parameter_daten.bytes[i+3];
            break;

        case 0x03:
            if (i<1)data[n] = getdatendaten.bytes[i];
            else data[n] = getdatendaten.bytes[i+3];
            break;
        case 0x09:
            if (i<1)data[n] = getstatusdaten.bytes[i];
            else data[n] = getstatusdaten.bytes[i+3];
            break;
        default:
            break;
        }
        i++;
    }
}

uint32_t Spi::i=0;
uint8_t Spi::befehl;
