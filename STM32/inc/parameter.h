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
#include "adc.h"

typedef struct{
	volatile uint32_t paket_size;
	volatile uint32_t startzeit;
	volatile float daten[FLASHPAGESIZE>>2];
}get_daten_t;

typedef enum {
	NOPARAM,
	SPANNUNG_IN,
	WIDERSTAND_IN,
	DAC_FREQUENCY,
	DAC_MINSPANNUNG,
	DAC_MAXSPANNUNG,
	DAC_OUTPUT,
	LED_TEST,
	MAXPARAMETER
}PARAMETER;

typedef enum { STRING, FLOAT } STRING_NOT_FLOAT;
typedef enum { FREI, LISTE, NICHT }PARAMETRIERBAR;

typedef struct{
	volatile get_daten_t eingangsbuffer[2];
	volatile get_daten_t* peingangsbuffer;
	volatile get_daten_t* pauslagerungsbuffer;
	volatile get_daten_t ausgangsbuffer;
	volatile uint32_t eingangsbuffersize;				//This contains the current value of the eingangsbuffer. eingangsbuffer->paket_size is the size, when written to the raspberry
	volatile uint32_t eingangsbufferstartzeit;			//This contains the startzeit of the current eingangsbuffer. It gets written to eingangsbuffer->startzeit, when transmitted
	volatile float lastvalue;
}parameterbuffer_t;

typedef struct{
	volatile const char* name;
	volatile STRING_NOT_FLOAT string_not_float;
	volatile PARAMETRIERBAR parametrierbar;
	volatile float min;
	volatile float max;
	volatile char** wahl;
	volatile uint32_t wahlnr;
	volatile ADC_CHANNEL channel;
	volatile float offset;				//result=DR * faktor + offset
	volatile float faktor;
	volatile parameterbuffer_t* buffer;
	void(*setfunction)(const char*);
}parameter_t;

void init_parameter();
void new_data(volatile PARAMETER parameter,volatile float data);
void reset_data();
volatile get_daten_t* get_datenblock(volatile PARAMETER parameter);
void set_parameter(volatile uint32_t nummer,volatile const char* wert);

#endif /* PARAMETER_H_ */
