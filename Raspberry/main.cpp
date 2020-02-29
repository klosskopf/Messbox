#include <QApplication>
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "mainwindow.h"
#include "karte.h"
#include "parameter.h"
#include "control.h"
#include <thread>
#include <list>


int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    mainWindow window;

    std::thread controlthread(Control::controlThread,&window);

    window.show();
    return app.exec();
}
