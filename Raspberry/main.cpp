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
    Karte karte1(&window, 1, "Karte 1");
    Parameter parameter1(true,"Spannung",LISTE);
    parameter1.add_auswahl("5");
    parameter1.add_auswahl("3.3");
    Parameter parameter2(true,"Strom",FREI);
    parameter2.add_auswahl("1");
    parameter2.add_auswahl("2");
    parameter2.add_auswahl("3");
    Parameter parameter3(false,"Leistung",NEIN);

    karte1.add_parameter(&parameter1);
    karte1.add_parameter(&parameter2);
    karte1.add_parameter(&parameter3);

    Karte karte2(&window, 2, "Karte 2");
    Karte karte3(&window, 3, "Karte 3");
    Karte karte4(&window, 4, "Karte 4");
    Karte karte5(&window, 5, "Karte 5");
    Karte karte6(&window, 6, "Karte 6");
    Karte karte7(&window, 7, "Karte 7");


    window.show();
    return app.exec();
}
