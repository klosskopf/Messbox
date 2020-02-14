#include "karte_gui.h"

Karte_GUI::Karte_GUI(Karte* karte)
{
    this->karte=karte;
    name = new QLabel(QString::fromUtf8((karte->name).c_str()));
    name->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    Parameter = new QComboBox;
    Parameter->setEditable(false);
    Parameter->setInsertPolicy(QComboBox::NoInsert);
    Parameter->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    Wert = new QComboBox;
    Wert->setEditable(true);
    Wert->setInsertPolicy(QComboBox::NoInsert);
    Wert->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(name,10);
    layout->addWidget(Parameter,10);
    layout->addWidget(Wert,10);
    setLayout(layout);
}
