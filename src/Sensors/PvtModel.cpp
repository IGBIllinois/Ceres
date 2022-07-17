
#include "PvtModel.hpp"


cPvtModel::cPvtModel(const std::string& name, QObject* parent)
    : cSensorModel(name, parent)
{
    mDataValid = false;

    mPosition_m = 0.0;
    mSpeed_mps = 0.0;
    mTimestamp_s = 0.0;
}

