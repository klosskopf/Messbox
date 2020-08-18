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
#include "math.h"
#include "stm32l4xx.h"

void L412_80MHz_MSI(void);

int main(void)
{

    __disable_irq();
	L412_80MHz_MSI();
	init_comhandler();
	init_gpio(LED, OUT, PUSH_PULL, OPEN, VERY_HIGH);
	init_gpio(SAMPLE, IN, PUSH_PULL, OPEN, VERY_HIGH);//Probably not needed. I think EXTI samples the pin, not the input
	__enable_irq();

	while(1)
	{

	}
}

void L412_80MHz_MSI(void)
{
	FLASH->ACR = (1<<FLASH_ACR_LATENCY_Pos) | FLASH_ACR_PRFTEN;					// Set Flash latency to 2 wait cycles
	while ((FLASH->ACR & FLASH_ACR_LATENCY) != (1<<FLASH_ACR_LATENCY_Pos)); 		// Wait till done

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
