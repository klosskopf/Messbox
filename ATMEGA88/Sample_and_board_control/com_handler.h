/*
 * com_handler.h
 *
 * Created: 18.07.2020 14:59:44
 *  Author: klosskopf
 */ 


#ifndef COM_HANDLER_H_
#define COM_HANDLER_H_

#include <stdint.h>

volatile uint32_t decoderbytenr;

typedef enum
{
	GET_PARAMETER	=0x01,
	SET_PARAMETER	=0x02,
	GET_DATEN		=0x03,
	START_KONT		=0x04,
	START_STARTSTOP	=0x05,
	STOP			=0x06,
	SET_SAMPLE_FREQ	=0x07,
	GET_STATUS		=0x08,
	BEFEHLNR        =0x09
}BEFEHL;

void (*befehllut[BEFEHLNR])(uint32_t position, uint8_t datum);

void init_com();



#endif /* COM_HANDLER_H_ */