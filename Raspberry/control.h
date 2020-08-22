#pragma once

#include <QObject>
#include <list>
#include <string>
#include <QMutex>
#include <QTimer>
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
enum Zustand {MESS,STOP,SAVE};

class Control
{
public:
    static void control_thread(mainWindow* n_gui);
    static mainWindow* gui;
    static std::list<Karte*> Kartenset;
    static QMutex kartenset_mutex;
    static Modus modus;
    static Zustand zustand;
    static QTimer timeouttimer;
    static float timeframe;
    static float samplefreq;
    static Rechenblock* xAchse;
    static Rechenblock* yAchse;
    static bool newkarte;

    static float vcc5V;
    static float vcc33V;
    static float icharge;
    static float vbat;
    static float vlade;
    static float vin;
    static bool last_paket_to_save_received;

    static void start();
    static void stop();
    static void check_karten();
    static void saveprocedure();
    static Karte* findkarte(int Karte);
};
