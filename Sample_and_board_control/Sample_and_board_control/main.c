/*
 * Sample_and_board_control.c
 *
 * Created: 12.05.2020 22:07:52
 * Author : klosskopf
 */ 

#include <avr/io.h>
#include "global.h"

void Init();
void set_sample_freq(float);

int main(void)
{
    Init();
    while (1) 
    {
    }
}

void Init()
{
	OSCCAL=0x53;
	
	PORTD |= (1<<CHARGE_EN) | (1<<BOOST_EN) | (1<<RUNNING);
	DDRD |= (1<<CHARGE_EN) | (1<<BOOST_EN) | (1<<RUNNING);
	
	PORTB |= (1<<START_STOPBUTTON);
	DDRB |= (1<<SAMPLE);
	
	TCCR1A |= (1<<COM1A0) | (1<<WGM11) | (1<<WGM10) ;
	TCCR1B |= (1<<WGM12) | (1<<WGM13) ;
	
	samplefreq=10000;
	set_sample_freq(samplefreq);
	 
}

void set_sample_freq(float freq)
{
	double ticks;
	if (freq==0)SET_PRE_0;
	else if (freq>62)
	{
		ticks=4000000/(double)freq;		//(fosz/2/N) / freq
		SET_PRE_1;
		OCR1A=(uint16_t)ticks;
	}
	else if (freq>8)
	{
		ticks=500000/(double)freq;
		SET_PRE_8;
		OCR1A=(uint16_t)ticks;
	}
	else if (freq>1)
	{
		ticks=62500/(double)freq;
		SET_PRE_64;
		OCR1A=(uint16_t)ticks;
	}
	else if (freq>0.24)
	{
		ticks=15625/(double)freq;
		SET_PRE_256;
		OCR1A=(uint16_t)ticks;
	}
	else if (freq>0.06)
	{
		ticks=3906.25/(double)freq;
		SET_PRE_1024;
		OCR1A=(uint16_t)ticks;
	}
	else SET_PRE_0;
	
}