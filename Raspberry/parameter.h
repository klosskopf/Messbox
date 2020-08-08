#pragma once
#include <string>
#include <vector>
#include <QMutex>
#include "rechenblock.h"
#include "karte.h"
#include "daten.h"
class Karte;

enum Parametrierbar { NEIN, FREI, LISTE };

class Parameter
{
public:
    Parameter(uint32_t n_nummer, bool n_f_nots, std::string n_name, Parametrierbar n_parametrierar, float n_min, float n_max);
    Karte* karte;
    const uint32_t nummer;
    const bool f_nots;
    std::string name;
    const Parametrierbar parametrierbar;
    std::list<std::string>* auswahlliste;
    float min;
    float max;
    std::vector<Daten*>* daten;
    QMutex daten_mutex;
    bool is_plotbar() const;
    void add_datum(uint32_t time, float datum);
    void delete_daten();
    void add_auswahl(std::string auswahl);
};

class ParameterViewer : public Rechenblock
{
public:
    ParameterViewer(Parameter* n_parameter):Rechenblock(0,5), parameter(n_parameter){index=0;}
    Parameter* parameter;
    double get_data(uint32_t time) override;
    uint32_t index;
    uint32_t newest() override;
    QString print() override;
};
