#ifndef GRAPHERSTELLER_H
#define GRAPHERSTELLER_H

#include <QtCharts>
#include <QObject>
#include "mainwindow.h"
class mainWindow;

class Graphersteller : public QObject
{
    Q_OBJECT
public:
    Graphersteller(mainWindow* n_gui):gui(n_gui){time=0;}
    void graph_thread();
private:
    mainWindow* gui;
    void draw_graph();
    uint32_t time;
signals:
    void create_graph(QLineSeries* n_serie);
};

#endif // GRAPHERSTELLER_H
