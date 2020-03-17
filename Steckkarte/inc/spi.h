/*
 * spi.h
 *
 *  Created on: 17.03.2020
 *      Author: klosskopf
 */

#ifndef SPI_H_
#define SPI_H_

#include "stm32l4xx.h"
#include "gpio.h"

typedef enum {SLAVE, MASTER}SPI_SLAVEMASTER;

void init_spi(SPI_TypeDef * spi, SPI_SLAVEMASTER, void (*interruptfkt)());
void send_spi(SPI_TypeDef * spi, uint8_t character);
uint8_t read_spi(SPI_TypeDef * spi);

void (* spi1int)();
void (* spi2int)();

#endif /* SPI_H_ */
