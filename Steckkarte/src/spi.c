/*
 * spi.c
 *
 *  Created on: 17.03.2020
 *      Author: klosskopf
 */
#include "spi.h"

void init_spi(SPI_TypeDef * spi, SPI_SLAVEMASTER slavemaster, void (*interruptfkt)())
{
	const GPIO_PIN SS1PIN ={GPIOA, 4};
	const GPIO_PIN CLK1PIN ={GPIOA, 1};
	const GPIO_PIN MISO1PIN ={GPIOA, 11};
	const GPIO_PIN MOSI1PIN ={GPIOA, 12};

	const GPIO_PIN SS2PIN ={GPIOB, 9};
	const GPIO_PIN CLK2PIN ={GPIOB, 10};
	const GPIO_PIN MISO2PIN ={GPIOB, 14};
	const GPIO_PIN MOSI2PIN ={GPIOB, 15};

	switch ((int)spi)
	{
	case (int)SPI1:
		init_gpio(SS1PIN, AF5, PUSH_PULL, PULL_UP, LOW);
		init_gpio(CLK1PIN, AF5, PUSH_PULL, OPEN, LOW);
		init_gpio(MISO1PIN, AF5, PUSH_PULL, OPEN, LOW);
		init_gpio(MOSI1PIN, AF5, PUSH_PULL, OPEN, LOW);
		spi1int=interruptfkt;
		RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
		NVIC_SetPriority(SPI1_IRQn, 15);        // set lowest prio
		NVIC_ClearPendingIRQ(SPI1_IRQn);        // clear potentially pending bits
		NVIC_EnableIRQ(SPI1_IRQn);              // enable interrupt in NVIC
		break;
	case (int)SPI2:
		init_gpio(SS2PIN, AF5, PUSH_PULL, OPEN, LOW);
		init_gpio(CLK2PIN, AF5, PUSH_PULL, OPEN, LOW);
		init_gpio(MISO2PIN, AF5, PUSH_PULL, OPEN, LOW);
		init_gpio(MOSI2PIN, AF5, PUSH_PULL, OPEN, LOW);
		RCC->APB1ENR1 |= RCC_APB1ENR1_SPI2EN;
		NVIC_SetPriority(SPI2_IRQn, 15);        // set lowest prio
		NVIC_ClearPendingIRQ(SPI2_IRQn);        // clear potentially pending bits
		NVIC_EnableIRQ(SPI2_IRQn);              // enable interrupt in NVIC
		break;
	default:
		break;
	}

	spi->CR1=0x0;					//configure SPI
	spi->CR2 = 0x1700;	//8bit format
	if (slavemaster == MASTER)
	{
		spi->CR1 |= SPI_CR1_SSM;
		spi->CR1 |= SPI_CR1_SSI;
		spi->CR1 |= SPI_CR1_MSTR;
		spi->CR2 |= SPI_CR2_TXEIE;;
		spi->CR2 |= SPI_CR2_NSSP;
	}
	else
	{
		spi->CR2 |= SPI_CR2_RXNEIE;
	}

	spi->CR1 |= SPI_CR1_SPE;	//start SPI
}

void send_spi(SPI_TypeDef * spi, uint8_t character)
{
	*(uint8_t *)&spi->DR = character;
}

uint8_t read_spi(SPI_TypeDef * spi)
{
	return(spi->DR);
}

void SPI1_IRQHandler()
{
	spi1int();
}
