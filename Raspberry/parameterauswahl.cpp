#include "parameterauswahl.h"

//std::mutex mtxparameterauswahl;           // mutex for critical section

Parameterauswahl::Parameterauswahl(QWidget *parent) : QListWidget(parent)
{
    setViewMode(QListView::ListMode);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setSortingEnabled(true);
}

void Parameterauswahl::create_karte(Karte* karte)
{
   // mtxparameterauswahl.lock();
    if (karte)
    {
        Karte_GUI* karte_gui = new Karte_GUI(karte);
        ListItem* listitem = new ListItem(karte->index);
        karte_gui->listitem = listitem;
        listitem->setSizeHint(QSize(200,100));
        addItem(listitem);
        setItemWidget(listitem, karte_gui);
        karte->karte_gui=karte_gui;
    }
   // mtxparameterauswahl.unlock();
}

void Parameterauswahl::delete_karte(Karte_GUI* karte_gui)
{
   // mtxparameterauswahl.lock();
    removeItemWidget(karte_gui->listitem);
    delete karte_gui;
   // mtxparameterauswahl.unlock();
}
