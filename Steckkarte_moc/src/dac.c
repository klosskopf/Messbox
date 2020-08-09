/*
 * dac.c
 *
 *  Created on: 09.08.2020
 *      Author: klosskopf
 */
#include "dac.h"
#include "gpio.h"
#include "stm32l4xx.h"

const GPIO_PIN DAC_OUT={GPIOA,4};

void init_dac()
{
	init_gpio(DAC_OUT,ANALOG,OPEN_DRAIN,OPEN,VERY_HIGH);	//Set pin to analog
	RCC->APB1ENR1 |= RCC_APB1ENR1_DAC1EN;					//enable DAC1 module
	DAC->DHR12R1 = 0;										//start with 0V output
	DAC->CR |= DAC_CR_EN1;									//enable DAC
}

void set_dac(float voltage)
{
	uint32_t result;
	if(voltage>3.3)voltage=3.3;
	if(voltage<0)voltage=0;
	result=voltage*4096/3.3;
	DAC->DHR12R1=result;
}
