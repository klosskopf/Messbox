/*
 * parameter.h
 *
 *  Created on: 18.03.2020
 *      Author: klosskopf
 */

#ifndef PARAMETER_H_
#define PARAMETER_H_

#include "stdint.h"

typedef union
{
	uint8_t binary[0x1000];//4KB block
	float floating[0x400];
}DATENUNION;

typedef struct
{
	uint32_t starttime;
	DATENUNION Data;
}Datenblock;

void init_parameter();
uint32_t database_size(uint32_t);
Datenblock* get_datenblock(uint32_t);
void set_parameter(const char*);

#endif /* PARAMETER_H_ */
