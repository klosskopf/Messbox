/*
 * adc.h
 *
 * Created: 21.05.2018 12:10:47
 *  Author: klosskopf
 */ 


#ifndef ADC_H_
#define ADC_H_

void adc_setup(void);
volatile uint16_t adc_read(volatile uint8_t kanal);



#endif /* ADC_H_ */