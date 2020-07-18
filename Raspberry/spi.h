#ifndef SPI_H
#define SPI_H
#include <stdint.h>

class Spi
{
public:
   // static uint32_t i;
   // static uint8_t befehl;
    static void init_spi(uint64_t baud);
    static void txrx(uint8_t *, uint32_t);
};

#endif // SPI_H
