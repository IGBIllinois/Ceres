
#pragma once

#include "../SensorModel.hpp"


class cTofCameralModel : public cSensorModel
{
public:

    /*
     * Returns a string used as a class descriptor of the
     * type sensor.
     */
    const char* sensorClass() const override { return "tof"; };

protected:
    cTofCameralModel(const std::string& name, QObject* parent = nullptr);
    virtual ~cTofCameralModel() = default;
};

