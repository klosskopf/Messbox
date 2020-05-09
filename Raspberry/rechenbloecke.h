#ifndef TIME_H
#define TIME_H
#include "rechenblock.h"
#include <QPainter>
#include <iostream>
//#include "control.h"

class Axis_Block : public Rechenblock
{
public:
    Axis_Block():Rechenblock(1,0){}
    double get_data(uint32_t time) override;
    QString print() override {return (QString("=").append(eingaenge.front()->print()));}
};

class Plus_Block : public Rechenblock
{
public:
    Plus_Block():Rechenblock(3,2){}
    double get_data(uint32_t time) override;
    QString print() override {return QString("(").append(eingaenge.front()->print()).append("+").append(eingaenge.back()->print()).append(")");}
};

class Minus_Block : public Rechenblock
{
public:
    Minus_Block():Rechenblock(3,2){}
    double get_data(uint32_t time) override;
    QString print() override {return QString("(").append(eingaenge.front()->print()).append("-").append(eingaenge.back()->print()).append(")");}
};

class Mal_Block : public Rechenblock
{
public:
    Mal_Block():Rechenblock(3,3){}
    double get_data(uint32_t time) override;
    QString print() override {return QString("(").append(eingaenge.front()->print()).append("*").append(eingaenge.back()->print()).append(")");}
};

class Geteilt_Block : public Rechenblock
{
public:
    Geteilt_Block():Rechenblock(3,3){}
    double get_data(uint32_t time) override;
    QString print() override {return QString("(").append(eingaenge.front()->print()).append("/").append(eingaenge.back()->print()).append(")");}
};

class Constant_Block : public Rechenblock
{
public:
    Constant_Block():Rechenblock(0,5)
    {
        constant=1.000;
    }
    double get_data(uint32_t time) override;
    uint32_t newest() override;
    QString print() override {return (QString::number(constant));}
private:
    double constant;
};

class Time_Block : public Rechenblock
{
public:
    Time_Block():Rechenblock(0,5){}
    double get_data(uint32_t time) override;
    uint32_t newest() override;
    QString print() override {return (QString("T"));}
};

class Integrate_Block : public Rechenblock
{
public:
    Integrate_Block():Rechenblock(1,4)
    {
        lasttime=0;lastausgabe=0;
    }
    double get_data(uint32_t time) override;
    QString print() override {return(QString("S(").append(eingaenge.front()->print()).append(")"));}
private:
    double lastausgabe;
    double lasttime;
};

class Derivate_Block : public Rechenblock
{
public:
    Derivate_Block():Rechenblock(1,4)
    {
        lasteingang=0;lasttime=0;
    }
    double get_data(uint32_t time) override;
    QString print() override {return(QString("D(").append(eingaenge.front()->print()).append(")"));}
private:
    double lasteingang;
    double lasttime;
};

class Klammerauf_Block : public Rechenblock
{
public:
    Klammerauf_Block():Rechenblock(1,1){}
    double get_data(uint32_t time) override;
    QString print() override {return(eingaenge.front()->print());}
};

class Klammerzu_Block : public Rechenblock
{
public:
    Klammerzu_Block():Rechenblock(0,-1){}
    double get_data(uint32_t time) override;
    QString print() override {return(QString(""));}
};

#endif // TIME_H
