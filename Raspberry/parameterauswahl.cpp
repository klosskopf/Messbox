#include "parameterauswahl.h"

#include <QPushButton>

Parameterauswahl::Parameterauswahl(QWidget *parent) : QListWidget(parent)
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setSortingEnabled(true);
}

void Parameterauswahl::add_karte(Karte* karte)
{
    Karte_GUI* karte_gui = new Karte_GUI(karte);
    ListItem* listitem = new ListItem(karte->index);
    listitem->setSizeHint(QSize(200,97));
    setViewMode(QListView::ListMode);
    addItem(listitem);
    setItemWidget(listitem, karte_gui);
}
