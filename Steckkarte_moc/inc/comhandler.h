/*
 * comhandler.h
 *
 *  Created on: 18.03.2020
 *      Author: klosskopf
 */

#ifndef COMHANDLER_H_
#define COMHANDLER_H_

#include "spi.h"
#include "stm32l4xx.h"
#include "parameter.h"
#include "string.h"

uint32_t decoderbytenr;

typedef enum
{
	GET_PARAMETER	=0x01,
	SET_PARAMETER	=0x02,
	GET_DATEN		=0x03,
	START_KONT		=0x04,
	START_STARTSTOP	=0x05,
	BEFEHLNR
}BEFEHL;

typedef struct
{
	BEFEHL befehl;
	void (*handler)(uint32_t position, uint8_t datum);
}BEFEHLHANDLER;

BEFEHLHANDLER befehllut[BEFEHLNR-1];

void init_comhandler(SPI_TypeDef *);

#endif /* COMHANDLER_H_ */
