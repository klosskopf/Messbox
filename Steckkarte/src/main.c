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

GPIO_PIN clkpin;
GPIO_PIN sspin;
GPIO_PIN mosipin;

void init_fake_spi()
{
	clkpin.port=GPIOA;
	clkpin.pin=6;
	sspin.port=GPIOA;
	sspin.pin=7;
	mosipin.port=GPIOA;
	mosipin.pin=8;
	init_gpio(clkpin, OUT, PUSH_PULL, OPEN, LOW);
	init_gpio(sspin, OUT, PUSH_PULL, OPEN, LOW);
	init_gpio(mosipin, OUT, PUSH_PULL, OPEN, LOW);
	set_gpio(clkpin,false);
	set_gpio(sspin,true);
	set_gpio(mosipin,false);
}

void fake_spi(uint8_t character)
{
	set_gpio(sspin,false);
	if (character&0x80)set_gpio(mosipin,true);
	else set_gpio(mosipin,false);
	for(int i=0; i<8;i++)
	{
		set_gpio(clkpin,true);
		set_gpio(clkpin,false);
		character=character<<1;
		if (character&0x80)set_gpio(mosipin,true);
		else set_gpio(mosipin,false);
	}
	set_gpio(sspin,true);
}

void fakespimsg()
{
	uint8_t befehl = 0x03;

	fake_spi(befehl);	//befehl

	fake_spi(0);		//nummer
	fake_spi(0);
	fake_spi(0);
	fake_spi(1);

	fake_spi(0);		//packetsize
	fake_spi(0);
	fake_spi(0);
	fake_spi(0);

	fake_spi(0);		//startzeit
	fake_spi(0);
	fake_spi(0);
	fake_spi(0);

	for(uint32_t i=0; i<0x4; i++) fake_spi(0);
}

int main(void)
{
	init_parameter();
	init_comhandler(SPI1);

	init_fake_spi();
	while(1)
	{
		fakespimsg();
		for(int i=0; i<1000; i++);
	}
}
