#ifndef PARAMETER_H
#define PARAMETER_H
#include "rechenblock.h"
#include "daten.h"
#include <string>
#include <list>

enum Parametrierbar { NEIN, FREI, LISTE };

class Parameter : public Rechenblock
{
public:
    Parameter(uint32_t n_nummer, bool n_f_nots, std::string n_name, Parametrierbar n_parametrierar, float n_min, float n_max);
    virtual ~Parameter();
    const uint32_t nummer;
    const bool f_nots;
    const std::string name;
    const Parametrierbar parametrierbar;
    std::list<std::string> auswahlliste;
    float min;
    float max;
    std::list<Daten*> daten;
    float get_data(uint32_t time) override;
    void delete_old();
    bool is_plotbar() const;
    void add_datum(uint32_t time, float datum);
    void add_auswahl(std::string auswahl);
    uint32_t newest() override;
};

#endif // PARAMETER_H
