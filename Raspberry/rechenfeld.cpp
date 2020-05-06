#include "rechenfeld.h"
#include <QGridLayout>

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
    layout->setColumnStretch(1,20);

    setLayout(layout);
}
