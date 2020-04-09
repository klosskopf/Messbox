#include "rechenblock.h"
#include <QtWidgets>
#include "control.h"

Rechenblock::Rechenblock(int n_eingaengenr) : eingaengenr(n_eingaengenr)
{
    on_kombinationsfeld=false;
    QObject::connect(this,&Rechenblock::delete_this,Control::gui,&mainWindow::delete_Block);
}

Rechenblock::~Rechenblock()
{
    while (nodes.size())
    {
        Node* node=nodes.front();
        nodes.pop_front();
        delete node;
    }
    if (widget)delete widget;
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

void Rechenblock::setup_Rechenblock(QWidget* n_widget)
{
    int row_count = eingaengenr ? eingaengenr*2-1 : 1;
    QGridLayout* layout = new QGridLayout;
    for (int i=0;i<eingaengenr;i++)
    {
        Node* node=new Node(this);
        layout->addWidget(node,i*2,0,1,1,Qt::AlignRight);
    }
    //n_widget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    layout->addWidget(n_widget,0,1,row_count,1,Qt::AlignCenter);
    Node* node=new Node(this);
    layout->addWidget(node,0,2,1,1,Qt::AlignLeft);

    for (int i=0;i<row_count;i++)layout->setRowStretch(i,1);
    layout->setColumnStretch(0,1);
    layout->setColumnStretch(1,10);
    layout->setColumnStretch(2,1);
    layout->setSpacing(0);
    setLayout(layout);
    widget=n_widget;
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
        if(on_kombinationsfeld) Control::delete_block.push_back(this);
    }
}
