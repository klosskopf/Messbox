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

extern const GPIO_PIN LED;

typedef struct {
	PARAMETER parameternummer;
	uint32_t startzeit;
}Datenblock_t;

Datenblock_t flash_meta[FLASHPAGECOUNT]={0};
parameter_t parameterliste[MAXPARAMETER];								//Ja index 0 ist ungenutzt ;)

void init_parameter()
{
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
	char* vout_wahl[5]={"0","1","2","3","3.3"};
	parameterliste[SPANNUNG_OUT].wahl=vout_wahl;

	parameterliste[LED_TEST].name="LED test";
	parameterliste[LED_TEST].string_not_float=STRING;
	parameterliste[LED_TEST].parametrierbar=LISTE;
	parameterliste[LED_TEST].min=0;
	parameterliste[LED_TEST].max=0;
	parameterliste[LED_TEST].wahlnr=2;
	char* ledtest_wahl[2]={"LED AN","LED AUS"};
	parameterliste[LED_TEST].wahl=ledtest_wahl;

	for (uint32_t i=0; i<MAXPARAMETER; i++)
	{
		parameterliste[i].eingangsbuffer.paket_size=0;
		parameterliste[i].ausgangsbuffer.startzeit=0;
	}
}

void new_data(PARAMETER parameter, volatile float data)
{
	parameterliste[parameter].eingangsbuffer.daten[(parameterliste[parameter].eingangsbuffer.paket_size)>>2]=data;
	parameterliste[parameter].eingangsbuffer.paket_size+=4;

	if (parameterliste[parameter].eingangsbuffer.paket_size == FLASHPAGESIZE)
	{
		uint32_t index;
		for (index = 0; index<FLASHPAGECOUNT; index++)
		{
			if(flash_meta[index].parameternummer == NOPARAM) break;
		}
		if ((index==FLASHPAGECOUNT) && (flash_meta[FLASHPAGECOUNT-1].parameternummer != NOPARAM))
		{

			set_gpio(LED,1);
		}
		else
		{
			flash_meta[index].startzeit=parameterliste[parameter].eingangsbuffer.startzeit;
			write_block(index*PAGESIZE,(uint8_t*)(parameterliste[parameter].eingangsbuffer.daten));
			flash_meta[index].parameternummer=parameter;
		}
	}
}

void reset_data()
{
	set_gpio(LED,0);
	for (uint32_t i=0; i<MAXPARAMETER; i++)
	{
		parameterliste[i].eingangsbuffer.paket_size=0;
		parameterliste[i].ausgangsbuffer.startzeit=0;
	}
	for (uint32_t i=0; i<FLASHPAGECOUNT; i++)
	{
		flash_meta[i].parameternummer=NOPARAM;
	}
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
	{
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

}

volatile get_daten_t* get_datenblock(PARAMETER parameter)
{
	volatile get_daten_t* returnbuffer;
	Datenblock_t* gefunden=NULL;
	uint32_t index;

	for(index=0; index<FLASHPAGECOUNT; index++)
	{
		if (flash_meta[index].parameternummer==parameter)
		{
			if (gefunden==NULL)
			{
				gefunden=&flash_meta[index];
			}
			else if(gefunden->startzeit > flash_meta[index].startzeit)
			{
				gefunden=&flash_meta[index];
			}
		}
	}

	if (gefunden)
	{
		returnbuffer=&parameterliste[parameter].ausgangsbuffer;
		returnbuffer->startzeit=flash_meta[index].startzeit;
		flash_meta[index].startzeit=-1;
		returnbuffer->paket_size=FLASHPAGESIZE;
		read_block(index*PAGESIZE,(uint8_t*)(returnbuffer->daten));
		//erase_data(index*PAGESIZE);
		flash_meta[index].parameternummer=NOPARAM;
	}
	else
	{
		returnbuffer=&parameterliste[parameter].eingangsbuffer;
	}

	return returnbuffer;
}
