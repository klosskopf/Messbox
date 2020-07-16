/*
 * comhandler.h
 *
 *  Created on: 18.03.2020
 *      Author: klosskopf
 */

#ifndef COMHANDLER_H_
#define COMHANDLER_H_

#include "stm32l4xx.h"

volatile uint32_t decoderbytenr;

typedef enum
{
	GET_PARAMETER	=0x01,
	SET_PARAMETER	=0x02,
	GET_DATEN		=0x03,
	START_KONT		=0x04,
	START_STARTSTOP	=0x05,
	BEFEHLNR        =0x06
}BEFEHL;

void (*befehllut[BEFEHLNR])(uint32_t position, uint8_t datum);

void init_comhandler();

#endif /* COMHANDLER_H_ */
