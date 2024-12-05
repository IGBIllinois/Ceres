
#include "GpsUtils.hpp"


int gps::to_int(gps::eDatum datum) { return static_cast<int>(datum); }

gps::eDatum gps::to_datum(int datum)
{
    switch (datum)
    {
    case 1: return gps::eDatum::DGNSS;
    case 2: return gps::eDatum::ETRS89;
    case 3: return gps::eDatum::NAD83;
    case 4: return gps::eDatum::NAD83_PA11;
    case 5: return gps::eDatum::NAD83_MA11;
    case 6: return gps::eDatum::GDA94;
    case 7: return gps::eDatum::FIRST_USER_DATUM;
    case 8: return gps::eDatum::SECOND_USER_DATUM;
    }

    return gps::eDatum::WGS84;
}

QString gps::to_qstring(gps::eDatum datum)
{
    switch (datum)
    {
    case gps::eDatum::WGS84: return QString("WGS84");
    case gps::eDatum::DGNSS: return QString("DGNSS");
    case gps::eDatum::ETRS89: return QString("ETRS89");
    case gps::eDatum::NAD83: return QString("NAD83");
    case gps::eDatum::NAD83_PA11: return QString("NAD83 (PA11)");
    case gps::eDatum::NAD83_MA11: return QString("NAD83 (MA11)");
    case gps::eDatum::GDA94: return QString("GDA94");
    case gps::eDatum::FIRST_USER_DATUM: return QString("First User Datum");
    case gps::eDatum::SECOND_USER_DATUM: return QString("Second User Datum");
    }

    return QString("Unknown");
}



int gps::to_int(eSolutionType solution_type) { return static_cast<int>(solution_type); }

gps::eSolutionType gps::to_solution_type(int solution_type)
{
    switch (solution_type)
    {
    case 1: return gps::eSolutionType::STAND_ALONE;
    case 2: return gps::eSolutionType::DIFFERENTIAL;
    case 3: return gps::eSolutionType::FIXED;
    case 4: return gps::eSolutionType::RTK_FIXED;
    case 5: return gps::eSolutionType::RTK_FLOAT;
    case 6: return gps::eSolutionType::SBAS;
    case 7: return gps::eSolutionType::MOVING_RTK_FIXED;
    case 8: return gps::eSolutionType::MOVING_RTK_FLOAT;
    case 9: return gps::eSolutionType::PPP;
    }

    return gps::eSolutionType::NONE;
}

QString gps::to_qstring(gps::eSolutionType mode)
{
    switch (mode)
    {
    case gps::eSolutionType::STAND_ALONE: return QString("Stand Alone");
    case gps::eSolutionType::DIFFERENTIAL: return QString("Differential");
    case gps::eSolutionType::FIXED: return QString("Fixed");
    case gps::eSolutionType::RTK_FIXED: return QString("RTK Fixed");
    case gps::eSolutionType::RTK_FLOAT: return QString("RTK Float");
    case gps::eSolutionType::SBAS: return QString("SBAS");
    case gps::eSolutionType::MOVING_RTK_FIXED: return QString("Moving RTK Fixed");
    case gps::eSolutionType::MOVING_RTK_FLOAT: return QString("Moving RTK Float");
    case gps::eSolutionType::PPP: return QString("PPP");
    }

    return QString("NONE");
}
