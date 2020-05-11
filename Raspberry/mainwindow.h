#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QMenuBar>
#include <QListWidget>
#include <QLineEdit>
#include <QtCharts>
#include <QGridLayout>
#include "parameterauswahl.h"
#include <stdio.h>
#include <string>
#include <QPalette>
#include <QString>
#include "control.h"
#include "rechenfeld.h"
#include "graphersteller.h"
class Parameterauswahl;
class Graphersteller;
class Rechenfeld;

using namespace QtCharts;

class mainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit mainWindow();

private slots:
    void handlestartstopbutton();
    void handlemodebutton();
    void handletimframe();
    void handlesample();
public slots:
    void create_graph(QLineSeries* n_serie);
public:
    QWidget* centralwidget;
    QMenuBar* menubar;
    Parameterauswahl* parameterauswahl;
    QPushButton* startstopbutton;
    QPushButton* modebutton;
    QLineEdit* timeframe;
    QLineEdit* sample;
    QPushButton* savebutton;
    QChart* graph;
    QLineSeries* anzeigeserie;
    QLineSeries* aenderserie;
    QChartView* graphview;
    Rechenfeld* rechenfeld;

    Graphersteller* graphersteller;
};

#endif // MAINWINDOW_H
