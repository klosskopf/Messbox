#ifndef CONTROL_H
#define CONTROL_H

#include <QObject>
#include <list>
#include <string>
#include "karte.h"
class Karte;

class Control : public QObject
{
    Q_OBJECT
public:
    explicit Control(QObject *parent = nullptr);
    void controlThread();
    std::list<Karte*> Kartenset;
signals:
    void let_create_karte(Karte* karte);
    void let_delete_karte(std::string name);
};

#endif // CONTROL_H
