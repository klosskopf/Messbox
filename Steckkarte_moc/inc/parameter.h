/*
 * parameter.h
 *
 *  Created on: 18.03.2020
 *      Author: klosskopf
 */

#ifndef PARAMETER_H_
#define PARAMETER_H_

#include <stdint.h>
#include "flash.h"

typedef struct{
	volatile uint32_t paket_size;
	volatile uint32_t startzeit;
	volatile float daten[FLASHPAGESIZE>>2];
}get_daten_t;

typedef enum {
	NOPARAM=0,
	SPANNUNG_IN=1,
	WIDERSTAND_IN=2,
	SPANNUNG_OUT=3,
	LED_TEST=4,
	MAXPARAMETER=5
}PARAMETER;

typedef enum { STRING, FLOAT } STRING_NOT_FLOAT;
typedef enum { FREI, LISTE, NICHT }PARAMETRIERBAR;

typedef struct{
	get_daten_t eingangsbuffer[2];
	get_daten_t* peingangsbuffer;
	get_daten_t ausgangsbuffer;
	uint32_t eingangsbuffersize;				//This contains the current value of the eingangsbuffer. eingangsbuffer->paket_size is the size, when written to the raspberry
	uint32_t eingangsbufferstartzeit;			//This contains the startzeit of the current eingangsbuffer. It gets written to eingangsbuffer->startzeit, when transmitted
	float lastvalue;
}parameterbuffer_t;

typedef struct{
	const char* name;
	STRING_NOT_FLOAT string_not_float;
	PARAMETRIERBAR parametrierbar;
	float min;
	float max;
	char** wahl;
	uint32_t wahlnr;
	parameterbuffer_t* buffer;
}parameter_t;

void init_parameter();
void new_data(PARAMETER parameter, float data);
void reset_data();
volatile get_daten_t* get_datenblock(PARAMETER parameter);
void set_parameter(uint32_t nummer, const char* wert);

#endif /* PARAMETER_H_ */
