/*
 * parameter.c
 *
 *  Created on: 18.03.2020
 *      Author: klosskopf
 */
#include "parameter.h"
#include "string.h"
#include "gpio.h"

extern const GPIO_PIN LED;
volatile get_daten_t ausgabe_buffer1;
volatile get_daten_t ausgabe_buffer2;

volatile get_daten_t* activated_buffer;

void init_parameter()
{
	ausgabe_buffer1.paket_size=0x0;
	ausgabe_buffer1.startzeit=0x0;
	activated_buffer=&ausgabe_buffer1;
}

void new_data(volatile float data)
{
	if (activated_buffer->paket_size >= 0x400)
	{
		set_gpio(LED,1);
	}
	else
	{
		activated_buffer->daten[activated_buffer->paket_size>>2]=data;
		activated_buffer->paket_size+=4;
	}
}

void reset_data()
{
	set_gpio(LED,0);
	ausgabe_buffer1.paket_size=0x0;
	ausgabe_buffer1.startzeit=0x0;
	activated_buffer=&ausgabe_buffer1;
}

void set_parameter(uint32_t nummer, const char* anweisung)
{
	if(nummer==10)
	{
		if (strcmp(anweisung,"LED AN")==0)
		{
			set_gpio(LED,1);
		}
		else if (strcmp(anweisung,"LED AUS")==0)
		{
			set_gpio(LED,0);
		}
	}
}

get_daten_t* get_datenblock(uint32_t nummer)
{
	volatile get_daten_t* returnbuffer;
	returnbuffer=activated_buffer;
	if (activated_buffer==&ausgabe_buffer1)
	{
		ausgabe_buffer2.startzeit=activated_buffer->startzeit+(activated_buffer->paket_size>>2);
		ausgabe_buffer2.paket_size=0;
		activated_buffer=&ausgabe_buffer2;
	}
	else
	{
		ausgabe_buffer1.startzeit=activated_buffer->startzeit+(activated_buffer->paket_size>>2);
		ausgabe_buffer1.paket_size=0;
		activated_buffer=&ausgabe_buffer1;
	}
	return returnbuffer;
}
