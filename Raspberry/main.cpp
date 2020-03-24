#include <QApplication>
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "mainwindow.h"
#include "karte.h"
#include "parameter.h"
#include "control.h"
#include "decoder.h"
#include <thread>
#include <list>


int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    mainWindow window;

    std::thread postthread(Post::spi_thread);
    std::thread decoderthread(Decoder::decoder_thread);
    std::thread controlthread(Control::control_thread,&window);

    window.show();
    return app.exec();
}
