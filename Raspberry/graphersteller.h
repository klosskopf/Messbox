#pragma once

#include <QtCharts>
#include <QObject>
#include <QVector>
#include "mainwindow.h"
class mainWindow;

class Graphersteller : public QObject
{
    Q_OBJECT
public:
    Graphersteller(mainWindow* n_gui);
    void graph_thread();
    void clear_graph();

private:
    mainWindow* gui;
    QVector<QPointF> buffer;
    uint32_t firsttimepoint;
    uint32_t nexttimepoint;
    double maxx,maxy,minx,miny;
public slots:
    void create_graph_rec();
signals:
    void create_graph_send();
};
