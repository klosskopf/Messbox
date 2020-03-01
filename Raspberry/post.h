#ifndef POST_H
#define POST_H

#include <list>
#include "paket.h"
class Paket;

class Post
{
private:
    static std::list<Paket*> Briefkasten;
public:
    static void spi_thread();
    static void send_get_parameter(int index);
};

#endif // POST_H
