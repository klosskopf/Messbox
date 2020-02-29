#ifndef CONTROL_H
#define CONTROL_H

#include <QObject>
#include <list>
#include <string>
#include "mainwindow.h"
#include "karte.h"
class Karte;
class mainWindow;

enum Modus {CONT,STARTSTOP};
enum Zustand {MESS,STOP};

class Control
{
public:
    static void controlThread(mainWindow* n_gui);
    static std::list<Karte*> Kartenset;
    static Modus modus;
    static Zustand zustand;
    static Rechenblock* xAchse;
    static Rechenblock* yAchse;
    static std::list<Daten*> kennlinie;
};

#endif // CONTROL_H
