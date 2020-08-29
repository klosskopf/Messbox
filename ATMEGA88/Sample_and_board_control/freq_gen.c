/*
 * freq_gen.c
 *
 * Created: 18.07.2020 14:37:14
 *  Author: klosskopf
 */ 

#include "freq_gen.h"
#include "global.h"
#include <avr/io.h>

void init_freq()
{
	OSCCAL=0x53;										//Enhance the factory calibration, if possible
	DDRB |= (1<<SAMPLE);								//Set the sampe-pin
	
	TCCR1A |= (1<<COM1A0) | (1<<WGM11) | (1<<WGM10) ;	//Toggle the sample pin on compare match
	TCCR1B |= (1<<WGM12) | (1<<WGM13) ;					//Counter in mode 15: reset at OCR1A
	samplefreq=1000;									//The Raspberry assumes a sample freq of 1kHz at start
	set_sample_freq(0);									//samplefreq is just a variable, to enable it, it must be activated
}

void set_sample_freq(float freq)
{
	double ticks;
	if (freq==0)SET_PRE_0;
	else if (freq>92)
	{
		ticks=6000000/(double)freq;		//(fosz/2/N) / freq
		SET_PRE_1;
		OCR1A=(uint16_t)ticks;
	}
	else if (freq>12)
	{
		ticks=750000/(double)freq;
		SET_PRE_8;
		OCR1A=(uint16_t)ticks;
	}
	else if (freq>1.44)
	{
		ticks=93750/(double)freq;
		SET_PRE_64;
		OCR1A=(uint16_t)ticks;
	}
	else if (freq>0.36)
	{
		ticks=23437.5/(double)freq;
		SET_PRE_256;
		OCR1A=(uint16_t)ticks;
	}
	else if (freq>0.09)
	{
		ticks=5859.375/(double)freq;
		SET_PRE_1024;
		OCR1A=(uint16_t)ticks;
	}
	else SET_PRE_0;
	
}