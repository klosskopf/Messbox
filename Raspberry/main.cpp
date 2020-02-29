#include <QApplication>
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "mainwindow.h"
#include "karte.h"
#include "parameter.h"



int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    mainWindow window;


    Karte* karte1 = new Karte(&window, 1, "Karte 1");
    Parameter* parameter1= new Parameter(true,"Spannung",LISTE);
    parameter1->add_auswahl("5");
    karte1->add_parameter(parameter1);
    delete karte1;

    window.show();
    return app.exec();
}
