#ifndef CONTROL_H
#define CONTROL_H

#include <QObject>
#include <list>
#include <string>
#include "mainwindow.h"
#include "karte.h"
#include "gpio.h"
#include "post.h"
class Karte;
class mainWindow;

enum Modus {CONT,STARTSTOP};
enum Zustand {MESS,STOP};

class Control
{
public:
    static void control_thread(mainWindow* n_gui);
    static mainWindow* gui;
    static std::list<Karte*> Kartenset;
    static Modus modus;
    static Zustand zustand;
    static Rechenblock* xAchse;
    static Rechenblock* yAchse;
    static std::list<Daten*> kennlinie;
    static bool newkarte;

    static void check_karten();
    static void build_kennlinie();
};

#endif // CONTROL_H
