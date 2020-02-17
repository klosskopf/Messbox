#ifndef PARAMETERAUSWAHL_H
#define PARAMETERAUSWAHL_H

#include <QWidget>
#include <QScrollArea>
#include <QListWidget>
#include "karte.h"
#include "karte_gui.h"

class Parameterauswahl : public QListWidget
{
    Q_OBJECT
public:
    explicit Parameterauswahl(QWidget *parent = nullptr);
signals:

};

#endif // PARAMETERAUSWAHL_H
