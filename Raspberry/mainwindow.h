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

using namespace QtCharts;

class mainWindow : public QWidget
{
    Q_OBJECT
public:
    explicit mainWindow(QWidget *parent = nullptr);

private slots:
    void handlestartstopbutton();
    void handlemodebutton();
public:
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
