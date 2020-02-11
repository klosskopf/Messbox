#include "karte.h"

Karte::Karte(QString n_name)
{
    name = new QLabel(n_name);
    name->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    Parameter = new QLabel("Oh mein");
    Parameter->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    Wert = new QLabel("Gott");
    Wert->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(name,10);
    layout->addWidget(Parameter,10);
    layout->addWidget(Wert,10);
    setLayout(layout);
}
