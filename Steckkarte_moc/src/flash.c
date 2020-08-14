/*
 * flash.c
 *
 *  Created on: 12.08.2020
 *      Author: Johannes Wittmann
 */

#include "gpio.h"
#include "stm32l4xx.h"


typedef enum {IDLE, READ_DATA, ERASE_DATA, WRITE_DATA}FlashStates_t;
typedef enum {WRITE_ENABLE, INSTRUCTION,ADDRESS,DUMMY,DATA,BUSY}FlashOperation_t;

GPIO_PIN FLASH_CS = {GPIOB, 12};
GPIO_PIN FLASH_SCK = {GPIOB, 13};
GPIO_PIN FLASH_MISO = {GPIOB, 14};
GPIO_PIN FLASH_MOSI = {GPIOB, 15};
volatile FlashStates_t flash_state;
volatile uint32_t write_address;

void read_data_sm();
void erase_data_sm();
void write_data_sm();

void init_flash()
{
	flash_state=IDLE;
	set_gpio(FLASH_CS,1);
	init_gpio(FLASH_CS,OUT,PUSH_PULL, OPEN, VERY_HIGH);
	init_gpio(FLASH_SCK,AF5,PUSH_PULL, OPEN, VERY_HIGH);
	init_gpio(FLASH_MISO,AF5,PUSH_PULL, OPEN, VERY_HIGH);
	init_gpio(FLASH_MOSI,AF5,PUSH_PULL, OPEN, VERY_HIGH);

	RCC->APB1ENR1 |= RCC_APB1ENR1_SPI2EN;
	SPI2->CR1 |= (7<<SPI_CR1_BR_Pos);
	SPI2->CR1 |= SPI_CR1_SSM | SPI_CR1_MSTR;				//SPI2 is master with software slave
	SPI2->CR1 |= SPI_CR1_SSI;								//Let the module send
	SPI2->CR2 |= SPI_CR2_FRXTH | (7<<SPI_CR2_DS_Pos) | SPI_CR2_TXDMAEN | SPI_CR2_RXDMAEN;
	//Init DMA
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN;
	DMA1_Channel4->CPAR = (uint32_t)&SPI2->DR;
	DMA1_Channel4->CCR |= (3<<DMA_CCR_PL_Pos) | DMA_CCR_MINC;				//set the CH4 (RX) to high prio; framesize to 8bit; read from peripheral;


	DMA1_Channel5->CCR |= (3<<DMA_CCR_PL_Pos) | DMA_CCR_MINC | DMA_CCR_DIR; 	//set the CH5 (TX) to high prio; framesize to 8bit; read from mem;
	DMA1_Channel5->CPAR = (uint32_t)&SPI2->DR;												//read data from the SPI->DR (please, please be a 8bit operation)

	DMA1_CSELR->CSELR |= (1<<DMA_CSELR_C4S_Pos) | (1<<DMA_CSELR_C5S_Pos);					//Set Channel 4 and 5 of DMA1 to SPI2RX and SPI2TX

	NVIC_SetPriority(DMA1_Channel4_IRQn,2);		//Enable RX DMA interrupt, but gets masked before read block
	NVIC_ClearPendingIRQ(DMA1_Channel4_IRQn);
	NVIC_EnableIRQ(DMA1_Channel4_IRQn);

	NVIC_SetPriority(SPI2_IRQn,2);				//Enable SPI2 interrupt, but gets masked before send/read single byte; gets triggered by receive either way!
	NVIC_ClearPendingIRQ(SPI2_IRQn);
	NVIC_EnableIRQ(SPI2_IRQn);

	NVIC_SetPriority(DMA1_Channel5_IRQn,2);		//Enable TX DMA interrupt, but gets masked before send block
	NVIC_ClearPendingIRQ(DMA1_Channel5_IRQn);
	NVIC_EnableIRQ(DMA1_Channel5_IRQn);
	SPI2->CR1 |= SPI_CR1_SPE;
}



void write_block(uint32_t address, uint8_t* data)	//writes a 2K block from data to address
{
	flash_state=ERASE_DATA;
	set_gpio(FLASH_CS,0);
	SPI2->CR2 |= SPI_CR2_RXNEIE;
	*(uint8_t*)&SPI2->DR = 0x06;
	write_address=address;

	DMA1_Channel5->CMAR=(uint32_t)data;
}



void read_block(uint32_t address, uint8_t* data)	//reads a 2K block from address to data
{
	flash_state=READ_DATA;
	set_gpio(FLASH_CS,0);
	SPI2->CR2 |= SPI_CR2_RXNEIE;
	*(uint8_t*)&SPI2->DR = 0x3b;
	*(uint8_t*)&SPI2->DR = address>>16;
	*(uint8_t*)&SPI2->DR = address>>8;
	*(uint8_t*)&SPI2->DR = address;

	DMA1_Channel4->CMAR=(uint32_t)data;
	DMA1_Channel5->CMAR=(uint32_t)data;
}



void flash_state_machine()
{
	switch(flash_state)
	{
	case READ_DATA:
		read_data_sm();
		break;
	case ERASE_DATA:
		erase_data_sm();
		break;
	case WRITE_DATA:
		write_data_sm();
		break;
	default:
		break;
	}
}

void read_data_sm()
{
	uint8_t muell;
	static volatile FlashOperation_t read_cycle=INSTRUCTION;
	static uint32_t address_count;

	if (read_cycle==INSTRUCTION)
	{
		muell=SPI2->DR;
		address_count=1;
		read_cycle=ADDRESS;
	}
	else if (read_cycle==ADDRESS)
	{
		muell=SPI2->DR;
		if (address_count==3)
		{
			read_cycle=DUMMY;
			*(uint8_t*)&SPI2->DR = 0;
		}
		else address_count++;
	}
	else if (read_cycle==DUMMY)
	{
		muell=SPI2->DR;
		SPI2->CR2 &= ~SPI_CR2_RXNEIE;					//stop the interrupt after every byte
		DMA1->IFCR = DMA_IFCR_CGIF4 | DMA_IFCR_CGIF5;	//clear all related interrupt flags

		DMA1_Channel4->CNDTR = 0xfff;					//read 4KByte
		DMA1_Channel4->CCR |= DMA_CCR_TCIE;				//enable interrupt, when read complete
		DMA1_Channel4->CCR |= DMA_CCR_EN;				//start the read

		DMA1_Channel5->CNDTR = 0xfff;					//create 4Kbyte pulses
		DMA1_Channel5->CCR |= DMA_CCR_EN;				//start the pulses

		read_cycle=DATA;
	}
	else if (read_cycle==DATA)
	{
		DMA1_Channel5->CCR &= ~DMA_CCR_EN;				//disable the TX DMA
		DMA1_Channel4->CCR &= ~DMA_CCR_EN;				//disable the RX DMA
		DMA1_Channel4->CCR &= ~DMA_CCR_TCIE;			//disable interrupt
		DMA1->IFCR = DMA_IFCR_CGIF4 | DMA_IFCR_CGIF5;	//clear all related interrupt flags
		set_gpio(FLASH_CS,1);
		read_cycle=INSTRUCTION;
	}
}

void erase_data_sm()
{
	volatile uint8_t muell;
	static volatile FlashOperation_t erase_cycle=WRITE_ENABLE;
	static volatile uint32_t count;

	if (erase_cycle == WRITE_ENABLE)
	{
		set_gpio(FLASH_CS,1);
		muell= SPI2->DR;
		erase_cycle=INSTRUCTION;
		set_gpio(FLASH_CS,0);
		*(uint8_t*)&SPI2->DR = 0x20;
		*(uint8_t*)&SPI2->DR = write_address>>16;
		*(uint8_t*)&SPI2->DR = write_address>>8;
		*(uint8_t*)&SPI2->DR = write_address;
	}
	else if (erase_cycle==INSTRUCTION)
	{
		muell=SPI2->DR;
		count=1;
		erase_cycle=ADDRESS;
	}
	else if (erase_cycle==ADDRESS)
	{
		muell=SPI2->DR;
		if (count==3)
		{
			set_gpio(FLASH_CS,1);
			erase_cycle=BUSY;
			count=0;
			SPI2->CR2 |= SPI_CR2_RXNEIE;	//enable the single byte interrupt
			set_gpio(FLASH_CS,0);
			*(uint8_t*)&SPI2->DR = 0x05;	//Read the status register 1
		}
		else count++;
	}
	else if (erase_cycle==BUSY)
	{
		volatile static uint8_t oldmuell;
		muell=SPI2->DR;
		if (count==0)
		{
			count=1;
			*(uint8_t*)&SPI2->DR = 0;
		}
		else
		{
			if (muell & 0x01)
			{
				*(uint8_t*)&SPI2->DR = 0x35;
			}
			else
			{
				set_gpio(FLASH_CS,1);
				//flash_state=WRITE_DATA;
				//erase_cycle=WRITE_ENABLE;

				//SPI2->CR2 &= ~SPI_CR2_RXNEIE;
				//set_gpio(FLASH_CS,0);
				//*(uint8_t*)&SPI2->DR = 0x06;
			}
		}
		oldmuell=muell;
	}
}

void write_data_sm()
{
	volatile uint8_t muell;
	static volatile FlashOperation_t write_cycle=INSTRUCTION;
	static volatile uint32_t count;

	if(write_cycle==WRITE_ENABLE)
	{
		set_gpio(FLASH_CS,1);
		muell= SPI2->DR;
		write_cycle=INSTRUCTION;
		set_gpio(FLASH_CS,0);
		*(uint8_t*)&SPI2->DR = 0x20;
		*(uint8_t*)&SPI2->DR = write_address>>16;
		*(uint8_t*)&SPI2->DR = write_address>>8;
		*(uint8_t*)&SPI2->DR = write_address;
		write_address+=0xFF;
	}
	else if (write_cycle==INSTRUCTION)
	{
		muell=SPI2->DR;
		count=1;
		write_cycle=ADDRESS;
	}
	else if (write_cycle==ADDRESS)
	{
		muell=SPI2->DR;
		if (count==3)
		{
			write_cycle=DATA;

			SPI2->CR2 &= ~SPI_CR2_RXNEIE;					//stop the interrupt after every byte
			DMA1->IFCR = DMA_IFCR_CGIF4 | DMA_IFCR_CGIF5;	//clear all related interrupt flags

			DMA1_Channel5->CCR |= DMA_CCR_TCIE;				//enable interrupt, when write complete
			DMA1_Channel5->CNDTR = 0xff;					//write 256 bytes
			DMA1_Channel5->CCR |= DMA_CCR_EN;				//start the write

		}
		else count++;
	}
	else if (write_cycle==DATA)
	{
		set_gpio(FLASH_CS,1);
		DMA1_Channel5->CCR &= ~DMA_CCR_EN;				//disable the TX DMA
		DMA1_Channel5->CCR &= ~DMA_CCR_TCIE;			//disable the interrupt
		DMA1->IFCR = DMA_IFCR_CGIF4 | DMA_IFCR_CGIF5;	//clear all related interrupt flags

		write_cycle=BUSY;

		set_gpio(FLASH_CS,0);
	}
	else if (write_cycle==BUSY)
	{
		muell=SPI2->DR;
		if (count==0)
		{
			count=1;
			*(uint8_t*)&SPI2->DR = 0;
		}
		else
		{
			if (muell & 0x01)
			{
				*(uint8_t*)&SPI2->DR = 0;
			}
			else
			{
				set_gpio(FLASH_CS,1);
				write_cycle=WRITE_ENABLE;
				set_gpio(FLASH_CS,0);
				*(uint8_t*)&SPI2->DR = 0x06;
			}
		}
	}
}


void SPI2_IRQHandler(){flash_state_machine();}
void DMA1_CH4_IRQHandler(){flash_state_machine();}
