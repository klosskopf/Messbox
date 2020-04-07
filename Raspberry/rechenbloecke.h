#ifndef TIME_H
#define TIME_H
#include "rechenblock.h"
//#include "control.h"

class Rechenblock;

class Plus_Block : public Rechenblock
{
public:
    Plus_Block()
    {
        label=new QLabel("Plus");
        label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        QGridLayout* layout = new QGridLayout();
        layout->addWidget(label);
        this->setLayout(layout);
    }
    Plus_Block(Plus_Block*){};
    ~Plus_Block(){}
    double get_data(uint32_t time) override;
    Plus_Block* copy() override
    {
        return new Plus_Block();
    }
};

class Minus_Block : public Rechenblock
{
public:
    Minus_Block()
    {
        label=new QLabel("Minus");
        label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        QGridLayout* layout = new QGridLayout();
        layout->addWidget(label);
        this->setLayout(layout);
    }
    Minus_Block(Minus_Block*){};
    ~Minus_Block(){}
    double get_data(uint32_t time) override;
    Minus_Block* copy() override
    {
        return new Minus_Block();
    }
};

class Mal_Block : public Rechenblock
{
public:
    Mal_Block()
    {
        label=new QLabel("Mal");
        label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        QGridLayout* layout = new QGridLayout();
        layout->addWidget(label);
        this->setLayout(layout);
    }
    Mal_Block(Mal_Block*){};
    ~Mal_Block(){}
    double get_data(uint32_t time) override;
    Mal_Block* copy() override
    {
        return new Mal_Block();
    }
};

class Geteilt_Block : public Rechenblock
{
public:
    Geteilt_Block()
    {
        label=new QLabel("Geteilt");
        label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        QGridLayout* layout = new QGridLayout();
        layout->addWidget(label);
        this->setLayout(layout);
    }
    ~Geteilt_Block(){}
    double get_data(uint32_t time) override;
    Geteilt_Block* copy() override
    {
        return new Geteilt_Block();
    }
};

class Constant_Block : public Rechenblock
{
public:
    Constant_Block()
    {
        constant=1;
        label=new QLabel("Const.");
        label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        QGridLayout* layout = new QGridLayout();
        layout->addWidget(label);
        this->setLayout(layout);
    }
    ~Constant_Block(){}
    double constant;
    double get_data(uint32_t time) override;
    Constant_Block* copy() override
    {
        return new Constant_Block();
    }
    uint32_t newest() override;
};

class Time_Block : public Rechenblock
{
public:
    Time_Block()
    {
        label=new QLabel("Time");
        label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        QGridLayout* layout = new QGridLayout();
        layout->addWidget(label);
        this->setLayout(layout);
    }
    ~Time_Block(){}
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
    Integrate_Block()
    {
        lasttime=0;lastausgabe=0;
        label=new QLabel("Itegrate");
        label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        QGridLayout* layout = new QGridLayout();
        layout->addWidget(label);
        this->setLayout(layout);
    }
    ~Integrate_Block(){}
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
    Derivate_Block()
    {
        lasteingang=0;lasttime=0;
        label=new QLabel("Derivate");
        label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        QGridLayout* layout = new QGridLayout();
        layout->addWidget(label);
        this->setLayout(layout);
    }
    ~Derivate_Block(){}
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
