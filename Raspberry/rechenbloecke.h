#ifndef TIME_H
#define TIME_H
#include "rechenblock.h"
#include <QPainter>
//#include "control.h"

class Plus_Block : public Rechenblock
{
public:
    Plus_Block():Rechenblock(3)
    {
        QLabel* label=new QLabel();
        label->setPixmap(QPixmap(":images/plus.png").scaled(30, 30));
        setup_Rechenblock(label);
    }
    Plus_Block(Plus_Block*):Rechenblock(3){};
    double get_data(uint32_t time) override;
    Plus_Block* copy() override
    {
        return new Plus_Block();
    }
};

class Minus_Block : public Rechenblock
{
public:
    Minus_Block():Rechenblock(3)
    {
        QLabel* label=new QLabel();
        label->setPixmap(QPixmap(":images/minus.png").scaled(30, 30));
        setup_Rechenblock(label);
    }
    Minus_Block(Minus_Block*):Rechenblock(3){};
    double get_data(uint32_t time) override;
    Minus_Block* copy() override
    {
        return new Minus_Block();
    }
};

class Mal_Block : public Rechenblock
{
public:
    Mal_Block():Rechenblock(3)
    {
        QLabel* label=new QLabel();
        label->setPixmap(QPixmap(":images/mal.png").scaled(30, 30));
        setup_Rechenblock(label);
    }
    Mal_Block(Mal_Block*):Rechenblock(3){};
    double get_data(uint32_t time) override;
    Mal_Block* copy() override
    {
        return new Mal_Block();
    }
};

class Geteilt_Block : public Rechenblock
{
public:
    Geteilt_Block():Rechenblock(3)
    {
        QLabel* label=new QLabel();
        label->setPixmap(QPixmap(":images/geteilt.png").scaled(30, 30));
        setup_Rechenblock(label);
    }
    double get_data(uint32_t time) override;
    Geteilt_Block* copy() override
    {
        return new Geteilt_Block();
    }
};

class Constant_Block : public Rechenblock
{
public:
    Constant_Block():Rechenblock(0)
    {
        constant=1.0001;
        set_constant();
    }
    double constant;
    double get_data(uint32_t time) override;
    void set_constant();
    Constant_Block* copy() override
    {
        return new Constant_Block();
    }
    uint32_t newest() override;
};

class Time_Block : public Rechenblock
{
public:
    Time_Block():Rechenblock(0)
    {
        QLabel* label=new QLabel();
        label->setPixmap(QPixmap(":images/time.png").scaled(30, 30));
        setup_Rechenblock(label);
    }
    double get_data(uint32_t time) override;
    Time_Block* copy() override
    {
        return new Time_Block();
    }
    uint32_t newest() override;
};

class Integrate_Block : public Rechenblock
{
public:
    Integrate_Block():Rechenblock(1)
    {
        lasttime=0;lastausgabe=0;
        QLabel* label=new QLabel();
        label->setPixmap(QPixmap(":images/integrate.png").scaled(30, 30));
        setup_Rechenblock(label);
    }
    double get_data(uint32_t time) override;
    Integrate_Block* copy() override
    {
        return new Integrate_Block();
    }
private:
    double lastausgabe;
    double lasttime;
};

class Derivate_Block : public Rechenblock
{
public:
    Derivate_Block():Rechenblock(1)
    {
        lasteingang=0;lasttime=0;
        QLabel* label=new QLabel();
        label->setPixmap(QPixmap(":images/derivate.png").scaled(30, 30));
        setup_Rechenblock(label);
    }
    double get_data(uint32_t time) override;
    Derivate_Block* copy() override
    {
        return new Derivate_Block();
    }
private:
    double lasteingang;
    double lasttime;
};

#endif // TIME_H
