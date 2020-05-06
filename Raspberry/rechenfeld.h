#ifndef RECHENFELD_H
#define RECHENFELD_H

#include <QWidget>
#include <QTextEdit>
#include <QLabel>
#include "rechenblock.h"
#include <list>

class Rechenfeld : public QWidget
{
    Q_OBJECT
public:
    explicit Rechenfeld(QWidget *parent = nullptr);
    static Rechenblock* activeblock;
    static std::list<Rechenblock*> xbloecke,ybloecke;
private:
    QLabel *xerror,*yerror,*xlabel,*ylabel;
    QTextEdit *xfeld,*yfeld;
private slots:
    void handle_y_input();
    void handle_x_input();


signals:

};

#endif // RECHENFELD_H
