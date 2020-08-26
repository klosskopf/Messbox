/*
 * flash.c
 *
 *  Created on: 12.08.2020
 *      Author: Johannes Wittmann
 */

#include "gpio.h"
#include "stm32l4xx.h"
#include "flash.h"
#include "main.h"

typedef enum {WRITE_ENABLE, INSTRUCTION, ADDRESS, DUMMY, DATA, BUSY, END}FlashOperation_t;

GPIO_PIN FLASH_CS = {GPIOB, 12};
GPIO_PIN FLASH_SCK = {GPIOB, 13};
GPIO_PIN FLASH_MISO = {GPIOB, 14};
GPIO_PIN FLASH_MOSI = {GPIOB, 15};
volatile FlashStates_t flash_state;
volatile uint8_t* datap;
volatile uint32_t dest_address;
volatile uint8_t new_byte;

void flash_state_machine();
void read_data_sm();
void erase_data_sm();
void erase_all_sm();
void write_data_sm();

void write_byte(uint8_t data);
void read_dma(uint32_t size, volatile uint8_t* data);
void write_dma(uint32_t size, volatile uint8_t* data);

void init_flash()
{
	flash_state=IDLE;
	set_gpio(FLASH_CS,1);
	init_gpio(FLASH_CS,OUT,PUSH_PULL, PULL_UP, MEDIUM);
	set_gpio(FLASH_CS,1);
	init_gpio(FLASH_SCK,AF5,PUSH_PULL, PULL_DOWN, MEDIUM);
	init_gpio(FLASH_MISO,AF5,PUSH_PULL, PULL_DOWN, MEDIUM);
	init_gpio(FLASH_MOSI,AF5,PUSH_PULL, PULL_DOWN, MEDIUM);

	RCC->APB1ENR1 |= RCC_APB1ENR1_SPI2EN;
	SPI2->CR1 |= (3<<SPI_CR1_BR_Pos);
	SPI2->CR1 |= SPI_CR1_SSM | SPI_CR1_MSTR;				//SPI2 is master with software slave
	SPI2->CR1 |= SPI_CR1_SSI;								//Let the module send
	SPI2->CR2 |= SPI_CR2_FRXTH | (7<<SPI_CR2_DS_Pos);
	//Init DMA
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN;
	DMA1_Channel4->CPAR = (uint32_t)&SPI2->DR;
	DMA1_Channel4->CCR |= (3<<DMA_CCR_PL_Pos) | DMA_CCR_MINC;				//set the CH4 (RX) to high prio; framesize to 8bit; read from peripheral;

	DMA1_Channel5->CCR |= (3<<DMA_CCR_PL_Pos) | DMA_CCR_MINC | DMA_CCR_DIR; 	//set the CH5 (TX) to high prio; framesize to 8bit; read from mem;
	DMA1_Channel5->CPAR = (uint32_t)&SPI2->DR;												//read data from the SPI->DR (please, please be a 8bit operation)

	DMA1_CSELR->CSELR |= (1<<DMA_CSELR_C4S_Pos) | (1<<DMA_CSELR_C5S_Pos);					//Set Channel 4 and 5 of DMA1 to SPI2RX and SPI2TX

	NVIC_SetPriority(DMA1_Channel4_IRQn,FLASH_PRIO);		//Enable RX DMA interrupt, but gets masked before read block
	NVIC_ClearPendingIRQ(DMA1_Channel4_IRQn);
	NVIC_EnableIRQ(DMA1_Channel4_IRQn);

	NVIC_SetPriority(SPI2_IRQn,FLASH_PRIO);				//Enable SPI2 interrupt, but gets masked before send/read single byte; gets triggered by receive either way!
	NVIC_ClearPendingIRQ(SPI2_IRQn);
	NVIC_EnableIRQ(SPI2_IRQn);

	NVIC_SetPriority(DMA1_Channel5_IRQn,FLASH_PRIO);		//Enable TX DMA interrupt, but gets masked before send block
	NVIC_ClearPendingIRQ(DMA1_Channel5_IRQn);
	NVIC_EnableIRQ(DMA1_Channel5_IRQn);
}

FlashStates_t get_flash_state()
{
	return flash_state;
}

void erase_block(volatile uint32_t address)
{
	while(flash_state != IDLE);
	//for (int i=0; i<0x10000; i++);
	flash_state=ERASE_DATA;
	dest_address=address;
	flash_state_machine();
}

void erase_device()
{
	while(flash_state != IDLE);
	//for (int i=0; i<0x10000; i++);
	flash_state=ERASE_ALL;
	flash_state_machine();
}

void write_block(volatile uint32_t address, volatile uint8_t* data)	//writes a 2K block from data to address
{
	while(flash_state != IDLE);
	//for (int i=0; i<0x10000; i++);
	flash_state=WRITE_DATA;
	dest_address=address;
	datap=data;
	flash_state_machine();
}

void read_block(volatile uint32_t address, volatile uint8_t* data)	//reads a 2K block from address to data
{
	while(flash_state != IDLE);
	//for (int i=0; i<0x10000; i++);
	flash_state=READ_DATA;
	dest_address=address;
	datap=data;
	flash_state_machine();
}

void write_byte(uint8_t data)
{
	SPI2->CR2 |= SPI_CR2_RXNEIE;
	*(uint8_t*)&SPI2->DR = data;
	SPI2->CR1 |= SPI_CR1_SPE;
}
void read_dma(uint32_t size, volatile uint8_t* data)
{
	DMA1_Channel4->CMAR=(uint32_t)data;				//set the memory location to write to
	DMA1_Channel5->CMAR=(uint32_t)data;
	DMA1_Channel4->CNDTR=size;						//set the amount to transmit
	DMA1_Channel5->CNDTR=size;
	DMA1_Channel4->CCR |= DMA_CCR_TCIE;				//enable interrupt, when read complete
	SPI2->CR2 |= SPI_CR2_RXDMAEN;					//enable the RXDMA trigger
	DMA1_Channel4->CCR |= DMA_CCR_EN;				//enable rx and tx DMA
	DMA1_Channel5->CCR |= DMA_CCR_EN;
	SPI2->CR2 |= SPI_CR2_TXDMAEN;					//enable TXDMA trigger
	SPI2->CR1 |= SPI_CR1_SPE;						//start the SPI
}
void write_dma(uint32_t size, volatile uint8_t* data)
{
	DMA1_Channel5->CMAR=(uint32_t)data;
	DMA1_Channel5->CNDTR=size;
	DMA1_Channel5->CCR |= DMA_CCR_TCIE;				//enable interrupt, when read complete
	DMA1_Channel5->CCR |= DMA_CCR_EN;
	SPI2->CR2 |= SPI_CR2_TXDMAEN;					//enable TXDMA trigger
	SPI2->CR1 |= SPI_CR1_SPE;						//start the SPI
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
	case ERASE_ALL:
		erase_all_sm();
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
	static volatile FlashOperation_t read_cycle=INSTRUCTION;
	static volatile uint32_t address_count=0;

	switch(read_cycle)
	{
	case INSTRUCTION:
		set_gpio(FLASH_CS,0);
		write_byte(0x0b);
		read_cycle=ADDRESS;
		break;
	case ADDRESS:
		switch(address_count)
		{
		case 0:
			write_byte(dest_address>>16);
			address_count++;
			break;
		case 1:
			write_byte(dest_address>>8);
			address_count++;
			break;
		case 2:
			write_byte(dest_address);
			address_count=0;
			read_cycle=DUMMY;
			break;
		default:
			break;
		}
		break;
	case DUMMY:
		write_byte(0);
		read_cycle=DATA;
		break;
	case DATA:
		read_dma(FLASHPAGESIZE,datap);
		read_cycle=END;
		break;
	case END:
		set_gpio(FLASH_CS,1);
		for(int i=0;i<100;i++);
		read_cycle=INSTRUCTION;
		flash_state=IDLE;
		break;
	default:
		break;
	}
}

void erase_data_sm()
{
	static volatile FlashOperation_t erase_cycle=WRITE_ENABLE;
	static volatile uint32_t address_count=0;

	switch(erase_cycle)
	{
	case WRITE_ENABLE:
		set_gpio(FLASH_CS,0);
		write_byte(0x06);
		erase_cycle=INSTRUCTION;
		break;
	case INSTRUCTION:
		set_gpio(FLASH_CS,1);
		for(int i=0; i<10; i++);
		set_gpio(FLASH_CS,0);
		write_byte(0x20);
		erase_cycle=ADDRESS;
		break;
	case ADDRESS:
		switch(address_count)
		{
		case 0:
			write_byte(dest_address>>16);
			address_count++;
			break;
		case 1:
			write_byte(dest_address>>8);
			address_count++;
			break;
		case 2:
			write_byte(dest_address);
			address_count=0;
			erase_cycle=BUSY;
			break;
		default:
			break;
		}
		break;
	case BUSY:
		switch (address_count)
		{
		case 0:
			set_gpio(FLASH_CS,1);
			for(int i=0; i<10; i++);
			set_gpio(FLASH_CS,0);
			write_byte(0x05);
			address_count=1;
			break;
		case 1:
			write_byte(0);
			address_count=2;
			break;
		case 2:
			if (new_byte&0x03)
			{
				write_byte(0);
			}
			else
			{
				address_count=0;
				set_gpio(FLASH_CS,1);
				for(int i=0;i<100;i++);
				erase_cycle=WRITE_ENABLE;
				flash_state=IDLE;
			}
			break;
		}
		break;
	default:
		break;
	}
}

void erase_all_sm()
{
	static volatile FlashOperation_t erase_cycle=WRITE_ENABLE;
	static volatile uint32_t address_count=0;

	switch(erase_cycle)
	{
	case WRITE_ENABLE:
		set_gpio(FLASH_CS,0);
		write_byte(0x06);
		erase_cycle=INSTRUCTION;
		break;
	case INSTRUCTION:
		set_gpio(FLASH_CS,1);
		for(int i=0; i<10; i++);
		set_gpio(FLASH_CS,0);
		write_byte(0x60);
		erase_cycle=BUSY;
		break;
	case BUSY:
		switch (address_count)
		{
		case 0:
			set_gpio(FLASH_CS,1);
			for(int i=0; i<10; i++);
			set_gpio(FLASH_CS,0);
			write_byte(0x05);
			address_count=1;
			break;
		case 1:
			write_byte(0);
			address_count=2;
			break;
		case 2:
			if (new_byte&0x03)
			{
				write_byte(0);
			}
			else
			{
				address_count=0;
				set_gpio(FLASH_CS,1);
				for(int i=0;i<100;i++);
				erase_cycle=WRITE_ENABLE;
				flash_state=IDLE;
			}
			break;
		}
		break;
	default:
		break;
	}
}

void write_data_sm(){
	static volatile FlashOperation_t write_cycle=WRITE_ENABLE;
	static volatile uint32_t address_count=0;
	static volatile uint32_t write_count=0;

	switch(write_cycle)
	{
	case WRITE_ENABLE:
		set_gpio(FLASH_CS,0);
		write_byte(0x06);
		write_cycle=INSTRUCTION;
		break;
	case INSTRUCTION:
		set_gpio(FLASH_CS,1);
		for(int i=0; i<10; i++);
		set_gpio(FLASH_CS,0);
		write_byte(0x02);
		write_cycle=ADDRESS;
		break;
	case ADDRESS:
		switch(address_count)
		{
		case 0:
			write_byte(dest_address>>16);
			address_count++;
			break;
		case 1:
			write_byte(dest_address>>8);
			address_count++;
			break;
		case 2:
			write_byte(dest_address);
			address_count=0;
			write_cycle=DATA;
			break;
		default:
			break;
		}
		break;
	case DATA:
		write_dma(0x100,datap);
		write_cycle=BUSY;
		break;
	case BUSY:
		switch (address_count)
		{
		case 0:
			set_gpio(FLASH_CS,1);
			for(int i=0; i<10; i++);
			set_gpio(FLASH_CS,0);
			write_byte(0x05);
			address_count=1;
			break;
		case 1:
			write_byte(0);
			address_count=2;
			break;
		case 2:
			if (new_byte&0x03)
			{
				write_byte(0);
			}
			else
			{
				address_count=0;
				set_gpio(FLASH_CS,1);
				for(int i=0;i<100;i++);
				write_cycle=WRITE_ENABLE;
				if (write_count==7)
				{
					write_count=0;
					flash_state=IDLE;
				}
				else
				{
					write_count++;
					dest_address += 0x100;
					datap += 0x100;
					flash_state_machine();
				}
			}
			break;
		}
		break;
	default:
		break;
	}
}

void SPI2_IRQHandler()
{
	SPI2->CR1 &= ~SPI_CR1_SPE;		//disable the SPI
	SPI2->CR2 &= ~SPI_CR2_RXNEIE;	//disable the byte receive interrupt
	new_byte=SPI2->DR;				//read the new byte, clears the interrupt flag
	flash_state_machine();			//look, what comes next
}
void DMA1_CH4_IRQHandler()
{
	uint8_t muell;
	DMA1_Channel4->CCR &= ~DMA_CCR_EN;					//disable the DMA
	DMA1_Channel5->CCR &= ~DMA_CCR_EN;

	while(SPI2->SR & SPI_SR_FTLVL_Msk);					//disable the SPI
	while(SPI2->SR & SPI_SR_BSY);
	SPI2->CR1 &= ~SPI_CR1_SPE;
	while(SPI2->SR & SPI_SR_FRLVL_Msk)muell=SPI2->DR;

	SPI2->CR2 &= ~SPI_CR2_TXDMAEN & ~SPI_CR2_RXDMAEN;	//disable the DMA trigger
	DMA1->IFCR = DMA_IFCR_CGIF4 | DMA_IFCR_CGIF5;		//clear all related interrupt flags
	flash_state_machine();
}
void DMA1_CH5_IRQHandler()
{
	uint8_t muell;
	DMA1_Channel5->CCR &= ~DMA_CCR_EN;					//disable the DMA

	while(SPI2->SR & SPI_SR_FTLVL_Msk);					//disable the SPI
	while(SPI2->SR & SPI_SR_BSY);
	set_gpio(FLASH_CS,1);
	for(int i=0;i<10;i++);
	SPI2->CR1 &= ~SPI_CR1_SPE;
	while(SPI2->SR & SPI_SR_FRLVL_Msk)muell=SPI2->DR;

	SPI2->CR2 &= ~SPI_CR2_TXDMAEN;						//diable the DMA trigger
	DMA1->IFCR = DMA_IFCR_CGIF5;						//clear all related interrupt flags
	flash_state_machine();
}
