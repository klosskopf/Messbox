#ifndef KOMBINATIONSFELD_H
#define KOMBINATIONSFELD_H

#include <QGroupBox>
#include <QGraphicsView>
#include "rechenblock.h"
#include "rechenbloecke.h"
class Kombinationsfeld;

class Feld : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit Feld(Kombinationsfeld* n_parent):kombinationsfeld(n_parent)
    {
    }
    Kombinationsfeld* kombinationsfeld;
protected:
    void dropEvent(QGraphicsSceneDragDropEvent *event) override;
    void dragEnterEvent(QGraphicsSceneDragDropEvent *event) override;
    void dragMoveEvent(QGraphicsSceneDragDropEvent *event) override;
};

class Kombinationsfeld : public QGroupBox
{
    Q_OBJECT
public:
    explicit Kombinationsfeld(QWidget *parent = nullptr);
    Rechenblock* plus_block;
    Rechenblock* minus_block;
    Rechenblock* mal_block;
    Rechenblock* geteilt_block;
    Rechenblock* constant_block;
    Rechenblock* time_block;
    Rechenblock* integrate_block;
    Rechenblock* derivate_block;
    Rechenblock* x;
    Rechenblock* y;
    QGraphicsView* feld_view;
    Feld* feld;
signals:

};



#endif // KOMBINATIONSFELD_H
