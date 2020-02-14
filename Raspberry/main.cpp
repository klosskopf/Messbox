#include <QApplication>
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "mainwindow.h"
#include "karte.h"



int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    mainWindow window;

    Karte karte1(window.parameterauswahl, 1, "Karte 1");
    Karte karte2(window.parameterauswahl, 2, "Karte 2");
    Karte karte3(window.parameterauswahl, 3, "Karte 3");
    Karte karte4(window.parameterauswahl, 4, "Karte 4");
    Karte karte5(window.parameterauswahl, 5, "Karte 5");
    Karte karte6(window.parameterauswahl, 6, "Karte 6");
    Karte karte7(window.parameterauswahl, 7, "Karte 7");


    window.show();
    return app.exec();
}
