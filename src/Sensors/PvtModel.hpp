
#pragma once


#include "SensorModel.hpp"

class cPvtModel : public cSensorModel
{
    Q_OBJECT

public:

protected:
    cPvtModel(QObject* parent = nullptr);
    ~cPvtModel() = default;

    bool mDataValid;

    double mPosition_m;
    double mSpeed_mps;
    double mTimestamp_s;
};

