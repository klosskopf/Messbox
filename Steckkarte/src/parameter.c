/*
 * parameter.c
 *
 *  Created on: 18.03.2020
 *      Author: klosskopf
 */
#include "parameter.h"

Datenblock ausgabebuffer;

void init_parameter()
{
	for(int i=0; i<0x1000;i++)
	{
		ausgabebuffer.starttime=0x12345678;
		ausgabebuffer.Data.binary[i]=i;
	}
}

void set_parameter(const char* anweisung)
{

}

uint32_t database_size(uint32_t nummer)
{
	return(0x4);
}

Datenblock* get_datenblock(uint32_t nummer)
{
	//for(int i=0; i<0x1000;i++)
	//{
	//	ausgabebuffer.Data.binary[i]=i;
	//}
	return &ausgabebuffer;
}
