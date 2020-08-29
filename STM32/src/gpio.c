/*
 * gpio.c
 *
 *  Created on: 17.03.2020
 *      Author: klosskopf
 */
#include <gpio.h>

void init_gpio(GPIO_PIN pin, GPIO_TYPE type, GPIO_PUSH_PULL pushpull, GPIO_PULL_DOWN_UP pullupdown, GPIO_SPEED speed)
{
	switch((int)pin.port)
	{
	case (int)GPIOA:
		RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
		break;
	case (int)GPIOB:
		RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
		break;
	case (int)GPIOC:
		RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;
		break;
	case (int)GPIOH:
		RCC->AHB2ENR |= RCC_AHB2ENR_GPIOHEN;
		break;
	}
	pin.port->MODER &= ~(0b11<<(pin.pin<<1));
	switch (type)
	{
	case OUT:
		pin.port->MODER |= (0b01<<(pin.pin<<1));
		break;
	case IN:
		break;
	case AF0 ... AF15:
		if (pin.pin<8)
		{
			pin.port->AFR[0] &= ~(0b1111<<(pin.pin<<2));
			pin.port->AFR[0] |= ((type-AF0)<<(pin.pin<<2));
		}
		else
		{
			pin.port->AFR[1] &= ~(0b1111<<(pin.pin<<2));
			pin.port->AFR[1] |= ((type-AF0)<<((pin.pin-8)<<2));
		}
		pin.port->MODER |= (0b10<<(pin.pin<<1));
		break;
	case ANALOG:
		pin.port->MODER |= (0b11<<(pin.pin<<1));
		break;
	default:
		break;
	}

	pin.port->OTYPER &= ~(0b1<<pin.pin);
	if (pushpull == OPEN_DRAIN) pin.port->OTYPER |= (1<<pin.pin);

	pin.port->PUPDR &= ~(0b11<<(pin.pin<<1));
	switch (pullupdown)
	{
	case PULL_UP:
		pin.port->PUPDR |= (0b01<<(pin.pin<<1));
		break;
	case PULL_DOWN:
		pin.port->PUPDR |= (0b10<<(pin.pin<<1));
		break;
	case OPEN:
		break;
	default:
		break;
	}

	pin.port->OSPEEDR &= ~(0b11<<(pin.pin<<1));
	switch (speed)
	{
	case LOW:
		break;
	case MEDIUM:
		pin.port->OSPEEDR |= (0b01<<(pin.pin<<1));
		break;
	case HIGH:
		pin.port->OSPEEDR |= (0b10<<(pin.pin<<1));
		break;
	case VERY_HIGH:
		pin.port->OSPEEDR |= (0b11<<(pin.pin<<1));
		break;
	default:
		break;
	}
}

void toggle_gpio(GPIO_PIN pin)
{
	pin.port->ODR ^= (1<<pin.pin);
}

void set_gpio(GPIO_PIN pin, bool value)
{
	if (value)
	{
		pin.port->BSRR = (1<<pin.pin);
	}
	else
	{
		pin.port->BSRR = (1<<(pin.pin+16));
	}
}

bool read_gpio(GPIO_PIN pin)
{
	return(pin.port->IDR & (1<<pin.pin));
}
