#ifndef SPI_H
#define SPI_H
#include <stdint.h>

#define DATA_BAUD 1000000           //the STM32 are fast. use that
#define CONTROL_BAUD 1000       //The Atmega is mentally a bit slow, so it must be talked to in a slow and clear manner

class Spi
{
public:
   // static uint32_t i;
   // static uint8_t befehl;
    static void init_spi(uint64_t baud);
    static void txrx(uint8_t *, uint32_t);
};

#endif // SPI_H
