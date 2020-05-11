#ifndef RECHENFELD_H
#define RECHENFELD_H

#include <QWidget>
#include <QTextEdit>
#include <QLabel>
#include "rechenblock.h"
#include "parameter.h"
#include <list>

class ParameterViewer;

typedef enum {START,KLAMMERAUF,KLAMMERZU,TIME,INTEGRATE,DERIVATE,ZAHL,PUNKT,PARAMETER}DECODESTATE;
typedef enum {NODECODEERROR,ZUVIELEKLAMMERN,ZUWENIGKLAMMERN,UNGUELTIG,MISSINGPARAMETER, MISSINGSTATE}DECODEERROR;

class Rechenfeld : public QWidget
{
    Q_OBJECT
public:
    explicit Rechenfeld(QWidget *parent = nullptr);
    static Rechenblock* activeblock;
    std::list<Rechenblock*> xbloecke,ybloecke;
private:
    QLabel *xerror,*yerror,*xlabel,*ylabel;
    QTextEdit *xfeld,*yfeld;
    QString zahlstring,parameterstring;
    DECODESTATE state;

    DECODEERROR start(QChar);
    DECODEERROR klammerauf(QChar);
    DECODEERROR klammerzu(QChar);
    DECODEERROR time(QChar);
    DECODEERROR integrate(QChar);
    DECODEERROR derivate(QChar);
    DECODEERROR zahl(QChar);
    DECODEERROR punkt(QChar);
    DECODEERROR parameter(QChar);

    int addblock(Rechenblock* block);
    DECODEERROR decode(QChar);
    ParameterViewer* get_parameter();
private slots:
    void handle_y_input();
    void handle_x_input();


signals:

};

#endif // RECHENFELD_H
