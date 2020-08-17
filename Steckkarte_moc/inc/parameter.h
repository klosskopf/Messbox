/*
 * parameter.h
 *
 *  Created on: 18.03.2020
 *      Author: klosskopf
 */

#ifndef PARAMETER_H_
#define PARAMETER_H_

#include <stdint.h>

#define FLASHPAGESIZE 0x1000
#define FLASHPAGECOUNT 800

typedef struct{
	volatile uint32_t paket_size;
	volatile uint32_t startzeit;
	volatile float daten[FLASHPAGESIZE>>2];
}get_daten_t;

typedef enum {
	NOPARAM=0,
	SPANNUNG_IN=1,
	SPANNUNG_OUT=2,
	LED_TEST=3,
	MAXPARAMETER=4
}PARAMETER;

typedef enum { STRING, FLOAT } STRING_NOT_FLOAT;
typedef enum { FREI, LISTE, NICHT }PARAMETRIERBAR;

typedef struct{
	const char* name;
	STRING_NOT_FLOAT string_not_float;
	PARAMETRIERBAR parametrierbar;
	float min;
	float max;
	char** wahl;
	uint32_t wahlnr;
	get_daten_t eingangsbuffer,ausgangsbuffer;
}parameter_t;

void init_parameter();
void new_data(PARAMETER parameter, float data);
void reset_data();
get_daten_t* get_datenblock(PARAMETER parameter);
void set_parameter(uint32_t nummer, const char* wert);

#endif /* PARAMETER_H_ */
