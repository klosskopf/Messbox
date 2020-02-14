#ifndef PARAMETER_GUI_H
#define PARAMETER_GUI_H
#include <QString>
#include "parameter.h"

class Parameter_GUI : public QString
{
public:
    Parameter_GUI();
    Parameter* parameter;
};

#endif // PARAMETER_GUI_H
