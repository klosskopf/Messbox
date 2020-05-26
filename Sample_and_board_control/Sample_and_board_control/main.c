/*
 * Sample_and_board_control.c
 *
 * Created: 12.05.2020 22:07:52
 * Author : klosskopf
 */ 

#include <avr/io.h>
#include "global.h"
#include <util/delay.h>
#include <avr/sleep.h>

void Init();
void set_sample_freq(float);
void check_taster();

int main(void)
{
    Init();
    while (1) 
    {
		check_taster();
		_delay_ms(1);
    }
}

void Init()
{
	OSCCAL=0x53;
	
	PORTD |= (1<<CHARGE_EN) | (1<<BOOST_EN) | (1<<RUNNING);
	DDRD |= (1<<CHARGE_EN) | (1<<BOOST_EN) | (1<<RUNNING) | (1<<STATUS_CHARGE) | (1<<STATUS_DONE);
	
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

void check_taster()
{
	static uint16_t tastercnt=LONGTASTERTRIGGER+1;
	
	if (!(PINB & (1<<START_STOPBUTTON)))
	tastercnt=tastercnt==LONGTASTERTRIGGER?tastercnt:tastercnt+1;
	else 
	{
		if (tastercnt==LONGTASTERTRIGGER)
		{
			PORTD ^= (1<<BOOST_EN);
			
			set_sleep_mode(SLEEP_MODE_PWR_DOWN);                       //stop interrupts to ensure the BOD timed sequence executes as required
			sleep_enable();                       //ensure interrupts enabled so we can wake up again
			sleep_cpu();
		}
		else if (tastercnt>SHORTTASTERTRIGGER)
		{
			
		}
		tastercnt=0;
	}
}