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
class Parameterauswahl;

using namespace QtCharts;

class mainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit mainWindow();

private slots:
    void handlestartstopbutton();
    void handlemodebutton();
public slots:
    void draw_graph();
public:
    QWidget* centralwidget;
    QMenuBar* menubar;
    Parameterauswahl* parameterauswahl;
    QPushButton* startstopbutton;
    QPushButton* modebutton;
    QLineEdit* timeframe;
    QPushButton* savebutton;
    QChart* graph;
    QChartView* graphview;
    QGroupBox* kombinationsfeld;
};

#endif // MAINWINDOW_H
