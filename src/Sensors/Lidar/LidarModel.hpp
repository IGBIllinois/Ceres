
#pragma once

#include "../SensorModel.hpp"


class cLidarModel : public cSensorModel
{
public:

    /*
     * Returns a string used as a class descriptor of the
     * type sensor.
     */
    char* sensorClass() const override { return "lidar"; };

protected:
    cLidarModel(const std::string& name, QObject* parent = nullptr);
    virtual ~cLidarModel() = default;
};

