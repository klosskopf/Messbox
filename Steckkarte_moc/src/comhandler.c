/*
 * comhandler.c
 *
 *  Created on: 18.03.2020
 *      Author: klosskopf
 */
#include "comhandler.h"
#include "parameter.h"
#include "string.h"
#include "gpio.h"

void get_parameter_decoder(uint32_t position, uint8_t datum);
void set_parameter_decoder(uint32_t position, uint8_t datum);
void get_daten_decoder(uint32_t position, uint8_t datum);
void start_kont_decoder(uint32_t position, uint8_t datum);
void start_startstop_decoder(uint32_t position, uint8_t datum);
void stop_decoder(uint32_t position, uint8_t datum);
void set_sample_freq_decoder(uint32_t position, uint8_t datum);

void send_com_char(uint8_t character);
void send_com_block(void* data, uint32_t size);
uint8_t read_com();

const GPIO_PIN SS1PIN ={GPIOB, 0};
const GPIO_PIN CLK1PIN ={GPIOA, 5};
const GPIO_PIN MISO1PIN ={GPIOA, 6};
const GPIO_PIN MOSI1PIN ={GPIOA, 7};

void init_comhandler()
{
//Init decoder
	decoderbytenr=0;
	//befehllut[0]=&nofault;
	befehllut[GET_PARAMETER]=&get_parameter_decoder;
	befehllut[SET_PARAMETER]=&set_parameter_decoder;
	befehllut[GET_DATEN]=&get_daten_decoder;
	befehllut[START_KONT]=&start_kont_decoder;
	befehllut[START_STARTSTOP]=&start_startstop_decoder;
	befehllut[STOP]=&stop_decoder;
	befehllut[SET_SAMPLE_FREQ]=&set_sample_freq_decoder;

//Init pins Interface
	init_gpio(SS1PIN, IN, PUSH_PULL, PULL_UP, VERY_HIGH);
	init_gpio(CLK1PIN, AF5, PUSH_PULL, OPEN, VERY_HIGH);
	init_gpio(MISO1PIN, AF5, PUSH_PULL, OPEN, VERY_HIGH);
	init_gpio(MOSI1PIN, AF5, PUSH_PULL, OPEN, VERY_HIGH);
//Init SS
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
	SYSCFG->EXTICR[0] |= (1<<SYSCFG_EXTICR1_EXTI0_Pos);	//Set PB0 to EXTI0
	EXTI->IMR1 |= EXTI_IMR1_IM0;	//Mask the EXTI0 to generate a interrupt
	EXTI->FTSR1 |= EXTI_FTSR1_FT0; 	//generate trigger on falling edge (A button press)
	EXTI->RTSR1 |= EXTI_RTSR1_RT0; 	//and rising edge (A button release)
	EXTI->PR1 = EXTI_PR1_PIF0;		//clear the pending flags
	NVIC_SetPriority(EXTI0_IRQn,8);									//Set the priority
	NVIC_EnableIRQ(EXTI0_IRQn);		//Enable the EXTI0 interrupt if required
//Init SPI
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
	NVIC_SetPriority(SPI1_IRQn, 15);        // set lowest prio
	NVIC_ClearPendingIRQ(SPI1_IRQn);        // clear potentially pending bits
	NVIC_EnableIRQ(SPI1_IRQn);              // enable interrupt in NVIC
	SPI1->CR1=SPI_CR1_SSM;					//configure SPI as software slave
	SPI1->CR1 |= SPI_CR1_SSI;
	SPI1->CR2 = 0x1700;						//8bit format
	SPI1->CR2 |= SPI_CR2_RXNEIE;
//Init DMA
	RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN;
	DMA1_Channel3->CCR |= (3<<DMA_CCR_PL_Pos) | DMA_CCR_MINC | DMA_CCR_DIR | DMA_CCR_TCIE;	//set the CH3 to high prio; framesize to 8bit; read from mem;
	DMA1_Channel3->CPAR = (uint32_t)&SPI1->DR;								//send data to the SPI->DR (please, please be a 8bit operation)
	DMA1_CSELR->CSELR |= (1<<DMA_CSELR_C3S_Pos);	//Set Channel 2 and 3 of DMA1 to SPIRX and SPITX

	NVIC_SetPriority(DMA1_Channel3_IRQn,0);		//enable ISR for receiving DMA. write and read use both dma.
	NVIC_ClearPendingIRQ(DMA1_Channel3_IRQn);	//the receiving always triggers after the sending dma
	NVIC_EnableIRQ(DMA1_Channel3_IRQn);

	SPI1->CR1 |= SPI_CR1_SPE;	//start SPI
}

void EXTI0_IRQHandler(void)
{
	if (read_gpio(SS1PIN))
	{
		SPI1->CR1 |= SPI_CR1_SSI;
		decoderbytenr=0;
	}
	else
	{
		SPI1->CR1 &= ~SPI_CR1_SSI;
	}
	EXTI->PR1 = EXTI_PR1_PIF0;
}

void SPI1_IRQHandler()
{
	static BEFEHL befehl;
	uint8_t character=read_com();
	if (decoderbytenr==0)
	{
		befehl=(BEFEHL)character;
	}
	if ((befehl>=BEFEHLNR) || (befehl==0))decoderbytenr=0;
	else befehllut[befehl](++decoderbytenr, character);
}

struct get_parameter_t {
	const uint32_t paket_size;
	const char parameter[200];
}get_parameter_data = {200,"Spannungsmesser,1,Spannung,f,n,0,5,2,Messbereich,s,l,0,0{pA,nA,mA,A}3,Strom,f,n,0,0,10,LED Test,s,l,0,0{LED AN,LED AUS}"};
void get_parameter_decoder(uint32_t position, uint8_t datum)
{
	send_com_block(&get_parameter_data,get_parameter_data.paket_size+4);
	decoderbytenr=0;
}

void set_parameter_decoder(uint32_t position, uint8_t datum)
{
	static uint32_t size;
	static uint32_t nummer;
	static char buffer[50];
	if (position==1) {size=0;nummer=0;}
	else if (position==2) size |= (uint32_t)datum;
	else if (position==3) size |= (uint32_t)datum << 8;
	else if (position==4) size |= (uint32_t)datum << 16;
	else if (position==5) size |= (uint32_t)datum << 24;
	else if (position==6) nummer |= (uint32_t)datum;
	else if (position==7) nummer |= (uint32_t)datum<<8;
	else if (position==8) nummer |= (uint32_t)datum<<16;
	else if (position==9) nummer |= (uint32_t)datum<<24;
	else
	{
		if (position > sizeof(buffer))	//A hard fault here would ruin your day
		{
			decoderbytenr=0;
			return;
		}
		buffer[position-10]=datum;
		if (position==size+5)
		{
			buffer[position-9]=0;
			decoderbytenr=0;
			set_parameter(nummer, buffer);
		}
	}
}


void get_daten_decoder(uint32_t position, uint8_t datum)
{
	volatile static uint32_t nummer;
	volatile static get_daten_t* block;
	if (position == 1) nummer=0;
	else if (position == 2) nummer |= datum;
	else if (position == 3) nummer |= datum<<8;
	else if (position == 4) nummer |= datum<<16;
	else if (position == 5)
	{
		nummer |= datum<<24;
		block = get_datenblock(nummer);

		send_com_block(block,block->paket_size+8);
		decoderbytenr=0;
	}
}

void start_kont_decoder(uint32_t position, uint8_t datum)
{
	reset_data();
}

void start_startstop_decoder(uint32_t position, uint8_t datum)
{
	reset_data();
}

void stop_decoder(uint32_t position, uint8_t datum)
{

}

void set_sample_freq_decoder(uint32_t position, uint8_t datum)
{

}

void send_com_char(uint8_t character)
{
	*(uint8_t *)&SPI1->DR = character;
}

void send_com_block(void* data, uint32_t size)
{
	SPI1->CR1 &= ~SPI_CR1_SPE;						//Stop The SPI module

	NVIC_DisableIRQ(SPI1_IRQn);						//When we send via dma, we don't want SPI Interrupts

	DMA1_Channel3->CMAR = (uint32_t)data;			//Enable DMA streams for Tx and Rx in DMA registers, if the streams are used.
	DMA1_Channel3->CNDTR=size;
	DMA1_Channel3->CCR |= DMA_CCR_EN;

	SPI1->CR2 |= SPI_CR2_TXDMAEN;					//Enable DMA Tx buffer in the TXDMAEN bit in the SPI_CR2 register, if DMA Tx is used.

	SPI1->CR1 |= SPI_CR1_SPE;						//Enable the SPI by setting the SPE bit.
	GPIOA->ODR = 1;
}

void DMA1_Channel3_IRQHandler()
{
	uint32_t dummy=0;
	DMA1_Channel3->CCR &= ~DMA_CCR_EN;				//Disable DMA streams for Tx and Rx in the DMA registers, if the streams are used.

	while (SPI1->SR & SPI_SR_FTLVL);				//Disable the SPI by following the SPI disable procedure.
	while(SPI1->SR & SPI_SR_BSY);
	SPI1->CR1 &= ~SPI_CR1_SPE;
	while(SPI1->SR & SPI_SR_FRLVL)dummy=SPI1->DR;
//	for(int i=0;i<1000;i++);

	GPIOA->ODR = 0;

	SPI1->CR2 &= ~SPI_CR2_TXDMAEN;					//Disable DMA Tx and Rx buffers by clearing the TXDMAEN and RXDMAEN bits in the
													//SPI_CR2 register, if DMA Tx and/or DMA Rx are used.

	NVIC_ClearPendingIRQ(SPI1_IRQn);				//restart the SPI interrupts
	NVIC_EnableIRQ(SPI1_IRQn);
	SPI1->CR1 |= SPI_CR1_SPE;

	DMA1->IFCR = DMA_IFCR_CGIF3;					//clear the int. pending flag
}

uint8_t read_com()
{
	return(SPI1->DR);
}

