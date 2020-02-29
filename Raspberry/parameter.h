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
    Parameter(bool n_f_nots, std::string n_name, Parametrierbar n_parametrierar);
    virtual ~Parameter();
    const bool f_nots;
    const std::string name;
    const Parametrierbar parametrierbar;
    std::list<std::string> auswahlliste;
    float min;
    float max;
    std::list<Daten*> daten;
    float get_data(float time) override;
    void delete_old();
    bool is_plotbar() const;
    void add_datum(float time, float datum);
    void add_auswahl(std::string auswahl);
};

#endif // PARAMETER_H
