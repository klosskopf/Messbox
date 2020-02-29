#include "parameterauswahl.h"

#include <QPushButton>

Parameterauswahl::Parameterauswahl(QWidget *parent) : QListWidget(parent)
{
    setViewMode(QListView::ListMode);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setSortingEnabled(true);
}

void Parameterauswahl::create_karte(Karte* karte)
{
    Karte_GUI* karte_gui = new Karte_GUI(karte);
    ListItem* listitem = new ListItem(karte->index);
    listitem->setSizeHint(QSize(200,97));
    addItem(listitem);
    setItemWidget(listitem, karte_gui);
}

