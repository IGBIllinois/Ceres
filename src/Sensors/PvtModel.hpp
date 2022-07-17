
#pragma once


#include "SensorModel.hpp"

class cPvtModel : public cSensorModel
{
    Q_OBJECT

public:

protected:
    cPvtModel(const std::string& name, QObject* parent = nullptr);
    ~cPvtModel() = default;

    bool mDataValid;

    double mPosition_m;
    double mSpeed_mps;
    double mTimestamp_s;
};

