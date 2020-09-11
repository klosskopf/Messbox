
#include "global.h"
#include <avr/io.h>
#include <util/delay.h>
#include "adc.h"

void adc_setup(void)
{
	// Set the prescaler to clock/128 & enable ADC
	DIDR0 = 0x3f;
	ADCSRA |= (1 << ADPS0) | (1 << ADPS1) | (1 << ADPS2);
	ADMUX = 0xC0;
	ADCSRA |= (1<<ADEN);
}

volatile uint16_t adc_read(volatile uint8_t kanal)
{
	//Set channel
	ADMUX = 0xC0 | kanal;
		
	// Start the conversion
	ADCSRA |= (1<<ADIF);
	ADCSRA |= (1 << ADSC);

	// Wait for it to finish - blocking
	while(!(ADCSRA & (1 << ADIF)));
	return(ADCL | ADCH<<8);
}
