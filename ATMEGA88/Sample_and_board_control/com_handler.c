/*
 * com_handler.c
 *
 * Created: 18.07.2020 14:48:10
 *  Author: klosskopf
 */ 
#include "com_handler.h"
#include "freq_gen.h"
#include "global.h"
#include <avr/io.h>
#include <avr/interrupt.h>

void notmybusiness_decoder(uint32_t position, uint8_t datum);
void start_kont_decoder(uint32_t position, uint8_t datum);
void start_startstop_decoder(uint32_t position, uint8_t datum);
void stop_decoder(uint32_t position, uint8_t datum);
void set_sample_freq_decoder(uint32_t position, uint8_t datum);
void get_status_decoder(uint32_t position, uint8_t datum);

void send_com_char(uint8_t character);
uint8_t read_com();

void init_com()
{
	//Init decoder
	decoderbytenr=0;
	//befehllut[0]=&nofault;
	befehllut[GET_PARAMETER]=&notmybusiness_decoder;
	befehllut[SET_PARAMETER]=&notmybusiness_decoder;
	befehllut[GET_DATEN]=&notmybusiness_decoder;
	befehllut[START_KONT]=&start_kont_decoder;
	befehllut[START_STARTSTOP]=&start_startstop_decoder;
	befehllut[STOP]=&stop_decoder;
	befehllut[SET_SAMPLE_FREQ]=&set_sample_freq_decoder;
	befehllut[GET_STATUS]=&get_status_decoder;
	
	DDRB |= (1<<PINB4);
	
	DDRB &= ~(1<<PINB2);
	PCICR |= (1<<PCIE0);
	PCMSK0 |= (1<<PCINT2);
	
	SPCR |= (1<<SPIE) | (1<<SPE);
	
}

ISR(PCINT0_vect)
{
	if(PINB & (1<<PINB2))
	{
		decoderbytenr=0;
	}
	else
	{
		
	}
}

ISR(SPI_STC_vect)
{
	static BEFEHL befehl;
	uint8_t character=read_com();
	if (decoderbytenr==0)
	{
		befehl=(BEFEHL)character;
	}
	if ((befehl>=BEFEHLNR) || (befehl==0))decoderbytenr=0;
	else befehllut[befehl](++decoderbytenr, character);
}

void notmybusiness_decoder(uint32_t position, uint8_t datum)
{
	
}

void start_kont_decoder(uint32_t position, uint8_t datum)
{
	set_sample_freq(samplefreq);
}

void start_startstop_decoder(uint32_t position, uint8_t datum)
{
	set_sample_freq(samplefreq);
}

void stop_decoder(uint32_t position, uint8_t datum)
{
	set_sample_freq(0);
}
typedef union {uint8_t byte[4];float result;}byte_to_float;
void set_sample_freq_decoder(uint32_t position, uint8_t datum)
{
	static byte_to_float data;
	if (position==1);
	else if (position==2)data.byte[0]=datum;
	else if (position==3)data.byte[1]=datum;
	else if (position==4)data.byte[2]=datum;
	else if (position==5)
	{
		data.byte[3]=datum;
		samplefreq=data.result;
	}
}

extern volatile float status[ADC_MAX];
void get_status_decoder(uint32_t position, uint8_t datum)
{
	volatile uint8_t* pstatus = status;
	send_com_char(pstatus[position-1]);
}

void send_com_char(uint8_t character)
{
	SPDR = ~character;
}

uint8_t read_com()
{
	return(SPDR);
}