#ifndef RECHENFELD_H
#define RECHENFELD_H

#include <QWidget>
#include <QTextEdit>
#include <QLabel>

class Rechenfeld : public QWidget
{
    Q_OBJECT
public:
    explicit Rechenfeld(QWidget *parent = nullptr);
private:
    QLabel *xerror,*yerror,*xlabel,*ylabel;
    QTextEdit *xfeld,*yfeld;

signals:

};

#endif // RECHENFELD_H
