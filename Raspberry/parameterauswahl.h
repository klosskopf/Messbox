#ifndef PARAMETERAUSWAHL_H
#define PARAMETERAUSWAHL_H

#include <QWidget>
#include <QScrollArea>
#include <QListWidget>
#include <string>
#include "karte.h"
#include "karte_gui.h"
class Karte;

class Parameterauswahl : public QListWidget
{
    Q_OBJECT
public:
    explicit Parameterauswahl(QWidget *parent = nullptr);
    void addparameter(std::string karte, std::string parameter);
public slots:
    void create_karte(Karte* karte);
    void delete_karte(std::string name);

};

#endif // PARAMETERAUSWAHL_H
