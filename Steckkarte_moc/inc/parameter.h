/*
 * parameter.h
 *
 *  Created on: 18.03.2020
 *      Author: klosskopf
 */

#ifndef PARAMETER_H_
#define PARAMETER_H_

#include "stdint.h"

typedef struct{
	volatile uint32_t paket_size;
	volatile uint32_t startzeit;
	volatile float daten[0x500];
}get_daten_t;


void init_parameter();
void new_data(float data);
void reset_data();
get_daten_t* get_datenblock(uint32_t);
void set_parameter(uint32_t nummer, const char* wert);

#endif /* PARAMETER_H_ */
