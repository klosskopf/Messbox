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
#include "comhandler.h"

int main(void)
{
	init_parameter();
	init_comhandler(SPI1);
	while(1)
	{

	}
}
