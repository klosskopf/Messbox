#pragma once

#include <QObject>
#include <list>
#include <string>
#include <mutex>
#include "rechenblock.h"
#include "mainwindow.h"
#include "karte.h"
#include "gpio.h"
#include "post.h"
#include "graphersteller.h"
#include "daten.h"
#include "rechenbloecke.h"
class Karte;
class mainWindow;
class Rechenblock;

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
    static float timeframe;
    static float samplefreq;
    static Rechenblock* xAchse;
    static Rechenblock* yAchse;
    static std::list<Kennliniendaten*> kennlinie;
    static bool newkarte;

    static float vcc5V;
    static float vcc33V;
    static float icharge;
    static float vbat;
    static float vlade;
    static float vin;

    static QMutex datenmutex;

    static void check_karten();
    static Karte* findkarte(int Karte);
};
