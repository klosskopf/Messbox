#ifndef GRAPHERSTELLER_H
#define GRAPHERSTELLER_H

#include <QObject>
#include <QtCharts>

class Graphersteller : public QObject
{
    Q_OBJECT
public:
    explicit Graphersteller(QObject *parent = nullptr);
    void draw();
signals:
    void create_graph();
};

#endif // GRAPHERSTELLER_H
