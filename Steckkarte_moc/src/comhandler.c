/*
 * comhandler.c
 *
 *  Created on: 18.03.2020
 *      Author: klosskopf
 */
#include "comhandler.h"

SPI_TypeDef * comhandlerspi;

void decoder();
void get_parameter_decoder(uint32_t position, uint8_t datum);
void set_parameter_decoder(uint32_t position, uint8_t datum);
void get_daten_decoder(uint32_t position, uint8_t datum);
void start_kont_decoder(uint32_t position, uint8_t datum);
void start_startstop_decoder(uint32_t position, uint8_t datum);

void init_comhandler(SPI_TypeDef * spi)
{
	comhandlerspi=spi;
	decoderbytenr=0;
	init_spi(spi, SLAVE, decoder);
	befehllut[0].befehl=GET_PARAMETER; befehllut[0].handler=&get_parameter_decoder;
	befehllut[1].befehl=SET_PARAMETER; befehllut[1].handler=&set_parameter_decoder;
	befehllut[2].befehl=GET_DATEN; befehllut[2].handler=&get_daten_decoder;
	befehllut[3].befehl=START_KONT; befehllut[3].handler=&start_kont_decoder;
	befehllut[4].befehl=START_STARTSTOP; befehllut[4].handler=&start_startstop_decoder;
}

void decoder()
{
	static BEFEHL befehl;
	uint8_t character=read_spi(comhandlerspi);
	if (decoderbytenr==0)
	{
		befehl=(BEFEHL)character;
	}
	int i;
	for(i=0;i<BEFEHLNR-1;i++)
	{
		if (befehl==befehllut[i].befehl)
		{
			uint32_t dummy = decoderbytenr++;
			befehllut[i].handler(dummy, character);
			break;
		}
	}
	if (i==BEFEHLNR-1)decoderbytenr=0;
}

void get_parameter_decoder(uint32_t position, uint8_t datum)
{
	const char* parameter = "Spannungsmesser,1,Spannung,f,n,0,5,2,Messbereich,s,l,0,0{pA,nA,mA,A}3,Strom,f,n,0,0,20,Ich liebe Julia,s,l,0,0{Beer,Lukas}";
	const uint32_t size = strlen(parameter);
	if (position==size+5) decoderbytenr=0;
	else if (position==0) send_spi(comhandlerspi,size);
	else if (position==1) send_spi(comhandlerspi,size>>8);
	else if (position==2) send_spi(comhandlerspi,size>>16);
	else if (position==3) send_spi(comhandlerspi,size>>24);
	else send_spi(comhandlerspi,parameter[position-4]);
}
void set_parameter_decoder(uint32_t position, uint8_t datum)
{
	static uint32_t size;
	static char buffer[50];
	if (position==0) {size=0; memset(buffer,0,sizeof(buffer));}
	else if (position==1) size |= (uint32_t)datum << 24;
	else if (position==2) size |= (uint32_t)datum << 16;
	else if (position==3) size |= (uint32_t)datum << 8;
	else if (position==4) size |= (uint32_t)datum;
	else
	{
		if (position-5 > sizeof(buffer)-1)	//A hard fault here would ruin your day
		{
			decoderbytenr=0;
			return;
		}
		buffer[position-5]=datum;
		if (position==size+4)
		{
			decoderbytenr=0;
			set_parameter(buffer);
		}
	}
}
void get_daten_decoder(uint32_t position, uint8_t datum)
{
	static uint32_t nummer;
	static uint32_t packetsize;
	static Datenblock* block;
	static uint32_t transmitbyte;
	if (position == 0) nummer=0;
	else if (position == 1) nummer |= datum<<24;
	else if (position == 2) nummer |= datum<<16;
	else if (position == 3) nummer |= datum<<8;
	else if (position == 4)
	{
		nummer |= datum;
		packetsize=database_size(nummer);
		send_spi(comhandlerspi, packetsize>>24);
	}
	else if (position == 5){send_spi(comhandlerspi, packetsize>>16); block = get_datenblock(nummer);}
	else if (position == 6) send_spi(comhandlerspi, packetsize>>8);
	else if (position == 7) send_spi(comhandlerspi, packetsize);
	else if (position == 8) send_spi(comhandlerspi, block->starttime>>24);
	else if (position == 9){ send_spi(comhandlerspi, block->starttime>>16); transmitbyte=0;}
	else if (position == 10) send_spi(comhandlerspi, block->starttime>>8);
	else if (position == 11) send_spi(comhandlerspi, block->starttime);
	else if (position == packetsize + 12)
		decoderbytenr=0;
	else
	{
		send_spi(comhandlerspi, block->Data.binary[transmitbyte++]);
		if(transmitbyte==0x1000)
		{
			block = get_datenblock(nummer);
			transmitbyte=0;
		}
	}

}
void start_kont_decoder(uint32_t position, uint8_t datum)
{

}
void start_startstop_decoder(uint32_t position, uint8_t datum)
{

}
