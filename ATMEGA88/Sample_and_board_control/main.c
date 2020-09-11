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
#include <avr/interrupt.h>
#include "freq_gen.h"
#include "com_handler.h"
#include "adc.h"

void Init();
void check_taster();
void read_status();

volatile float status[ADC_MAX];

int main(void)
{
	cli();
	adc_setup();
    Init();
	init_freq();
	DDRB |= (1<<SAMPLE);
	
	init_com();
	sei();
    while (1) 
    {
		read_status();
		if (status[ADC_VBAT]<3) PORTD &= ~(1<<BOOST_EN);
		else PORTD |= (1<<BOOST_EN);
		check_taster();
		_delay_ms(1);
    }
}

void Init()
{
	status[ADC_5V]=5;
	status[ADC_3V3]=3.3;
	status[ADC_ICHARGE]=0;
	status[ADC_VBAT]=4.2;
	status[ADC_VLADE]=5;
	status[ADC_VIN]=12;
	
	PORTD |= (1<<CHARGE_EN) | (1<<BOOST_EN) | (1<<RUNNING);
	DDRD |= (1<<CHARGE_EN) | (1<<BOOST_EN) | (1<<RUNNING) | (1<<STATUS_CHARGE) | (1<<STATUS_DONE);
	PORTB |= (1<<START_STOPBUTTON);
}

void read_status()
{
	volatile static uint8_t currentchannel=0;
	volatile uint16_t binaryvalue = adc_read(currentchannel);
	volatile float floatvalue;
	switch (currentchannel)
	{
		case ADC_5V:
			floatvalue=(float)binaryvalue*1.1/0x400/10*(10+82);
			break;
		case ADC_3V3:
			floatvalue=(float)binaryvalue*1.1/0x400/10*(10+82);
			break;
		case ADC_ICHARGE:
			floatvalue=(float)binaryvalue*1.1/0x400/10*(10+10);
			break;
		case ADC_VBAT:
			floatvalue=(float)binaryvalue*1.1/0x400/10*(10+82);
			break;
		case ADC_VLADE:
			floatvalue=5;
			break;
		case ADC_VIN:
			floatvalue=12;
			break;
		default:
			return;
			break;
	}
	status[currentchannel]=floatvalue;
	currentchannel++;
	if (currentchannel==ADC_MAX) currentchannel=0;
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

