#include "parameterauswahl.h"
#include "karte.h"

#include <QPushButton>

Parameterauswahl::Parameterauswahl(QWidget *parent) : QListWidget(parent)
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setSortingEnabled(true);
}

void Parameterauswahl::add_karte(QString n_name)
{
    //Karte* karte = new Karte(n_name);
    QListWidgetItem* listitem = new QListWidgetItem(this);
    addItem(listitem);

    QGroupBox* box = new QGroupBox;
    QPushButton* button = new QPushButton("Ahhh");
    QGridLayout* layout = new QGridLayout;
    layout->addWidget(button);
    box->setLayout(layout);

    setItemWidget(listitem, box);
}
