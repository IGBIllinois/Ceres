
#pragma once

#include "SensorModel.hpp"


class cLidarModel : public cSensorModel
{
public:

protected:
    cLidarModel(QObject* parent = nullptr);
    virtual ~cLidarModel() = default;
};

