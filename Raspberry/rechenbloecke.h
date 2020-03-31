#ifndef TIME_H
#define TIME_H
#include "rechenblock.h"
#include "control.h"

class Time : public Rechenblock
{
public:
    Time(){}
    ~Time(){}
    virtual double get_data(uint32_t time) override;
    virtual uint32_t newest() override;
};

#endif // TIME_H
