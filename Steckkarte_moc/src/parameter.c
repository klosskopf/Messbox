/*
 * parameter.c
 *
 *  Created on: 18.03.2020
 *      Author: klosskopf
 */
#include "parameter.h"
#include "gpio.h"
#include "flash.h"
#include <stdbool.h>

extern const GPIO_PIN LED;

typedef struct {
	volatile PARAMETER parameternummer;
	volatile uint32_t startzeit;
}Datenblock_t;

volatile Datenblock_t flash_meta[FLASHPAGECOUNT]={0};
volatile parameter_t parameterliste[MAXPARAMETER];								//Ja index 0 ist ungenutzt ;)
volatile bool full;
volatile int nextindexwritten,nextreadindex;
volatile const get_daten_t fehlerpaket={0,0,{0}};

volatile parameterbuffer_t voltage_in_buffer;
volatile parameterbuffer_t voltage_out_buffer;
volatile parameterbuffer_t resistance_in_buffer;

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
	parameterliste[SPANNUNG_IN].channel=ADC1_IN5;
	parameterliste[SPANNUNG_IN].faktor=2.820432466;
	parameterliste[SPANNUNG_IN].offset=0;
	parameterliste[SPANNUNG_IN].buffer=&voltage_in_buffer;


	parameterliste[WIDERSTAND_IN].name="Resistance in";
	parameterliste[WIDERSTAND_IN].string_not_float=FLOAT;
	parameterliste[WIDERSTAND_IN].parametrierbar=NICHT;
	parameterliste[WIDERSTAND_IN].min=0;
	parameterliste[WIDERSTAND_IN].max=3.3;
	parameterliste[WIDERSTAND_IN].wahlnr=0;
	parameterliste[WIDERSTAND_IN].wahl=NULL;
	parameterliste[WIDERSTAND_IN].channel=ADC1_IN7;
	parameterliste[WIDERSTAND_IN].faktor=754.3720191;
	parameterliste[WIDERSTAND_IN].offset=7.733386328;
	parameterliste[WIDERSTAND_IN].buffer=&resistance_in_buffer;


	parameterliste[DAC_MAXSPANNUNG].name="Output max.";
	parameterliste[DAC_MAXSPANNUNG].string_not_float=STRING;
	parameterliste[DAC_MAXSPANNUNG].parametrierbar=FREI;
	parameterliste[DAC_MAXSPANNUNG].min=0;
	parameterliste[DAC_MAXSPANNUNG].max=3.3;
	parameterliste[DAC_MAXSPANNUNG].wahlnr=5;
	static char* vout_max_wahl[5]={"0","1","2","3","3.3"};
	parameterliste[DAC_MAXSPANNUNG].wahl=vout_max_wahl;
	parameterliste[DAC_MAXSPANNUNG].channel=NOCHANNEL;
	parameterliste[DAC_MAXSPANNUNG].faktor=1;
	parameterliste[DAC_MAXSPANNUNG].offset=0;
	parameterliste[DAC_MAXSPANNUNG].buffer=NULL;
	extern void set_dac_maxvoltage();
	parameterliste[DAC_MAXSPANNUNG].setfunction=&set_dac_maxvoltage;

	parameterliste[DAC_MINSPANNUNG].name="Output min.";
	parameterliste[DAC_MINSPANNUNG].string_not_float=STRING;
	parameterliste[DAC_MINSPANNUNG].parametrierbar=FREI;
	parameterliste[DAC_MINSPANNUNG].min=0;
	parameterliste[DAC_MINSPANNUNG].max=3.3;
	parameterliste[DAC_MINSPANNUNG].wahlnr=5;
	static char* vout_min_wahl[5]={"0","1","2","3","3.3"};
	parameterliste[DAC_MINSPANNUNG].wahl=vout_min_wahl;
	parameterliste[DAC_MINSPANNUNG].channel=NOCHANNEL;
	parameterliste[DAC_MINSPANNUNG].faktor=1;
	parameterliste[DAC_MINSPANNUNG].offset=0;
	parameterliste[DAC_MINSPANNUNG].buffer=NULL;
	extern void set_dac_minvoltage();
	parameterliste[DAC_MINSPANNUNG].setfunction=&set_dac_minvoltage;

	parameterliste[DAC_FREQUENCY].name="Output freq.";
	parameterliste[DAC_FREQUENCY].string_not_float=STRING;
	parameterliste[DAC_FREQUENCY].parametrierbar=FREI;
	parameterliste[DAC_FREQUENCY].min=0;
	parameterliste[DAC_FREQUENCY].max=1000;
	parameterliste[DAC_FREQUENCY].wahlnr=5;
	static char* vout_freq_wahl[5]={"0","1","10","100","1000"};
	parameterliste[DAC_FREQUENCY].wahl=vout_freq_wahl;
	parameterliste[DAC_FREQUENCY].channel=NOCHANNEL;
	parameterliste[DAC_FREQUENCY].faktor=1;
	parameterliste[DAC_FREQUENCY].offset=0;
	parameterliste[DAC_FREQUENCY].buffer=NULL;
	extern void set_dac_frequency();
	parameterliste[DAC_FREQUENCY].setfunction=&set_dac_frequency;

	parameterliste[DAC_OUTPUT].name="Output voltage";
	parameterliste[DAC_OUTPUT].string_not_float=FLOAT;
	parameterliste[DAC_OUTPUT].parametrierbar=NICHT;
	parameterliste[DAC_OUTPUT].min=0;
	parameterliste[DAC_OUTPUT].max=3.3;
	parameterliste[DAC_OUTPUT].wahlnr=0;
	parameterliste[DAC_OUTPUT].wahl=NULL;
	parameterliste[DAC_OUTPUT].channel=NOCHANNEL;
	parameterliste[DAC_OUTPUT].faktor=1;
	parameterliste[DAC_OUTPUT].offset=0;
	parameterliste[DAC_OUTPUT].buffer=&voltage_out_buffer;


	parameterliste[LED_TEST].name="LED test";
	parameterliste[LED_TEST].string_not_float=STRING;
	parameterliste[LED_TEST].parametrierbar=LISTE;
	parameterliste[LED_TEST].min=0;
	parameterliste[LED_TEST].max=0;
	parameterliste[LED_TEST].wahlnr=2;
	static char* ledtest_wahl[2]={"LED AN","LED AUS"};
	parameterliste[LED_TEST].wahl=ledtest_wahl;
	parameterliste[LED_TEST].channel=NOCHANNEL;
	parameterliste[LED_TEST].faktor=1;
	parameterliste[LED_TEST].offset=0;
	parameterliste[LED_TEST].buffer=NULL;
	extern void led_test(const char*);
	parameterliste[LED_TEST].setfunction=&led_test;

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

void new_data(volatile PARAMETER parameter, volatile float data)
{
	parameterliste[parameter].buffer->lastvalue=(data);//+4*parameterliste[parameter].buffer->lastvalue)/5;
	parameterliste[parameter].buffer->peingangsbuffer->daten[(parameterliste[parameter].buffer->eingangsbuffersize)>>2]=parameterliste[parameter].buffer->lastvalue;
	parameterliste[parameter].buffer->eingangsbuffersize+=4;

	if (parameterliste[parameter].buffer->eingangsbuffersize == FLASHPAGESIZE)
	{
		volatile Datenblock_t* gefunden=NULL;
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
			volatile get_daten_t* oldbuffer=parameterliste[parameter].buffer->peingangsbuffer;
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

void set_parameter(volatile uint32_t nummer, volatile const char* anweisung)
{
	if (nummer < MAXPARAMETER && parameterliste[nummer].setfunction)
	{
		parameterliste[nummer].setfunction(anweisung);
	}
}

volatile get_daten_t* get_datenblock(volatile PARAMETER parameter)
{
	volatile get_daten_t* returnbuffer;
	volatile Datenblock_t* gefunden=NULL;
	volatile uint32_t index;

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
