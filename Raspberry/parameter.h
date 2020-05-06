#ifndef PARAMETER_H
#define PARAMETER_H
#include <string>
#include <vector>
#include "rechenblock.h"
#include "karte.h"
#include "daten.h"
class Karte;

enum Parametrierbar { NEIN, FREI, LISTE };

class Parameter : public Rechenblock
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
    double get_data(uint32_t time) override;
    bool is_plotbar() const;
    void add_datum(uint32_t time, float datum);
    void add_auswahl(std::string auswahl);
    uint32_t newest() override;
};

#endif // PARAMETER_H
