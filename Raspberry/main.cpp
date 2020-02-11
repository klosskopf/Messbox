#include <QApplication>
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "mainwindow.h"



int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    mainWindow window;
    window.show();
    return app.exec();
}
