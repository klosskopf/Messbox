#include "spi.h"
#include "string.h"

void Spi::txrx(uint8_t * data, uint32_t laenge)
{
    const char* retdata="Spannungsmesser,Spannung,f,f,0,5{0,1,2,3,4,5}Strom,f,n,0,0";
    static int n=0;
    for (uint32_t i=0;i<laenge;i++)
    {
        if (n==0)data[i]=0;
        else if (n==1)data[i]=(uint8_t)(strlen(retdata)>>24);
        else if (n==2)data[i]=(uint8_t)(strlen(retdata)>>16);
        else if (n==3)data[i]=(uint8_t)(strlen(retdata)>>8);
        else if (n==4)data[i]=(uint8_t)strlen(retdata);
        else data[i]=retdata[(n-5)%strlen(retdata)];
        n++;
    }
}
