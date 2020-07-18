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

void Init();
void check_taster();

int main(void)
{
	cli();
    Init();
	init_freq();
	DDRB |= (1<<SAMPLE);
	
	init_com();
	sei();
    while (1) 
    {
		//check_taster();
		_delay_ms(1);
    }
}

void Init()
{
	PORTD |= (1<<CHARGE_EN) | (1<<BOOST_EN) | (1<<RUNNING);
	DDRD |= (1<<CHARGE_EN) | (1<<BOOST_EN) | (1<<RUNNING) | (1<<STATUS_CHARGE) | (1<<STATUS_DONE);
	PORTB |= (1<<START_STOPBUTTON);
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

