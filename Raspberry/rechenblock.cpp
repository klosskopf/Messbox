#include "rechenblock.h"
#include <QtWidgets>
#include "control.h"

Rechenblock::Rechenblock()
{

}

Rechenblock::~Rechenblock()
{
    if (label)delete label;
}

uint32_t Rechenblock::newest()
{
    uint32_t newest=-1;
    for (Rechenblock* block : eingaenge)
    {
        if (block->newest() < newest)
            newest=block->newest();
    }
    if (eingaenge.size()==0) newest=0;
    return newest;
}

void Rechenblock::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        Control::clipboard=this;
        QDrag *drag = new QDrag(this);
        QMimeData *mimeData = new QMimeData;

        mimeData->setText("Rechenblock");
        drag->setMimeData(mimeData);
        //drag->setPixmap(iconPixmap);

        Qt::DropAction dropAction = drag->exec();
    }
    else if (event->button() == Qt::RightButton)
    {
        Control::delete_block.push_back(this);
    }
}
