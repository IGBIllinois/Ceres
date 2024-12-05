
#pragma once

#include "GpsTypes.hpp"

#include <QString>

namespace gps
{
    int to_int(eDatum datum);
    eDatum to_datum(int datum);

    QString to_qstring(gps::eDatum datum);

    int to_int(eSolutionType solution_type);
    eSolutionType to_solution_type(int solution_type);

    QString to_qstring(eSolutionType solution_type);

};

