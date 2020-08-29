/*
 * dac.c
 *
 *  Created on: 09.08.2020
 *      Author: klosskopf
 */
#include "dac.h"
#include "gpio.h"
#include "stm32l4xx.h"
#include <string.h>
#include "main.h"
#include <stdlib.h>

const GPIO_PIN DAC_OUT={GPIOA,4};
volatile float dac_voltage;

volatile float frequency=0;
volatile float maxvoltage=0;
volatile float minvoltage=0;

void init_dac()
{
	dac_voltage=0;
	init_gpio(DAC_OUT,ANALOG,OPEN_DRAIN,OPEN,VERY_HIGH);	//Set pin to analog
	RCC->APB1ENR1 |= RCC_APB1ENR1_DAC1EN;					//enable DAC1 module
	DAC->DHR12R1 = 0;										//start with 0V output
	DAC->CR |= DAC_CR_EN1;									//enable DAC

	RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN;
	TIM2->ARR = 79999;
	TIM2->CR1 |= TIM_CR1_ARPE;
	TIM2->DIER |= TIM_DIER_UIE;
	TIM2->CNT = 0;
	TIM2->CR1 |= TIM_CR1_CEN;
	NVIC_SetPriority(TIM2_IRQn,DAC_PRIO);
	NVIC_ClearPendingIRQ(TIM2_IRQn);
	NVIC_EnableIRQ(TIM2_IRQn);

}

void TIM2_IRQHandler()
{
	static float time = 0;
	time+=0.001;

	if (time > 1/frequency)
	{
		dac_voltage=minvoltage;
		time=0;
	}
	else if (time>1/frequency/2)
	{
		dac_voltage=-2*(maxvoltage-minvoltage)*frequency*time+2*maxvoltage-minvoltage;
	}
	else
	{
		dac_voltage=2*(maxvoltage-minvoltage)*frequency*time+minvoltage;
	}
	set_dac(dac_voltage);
	TIM2->SR &= ~TIM_SR_UIF;
}

void set_dac(float voltage)
{
	dac_voltage=voltage;
	uint32_t result;
	if(voltage>3.3)voltage=3.3;
	if(voltage<0)voltage=0;
	result=voltage*4096/3.3*1.098901;
	DAC->DHR12R1=result;
}

void set_dac_frequency(const char* anweisung)
{
	frequency=atof(anweisung);
}

void set_dac_minvoltage(const char* anweisung)
{
	minvoltage=atof(anweisung);
}

void set_dac_maxvoltage(const char* anweisung)
{
	maxvoltage=atof(anweisung);
}
