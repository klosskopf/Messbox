/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/


#include "stm32l4xx.h"
#include "spi.h"

void fkt()
{
	send_spi(SPI1, 0x0f);
}

int main(void)
{
	init_spi(SPI1, MASTER, &fkt);
	while(1)
	{
	}
}
