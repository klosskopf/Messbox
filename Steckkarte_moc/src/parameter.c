/*
 * parameter.c
 *
 *  Created on: 18.03.2020
 *      Author: klosskopf
 */
#include "parameter.h"
#include "dac.h"
#include "string.h"
#include "gpio.h"
#include "flash.h"
#include <stdlib.h>
#include <stdbool.h>

extern const GPIO_PIN LED;

typedef struct {
	PARAMETER parameternummer;
	uint32_t startzeit;
}Datenblock_t;

Datenblock_t flash_meta[FLASHPAGECOUNT]={0};
parameter_t parameterliste[MAXPARAMETER];								//Ja index 0 ist ungenutzt ;)
bool full;
volatile int nextindexwritten,nextreadindex;
const get_daten_t fehlerpaket={0,0,{0}};

void init_parameter()
{
	nextindexwritten=0;
	nextreadindex=0;
	parameterliste[SPANNUNG_IN].name="Voltage in";
	parameterliste[SPANNUNG_IN].string_not_float=FLOAT;
	parameterliste[SPANNUNG_IN].parametrierbar=NICHT;
	parameterliste[SPANNUNG_IN].min=0;
	parameterliste[SPANNUNG_IN].max=3.3;
	parameterliste[SPANNUNG_IN].wahlnr=0;
	parameterliste[SPANNUNG_IN].wahl=NULL;

	parameterliste[SPANNUNG_OUT].name="Voltage out";
	parameterliste[SPANNUNG_OUT].string_not_float=FLOAT;
	parameterliste[SPANNUNG_OUT].parametrierbar=FREI;
	parameterliste[SPANNUNG_OUT].min=0;
	parameterliste[SPANNUNG_OUT].max=3.3;
	parameterliste[SPANNUNG_OUT].wahlnr=4;
	static char* vout_wahl[5]={"0","1","2","3","3.3"};
	parameterliste[SPANNUNG_OUT].wahl=vout_wahl;

	parameterliste[LED_TEST].name="LED test";
	parameterliste[LED_TEST].string_not_float=STRING;
	parameterliste[LED_TEST].parametrierbar=LISTE;
	parameterliste[LED_TEST].min=0;
	parameterliste[LED_TEST].max=0;
	parameterliste[LED_TEST].wahlnr=2;
	static char* ledtest_wahl[2]={"LED AN","LED AUS"};
	parameterliste[LED_TEST].wahl=ledtest_wahl;

	for (uint32_t i=0; i<MAXPARAMETER; i++)
	{
		parameterliste[i].eingangsbuffer.paket_size=0;
		parameterliste[i].eingangsbuffersize=0;
		parameterliste[i].eingangsbufferstartzeit=0;
		parameterliste[i].ausgangsbuffer.startzeit=0;
	}
	full=false;
}

void new_data(PARAMETER parameter, volatile float data)
{
	if(!full)
	{
		parameterliste[parameter].eingangsbuffer.daten[(parameterliste[parameter].eingangsbuffersize)>>2]=data;
		parameterliste[parameter].eingangsbuffersize+=4;
	}

	if (parameterliste[parameter].eingangsbuffersize == FLASHPAGESIZE)
	{
		Datenblock_t* gefunden=NULL;
		int index;

		index=nextindexwritten;
		do
		{
			if(flash_meta[index].parameternummer == NOPARAM)
			{
				nextindexwritten=index;
				gefunden=&flash_meta[index];
				break;
			}
			index++;
			if(index==FLASHPAGECOUNT)index=0;

		}while(nextindexwritten!=index);

		if (!gefunden)
		{
			set_gpio(LED,1);
			full=true;
		}
		else
		{
			full=false;
			gefunden->startzeit=parameterliste[parameter].eingangsbufferstartzeit;
			(parameterliste[parameter].eingangsbufferstartzeit)+=(FLASHPAGESIZE>>2);
			parameterliste[parameter].eingangsbuffersize=0;
			write_block(index*FLASHPAGESIZE,(uint8_t*)(parameterliste[parameter].eingangsbuffer.daten));
			gefunden->parameternummer=parameter;
		}
	}
}

void reset_data()
{
	nextindexwritten++;
	nextreadindex++;
	set_gpio(LED,0);
	for (uint32_t i=0; i<MAXPARAMETER; i++)
	{
		parameterliste[i].eingangsbuffer.paket_size=0;
		parameterliste[i].ausgangsbuffer.paket_size=0;
		parameterliste[i].eingangsbuffer.startzeit=0;
		parameterliste[i].ausgangsbuffer.startzeit=0;
		parameterliste[i].eingangsbuffersize=0;
		parameterliste[i].eingangsbufferstartzeit=0;
	}
	for (uint32_t i=0; i<FLASHPAGECOUNT; i++)
	{
		flash_meta[i].parameternummer=NOPARAM;
	}
	full=false;
}

void set_parameter(uint32_t nummer, const char* anweisung)
{
	switch(nummer)
	{
	case 2:
	{
		float voltage=atof(anweisung);
		set_dac(voltage);
		break;
	}
	case 3:
		if (strcmp(anweisung,"LED AN")==0)
		{
			set_gpio(LED,1);
		}
		else if (strcmp(anweisung,"LED AUS")==0)
		{
			set_gpio(LED,0);
		}
		break;
	}
}

volatile get_daten_t* get_datenblock(PARAMETER parameter)
{
	volatile get_daten_t* returnbuffer;
	Datenblock_t* gefunden=NULL;
	uint32_t index;

	index=nextreadindex;
	do
	{
		if(flash_meta[index].parameternummer == parameter)
		{
			nextreadindex=index;
			gefunden=&flash_meta[index];
			break;
		}
		index++;
		if(index==FLASHPAGECOUNT)index=0;

	}while(nextreadindex!=index);

	if (gefunden)
	{
		if(get_flash_state()==IDLE)
		{
			returnbuffer=&parameterliste[parameter].ausgangsbuffer;
			returnbuffer->startzeit=flash_meta[index].startzeit;
			flash_meta[index].startzeit=-1;
			returnbuffer->paket_size=FLASHPAGESIZE;
			read_block(index*FLASHPAGESIZE,(uint8_t*)(returnbuffer->daten));
			//erase_data(index*PAGESIZE);
			flash_meta[index].parameternummer=NOPARAM;
		}
		else
		{
			returnbuffer=&fehlerpaket;
		}
	}
	else
	{
		returnbuffer=&parameterliste[parameter].eingangsbuffer;
		parameterliste[parameter].eingangsbuffer.startzeit=parameterliste[parameter].eingangsbufferstartzeit;
		(parameterliste[parameter].eingangsbufferstartzeit)+=(parameterliste[parameter].eingangsbuffersize >> 2);
		parameterliste[parameter].eingangsbuffer.paket_size=parameterliste[parameter].eingangsbuffersize;
		parameterliste[parameter].eingangsbuffersize=0;
	}

	return returnbuffer;
}
