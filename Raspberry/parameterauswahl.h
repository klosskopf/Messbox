#pragma once

#include <QWidget>
#include <QScrollArea>
#include <QListWidget>
#include <string>
#include <mutex>
#include "karte.h"
#include "karte_gui.h"
class Karte;
class Karte_GUI;

class Parameterauswahl : public QListWidget
{
    Q_OBJECT
public:
    explicit Parameterauswahl(QWidget *parent = nullptr);
public slots:
    void create_karte(Karte* karte);
    void delete_karte(Karte_GUI* karte_gui);

};
