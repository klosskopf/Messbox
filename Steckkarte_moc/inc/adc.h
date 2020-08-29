/*
 * adc.h
 *
 *  Created on: 08.08.2020
 *      Author: klosskopf
 */

#ifndef ADC_H_
#define ADC_H_

#include "gpio.h"

typedef enum {ADC1_IN0,ADC1_IN1,ADC1_IN2,ADC1_IN3,ADC1_IN4,ADC1_IN5,ADC1_IN6,ADC1_IN7,ADC1_IN8,ADC1_IN9,NOCHANNEL}ADC_CHANNEL;

void init_adc();
void start_conv();
void stop_conv();

#endif /* ADC_H_ */
