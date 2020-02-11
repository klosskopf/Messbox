#ifndef PARAMETERAUSWAHL_H
#define PARAMETERAUSWAHL_H

#include <QWidget>
#include <QScrollArea>
#include <QListWidget>

class Parameterauswahl : public QListWidget
{
    Q_OBJECT
public:
    explicit Parameterauswahl(QWidget *parent = nullptr);
    void add_karte(QString n_name);
signals:

};

#endif // PARAMETERAUSWAHL_H
