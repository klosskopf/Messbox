#include "kombinationsfeld.h"
#include <QDropEvent>
#include <QDragMoveEvent>
#include <QDragEnterEvent>
#include "control.h"

Kombinationsfeld::Kombinationsfeld(QWidget *parent) : QGroupBox("Kennlinienbaukasten",parent)
{
    setAlignment(Qt::AlignHCenter);
    plus_block = new Plus_Block();
    minus_block = new Minus_Block();
    mal_block = new Mal_Block();
    geteilt_block = new Geteilt_Block();
    constant_block = new Constant_Block();
    time_block = new Time_Block();
    integrate_block = new Integrate_Block();
    derivate_block = new Derivate_Block();
    feld = new Feld(this);
    feld_view = new QGraphicsView();
    feld_view->setScene(feld);
    feld_view->setAcceptDrops(true);

    QGridLayout *layout = new QGridLayout;

    layout->addWidget(plus_block,0,0,1,1);
    layout->addWidget(minus_block,0,1,1,1);
    layout->addWidget(mal_block,0,2,1,1);
    layout->addWidget(geteilt_block,0,3,1,1);
    layout->addWidget(constant_block,0,4,1,1);
    layout->addWidget(time_block,0,5,1,1);
    layout->addWidget(integrate_block,0,6,1,1);
    layout->addWidget(derivate_block,0,7,1,1);
    layout->addWidget(feld_view,1,0,1,8);

    layout->setRowStretch(0,1);
    layout->setRowStretch(1,10);
    for (int i=0;i<7;i++)
    {
        layout->setColumnStretch(i,1);
    }
    setLayout(layout);
    setAcceptDrops(true);
    feld->setSceneRect(0,0,1000,300);
}

void Feld::dropEvent(QGraphicsSceneDragDropEvent *event)
{
    if (event->mimeData()->text()=="Rechenblock")
    {
        Rechenblock* na = Control::clipboard->copy();
        kombinationsfeld->feld->addWidget(na);
        na->setGeometry(event->scenePos().x(), event->scenePos().y(), 100,50);
        update();
    }
}
void Feld::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
    if (event->mimeData()->text()=="Rechenblock") event->accept();
}
void Feld::dragMoveEvent(QGraphicsSceneDragDropEvent *event)
{
    if (event->mimeData()->text()=="Rechenblock") event->accept();
}
