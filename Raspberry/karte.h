#ifndef KARTE_H
#define KARTE_H
#include <QGroupBox>
#include <QLabel>
#include <QComboBox>
#include <QVBoxLayout>

class Karte : public QGroupBox
{
public:
    Karte(QString n_name);
    QLabel* name;
    QLabel* Parameter;
    QLabel* Wert;
};

#endif // KARTE_H
