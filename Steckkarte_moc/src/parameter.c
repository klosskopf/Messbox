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

parameterbuffer_t voltage_in_buffer;
parameterbuffer_t voltage_out_buffer;

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
	parameterliste[SPANNUNG_IN].buffer=&voltage_in_buffer;

	parameterliste[SPANNUNG_OUT].name="Voltage out";
	parameterliste[SPANNUNG_OUT].string_not_float=FLOAT;
	parameterliste[SPANNUNG_OUT].parametrierbar=FREI;
	parameterliste[SPANNUNG_OUT].min=0;
	parameterliste[SPANNUNG_OUT].max=3.3;
	parameterliste[SPANNUNG_OUT].wahlnr=5;
	static char* vout_wahl[5]={"0","1","2","3","3.3"};
	parameterliste[SPANNUNG_OUT].wahl=vout_wahl;
	parameterliste[SPANNUNG_OUT].buffer=&voltage_out_buffer;

	parameterliste[LED_TEST].name="LED test";
	parameterliste[LED_TEST].string_not_float=STRING;
	parameterliste[LED_TEST].parametrierbar=LISTE;
	parameterliste[LED_TEST].min=0;
	parameterliste[LED_TEST].max=0;
	parameterliste[LED_TEST].wahlnr=2;
	static char* ledtest_wahl[2]={"LED AN","LED AUS"};
	parameterliste[LED_TEST].wahl=ledtest_wahl;
	parameterliste[LED_TEST].buffer=NULL;

	for (uint32_t i=1; i<MAXPARAMETER; i++)
	{
		if (parameterliste[i].buffer)
		{
			parameterliste[i].buffer->peingangsbuffer=&(parameterliste[i].buffer->eingangsbuffer[0]);
			parameterliste[i].buffer->eingangsbuffersize=0;
			parameterliste[i].buffer->eingangsbufferstartzeit=0;
			parameterliste[i].buffer->ausgangsbuffer.startzeit=0;
		}
	}
	full=false;
}

void new_data(PARAMETER parameter, volatile float data)
{
	parameterliste[parameter].buffer->peingangsbuffer->daten[(parameterliste[parameter].buffer->eingangsbuffersize)>>2]=data;
	parameterliste[parameter].buffer->eingangsbuffersize+=4;

	if (parameterliste[parameter].buffer->eingangsbuffersize == FLASHPAGESIZE)
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
			parameterliste[parameter].buffer->eingangsbuffersize=0;
			parameterliste[parameter].buffer->eingangsbufferstartzeit+=0x1000;
		}
		else
		{
			full=false;
			get_daten_t* oldbuffer=parameterliste[parameter].buffer->peingangsbuffer;
			parameterliste[parameter].buffer->eingangsbuffersize=0;
			if(oldbuffer==parameterliste[parameter].buffer->eingangsbuffer) parameterliste[parameter].buffer->peingangsbuffer = &(parameterliste[parameter].buffer->eingangsbuffer[1]);
			else parameterliste[parameter].buffer->peingangsbuffer = &(parameterliste[parameter].buffer->eingangsbuffer[0]);
			gefunden->startzeit=parameterliste[parameter].buffer->eingangsbufferstartzeit;
			(parameterliste[parameter].buffer->eingangsbufferstartzeit)+=(FLASHPAGESIZE>>2);
			write_block(index*FLASHPAGESIZE,(uint8_t*)(oldbuffer->daten));
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
		parameterliste[i].buffer->eingangsbuffersize=0;
		parameterliste[i].buffer->eingangsbufferstartzeit=0;
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
			returnbuffer=&(parameterliste[parameter].buffer->ausgangsbuffer);
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
		returnbuffer=parameterliste[parameter].buffer->peingangsbuffer;

		returnbuffer->startzeit=parameterliste[parameter].buffer->eingangsbufferstartzeit;
		(parameterliste[parameter].buffer->eingangsbufferstartzeit)+=(parameterliste[parameter].buffer->eingangsbuffersize >> 2);
		returnbuffer->paket_size=parameterliste[parameter].buffer->eingangsbuffersize;
		parameterliste[parameter].buffer->eingangsbuffersize=0;

		if(returnbuffer==parameterliste[parameter].buffer->eingangsbuffer) parameterliste[parameter].buffer->peingangsbuffer = &(parameterliste[parameter].buffer->eingangsbuffer[1]);
		else parameterliste[parameter].buffer->peingangsbuffer = &(parameterliste[parameter].buffer->eingangsbuffer[0]);
	}

	return returnbuffer;
}
