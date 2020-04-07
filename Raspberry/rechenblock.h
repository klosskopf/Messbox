#ifndef RECHENBLOCK_H
#define RECHENBLOCK_H
#include <list>
#include <stdint.h>
#include <QLabel>
#include <QPushButton>
#include <QGridLayout>
#include <QString>

class Rechenblock : public QWidget
{
    Q_OBJECT
public:
    Rechenblock();
    virtual ~Rechenblock();
    std::list<Rechenblock*> eingaenge;
    virtual double get_data(uint32_t time)=0;
    virtual Rechenblock* copy()=0;
    virtual uint32_t newest();

    QLabel* label;
protected:
    void mousePressEvent(QMouseEvent *event) override;
};

#endif // RECHENBLOCK_H
