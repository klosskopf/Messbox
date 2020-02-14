#ifndef PARAMETERAUSWAHL_H
#define PARAMETERAUSWAHL_H

#include <QWidget>
#include <QScrollArea>
#include <QListWidget>
#include "karte.h"
#include "karte_gui.h"
class Karte;

class Parameterauswahl : public QListWidget
{
    Q_OBJECT
public:
    explicit Parameterauswahl(QWidget *parent = nullptr);
    void add_karte(Karte* karte);
signals:

};

#endif // PARAMETERAUSWAHL_H
