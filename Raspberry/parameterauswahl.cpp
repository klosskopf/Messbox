#include "parameterauswahl.h"

#include <QPushButton>

Parameterauswahl::Parameterauswahl(QWidget *parent) : QListWidget(parent)
{
    setViewMode(QListView::ListMode);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setSortingEnabled(true);
}
