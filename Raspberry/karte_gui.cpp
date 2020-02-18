#include "karte_gui.h"

Karte_GUI::Karte_GUI(Karte* karte)
{
    this->karte=karte;
    name = new QLabel(QString::fromUtf8((karte->name).c_str()));
    name->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    parameter = new QComboBox;
    parameter->setEditable(false);
    parameter->setInsertPolicy(QComboBox::NoInsert);
    parameter->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    QObject::connect(parameter, SIGNAL(currentIndexChanged(const QString&)), this, SLOT (parameter_activated(const QString&)));

    wert = new QComboBox;
    wert->setEditable(true);
    wert->setInsertPolicy(QComboBox::NoInsert);
    wert->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    QObject::connect(wert, SIGNAL(currentTextChanged(const QString&)), this, SLOT (wert_activated(const QString&)));

    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(name,10);
    layout->addWidget(parameter,10);
    layout->addWidget(wert,10);
    setLayout(layout);
}

void Karte_GUI::parameter_activated(const QString& name)
{
   set_wert_to_parameter(name.toUtf8().constData());
}

void Karte_GUI::wert_activated(const QString& name)
{

}

void Karte_GUI::set_wert_to_parameter(std::string name)
{
    Parameter* parameter = karte->find_parameter(name);
    if(parameter->parametrierbar == LISTE)
    {
        wert->clear();
        wert->setEditable(false);
        for(std::string moeglichkeit : parameter->auswahlliste)
        wert->addItem(QString::fromStdString(moeglichkeit));
    }
    else if (parameter->parametrierbar == FREI)
    {
        wert->clear();
        wert->setEditable(true);
        for(std::string moeglichkeit : parameter->auswahlliste)
        wert->addItem(QString::fromStdString(moeglichkeit));
    }
    else if ((parameter->parametrierbar == NEIN))
    {
        wert->clear();
        wert->setEditable(false);
        for(std::string moeglichkeit : parameter->auswahlliste)
        wert->addItem(QString::fromStdString(moeglichkeit));
    }
}