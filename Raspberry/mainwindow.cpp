#include "mainwindow.h"
#include <stdio.h>
#include <string>
#include <QString>

mainWindow::mainWindow(QWidget *parent) : QWidget(parent)
{
    setGeometry(0,0,800,500);

    #include <QtCharts>

    menubar= new QMenuBar;
    parameterauswahl = new Parameterauswahl();
    startstopbutton = new QPushButton;
    startstopbutton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    modebutton = new QPushButton;
    modebutton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    timeframe = new QLineEdit;
    timeframe->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    savebutton = new QPushButton;
    savebutton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    graph = new QChart;
    graph->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    graphview = new QChartView;
    graphview->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    graphview->setChart(graph);
    kombinationsfeld = new QGroupBox;
    kombinationsfeld->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    for (int i=0;i<10;i++)
    {
        QString text("Karte");
        text.append(QString::number(100-i));
        parameterauswahl->add_karte(text);
    }

   // parameterauswahl->

    QGridLayout *mainLayout = new QGridLayout;

    mainLayout->addWidget(parameterauswahl,0,0,10,3);
    mainLayout->addWidget(startstopbutton,0,3,1,1);
    mainLayout->addWidget(modebutton,1,3,1,1);
    mainLayout->addWidget(timeframe,2,3,1,1);
    mainLayout->addWidget(savebutton,5,3,1,1);
    mainLayout->addWidget(graphview,0,4,6,6);
    mainLayout->addWidget(kombinationsfeld,6,3,4,7);
    for (int i=0;i<10;i++)
    {
        mainLayout->setRowStretch(i,10);
        mainLayout->setColumnStretch(i,10);
    }
    // Add the previous two inner layouts


    // Set the outer layout as a main layout
    // of the widget
    this->setLayout(mainLayout);

    // Window title
    setWindowTitle("Buttonbox");
}
