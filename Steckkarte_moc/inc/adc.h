/*
 * adc.h
 *
 *  Created on: 08.08.2020
 *      Author: klosskopf
 */

#ifndef ADC_H_
#define ADC_H_

#include "gpio.h"

void init_adc();
void start_conv();
void stop_conv();

#endif /* ADC_H_ */
