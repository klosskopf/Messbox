/*
 * adc.c
 *
 *  Created on: 08.08.2020
 *      Author: klosskopf
 */
#include "adc.h"
#include "stm32l4xx.h"
#include "parameter.h"
#include <stdint.h>

const GPIO_PIN VOLTAGE_IN = {GPIOA,0};

void calibrate_adc();
void wakeup_adc();

void init_adc()
{
	init_gpio(VOLTAGE_IN, ANALOG, OPEN_DRAIN, OPEN, VERY_HIGH);

	RCC->AHB2ENR |= RCC_AHB2ENR_ADCEN; 		// ADC synchronous clock on. RCC_APB2ENR[9]
	RCC->CCIPR |= (3<<28);					// ADC asynchronous clock is system clock
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN; 	// SYSCFG clock on. ...ENR[0]
	wakeup_adc();							// Wakeup ADC
	calibrate_adc();

	ADC1->IER |= ADC_IER_EOCIE;				//Mask the EOC interrupt

	ADC1->CFGR |= ADC_CFGR_JQDIS;			//Disable the injected queue; ***NOT NEEDED***
	ADC1->CFGR2 |= (4<<ADC_CFGR2_OVSS_Pos);	//set oversampling shift to 0
	ADC1->CFGR2 |= (7<<ADC_CFGR2_OVSR_Pos);	//set oversampling ratio to 16
	ADC1->CFGR2 |= ADC_CFGR2_ROVSE;			//enable oversampling

	ADC1->SMPR1 |= (2<<ADC_SMPR1_SMP5_Pos);	//Set 640.5 sampling clocks to ADC1_IN5

	ADC1->SQR1 |= (5<<ADC_SQR1_SQ1_Pos);	//Set ADC1_IN5 to first position in sequence
	ADC1->SQR1 |= (0<<ADC_SQR1_L_Pos);		//Set to 1 conversion in sequence

	ADC1->ISR |= ADC_ISR_ADRDY;				//clear the adc ready bit, to later recheck
	ADC1->CR |= ADC_CR_ADEN;				//start the ADC module
	while(!(ADC1->ISR & ADC_ISR_ADRDY));	//wait for startup
	ADC1->ISR |= ADC_ISR_ADRDY;				//clear for next check (optional)

	NVIC_ClearPendingIRQ(ADC1_2_IRQn);		//set the NVIC for EOC
	NVIC_SetPriority(ADC1_2_IRQn,7);		//must be higher priority than sample
	NVIC_EnableIRQ(ADC1_2_IRQn);			//
}

void start_conv()
{
	ADC1->CR |= ADC_CR_ADSTART;				//start a conversion
}

void stop_conv()
{
	ADC1->CR |= ADC_CR_ADSTP;				//Stop the conversion
	while(ADC1->CR & ADC_CR_ADSTART);		//Wait till stop is done
}

void ADC1_IRQHandler()
{
	uint32_t result=ADC1->DR;				//fetch conversion; clears the EOC flag
	float voltage= (float)result*3.3/0xFFF0;
	new_data(SPANNUNG_IN,(float)voltage);				//store conversion
}

void calibrate_adc()
{
	if ((ADC1->CR & ADC_CR_DEEPPWD) || !(ADC1->CR & ADC_CR_ADVREGEN)) wakeup_adc();	//Before calibration, the ADC must be awake
	if (ADC1->CR & ADC_CR_ADEN)ADC1->CR &= ~ADC_CR_ADEN;							//And disabled ***Wrong procedure***
	ADC1->CR &= ~ADC_CR_ADCALDIF;													//Set to single ended calibration
	ADC1->CR |= ADC_CR_ADCAL;														//Start conversion
	while(ADC1->CR & ADC_CR_ADCAL);													//Wait for calibration finish
}

void wakeup_adc()
{
	ADC1->CR &= ~ADC_CR_DEEPPWD;													//Wakeup the ADC
	ADC1->CR |= ADC_CR_ADVREGEN;													//Start the Regulator
	for (int i=0; i<1600;i++); 														//Wait min. 20us tADCVREG_STUP
}
