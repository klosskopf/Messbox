/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/


#include "main.h"
#include "comhandler.h"
#include "parameter.h"
#include "adc.h"
#include "dac.h"
#include "math.h"
#include "flash.h"
#include "stm32l4xx.h"

void L412_80MHz_MSI(void);
void init_sample();

const GPIO_PIN LED ={GPIOB,5};
const GPIO_PIN SAMPLE ={GPIOA,3};

extern float dac_voltage;

int main(void)
{
    __disable_irq();
	L412_80MHz_MSI();
	init_parameter();
	init_comhandler();
	init_adc();			//adc needs a initialized parameterliste[]
	init_flash();
	init_gpio(LED, OUT, PUSH_PULL, OPEN, VERY_HIGH);
	init_gpio(SAMPLE, IN, PUSH_PULL, OPEN, VERY_HIGH);//Probably not needed. I think EXTI samples the pin, not the input
	init_sample();
	init_dac();
	__enable_irq();

	erase_device();
	set_gpio(LED,1);
	while(get_flash_state()!=IDLE);
	set_gpio(LED,0);
	while(1)
	{

	}
}

void led_test(const char* anweisung)
{
	if (strcmp(anweisung,"LED AN")==0)
	{
		set_gpio(LED,1);
	}
	else if (strcmp(anweisung,"LED AUS")==0)
	{
		set_gpio(LED,0);
	}
}

void init_sample()
{
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
	SYSCFG->EXTICR[0] |= (0<<SYSCFG_EXTICR1_EXTI3_Pos);	//Set PA3 to EXTI3
	EXTI->IMR1 |= EXTI_IMR1_IM3;	//Mask the EXTI3 to generate a interrupt
	EXTI->RTSR1 |= EXTI_RTSR1_RT3; 	//and rising edge (A button release)
	EXTI->PR1 = EXTI_PR1_PIF2;		//clear the pending flags
	NVIC_ClearPendingIRQ(EXTI3_IRQn);
	NVIC_SetPriority(EXTI3_IRQn,SAMPLE_PRIO);									//Set the priority
	NVIC_EnableIRQ(EXTI3_IRQn);		//Enable the EXTI0 interrupt if required
}

void EXTI3_IRQHandler(void)
{
	start_conv();
	new_data(DAC_OUTPUT,dac_voltage);
	EXTI->PR1 = EXTI_PR1_PIF3;
}

void L412_80MHz_MSI(void)
{
	FLASH->ACR = (2<<FLASH_ACR_LATENCY_Pos) | FLASH_ACR_PRFTEN;					// Set Flash latency to 2 wait cycles
	while ((FLASH->ACR & FLASH_ACR_LATENCY) != (2<<FLASH_ACR_LATENCY_Pos)); 		// Wait till done

	RCC->CR &= (uint32_t)(~RCC_CR_PLLON);				// Disable PLL. After reset default: RCC->CR &= ~(1<<24)
	while((RCC->CR & RCC_CR_PLLRDY) != 0);				// Wait until PLLRDY is cleared

	RCC->PLLCFGR = 0;
	RCC->PLLCFGR |= (0<<RCC_PLLCFGR_PLLR_Pos);
	RCC->PLLCFGR |= (40<<RCC_PLLCFGR_PLLN_Pos);			// Configure PLL: 4 MHZ (default) *40 /2 (default) = 80 MHz. 40dez = 0x28.
	RCC->PLLCFGR |= (0<<RCC_PLLCFGR_PLLM_Pos);
	RCC->PLLCFGR |= (1<<0);								// take MSI as input for PLL
	RCC->PLLCFGR |= (1<<24);							// activate PLL's clock output

	RCC->CR |= RCC_CR_PLLON; 							// enable PLL: RCC->CR |= (1<<24)
	while ((RCC->CR & RCC_CR_PLLRDY) == 0);				// Wait for PLL ready flag

	RCC->CFGR |= (4<<RCC_CFGR_MCOPRE_Pos);
	RCC->CFGR |= (5<<24);								// MCOSEL[2:0]=101: PLLCLK output
	RCC->CFGR |= (3<<0); 								// use PLL as SYSCLK
}
