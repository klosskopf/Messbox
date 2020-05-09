#include "control.h"
#include <QGridLayout>
#include "rechenfeld.h"

Rechenfeld::Rechenfeld(QWidget *parent) : QWidget(parent)
{
    QGridLayout *layout = new QGridLayout;

    xerror = new QLabel("Keine gültige Kennlinie");
    yerror = new QLabel("Keine gültige Kennlinie");
    xlabel = new QLabel("x=");
    ylabel = new QLabel("y=");
    xfeld = new QTextEdit();
    yfeld = new QTextEdit();

    layout->addWidget(xerror,0,0,1,2);
    layout->addWidget(yerror,2,0,1,2);
    layout->addWidget(xlabel,1,0,1,1);
    layout->addWidget(ylabel,3,0,1,1);
    layout->addWidget(xfeld,1,1,1,1);
    layout->addWidget(yfeld,3,1,1,1);

    layout->setRowStretch(0,1);
    layout->setRowStretch(1,10);
    layout->setRowStretch(2,1);
    layout->setRowStretch(3,10);

    layout->setColumnStretch(0,1);
    layout->setColumnStretch(1,30);

    setLayout(layout);

    connect(xfeld, SIGNAL (textChanged()),this, SLOT (handle_x_input()));
    connect(yfeld, SIGNAL (textChanged()),this, SLOT (handle_y_input()));
}


int Rechenfeld::addblock(Rechenblock* block)
{
    xbloecke.push_back(block);
    return(Rechenfeld::activeblock->von_unten(block));
}

void Rechenfeld::handle_x_input()   //This needs a mutex
{
    while (Rechenfeld::xbloecke.size())
    {
        Rechenblock* block = Rechenfeld::xbloecke.front();
        Rechenfeld::xbloecke.pop_front();
        delete block;
    }
    Control::xAchse->eingaenge.clear();
    Rechenfeld::activeblock=Control::xAchse;

    addblock(new Time_Block());
    addblock(new Mal_Block());
    addblock(new Klammerauf_Block());
    addblock(new Time_Block());
    addblock(new Plus_Block());
    addblock(new Constant_Block());
    addblock(new Klammerzu_Block());
    addblock(new Plus_Block());
    addblock(new Time_Block());
    addblock(new Mal_Block());
    addblock(new Integrate_Block());
    addblock(new Klammerauf_Block());
    addblock(new Klammerauf_Block());
    addblock(new Constant_Block());
    addblock(new Minus_Block());
    addblock(new Time_Block());
    addblock(new Klammerzu_Block());
    addblock(new Mal_Block());
    addblock(new Constant_Block());
    addblock(new Klammerzu_Block());

    xerror->setText(Control::xAchse->print());
}

void Rechenfeld::handle_y_input()
{

}

Rechenblock* Rechenfeld::activeblock=NULL;
std::list<Rechenblock*> Rechenfeld::xbloecke,ybloecke;
