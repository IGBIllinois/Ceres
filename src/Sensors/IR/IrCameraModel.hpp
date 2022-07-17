
#pragma once

#include "../SensorModel.hpp"


class cIrCameralModel : public cSensorModel
{
public:

    /*
     * Returns a string used as a class descriptor of the
     * type sensor.
     */
    char* sensorClass() const override { return "ir"; };

protected:
    cIrCameralModel(const std::string& name, QObject* parent = nullptr);
    virtual ~cIrCameralModel() = default;
};

