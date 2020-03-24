#ifndef SPI_H
#define SPI_H
#include <stdint.h>

class Spi
{
public:
    static void init_spi();
    static void txrx(uint8_t *, uint32_t);
};

#endif // SPI_H
