
#pragma once

#include "../SensorModel.hpp"


class cMultispectralModel : public cSensorModel
{
public:

    /*
     * Returns a string used as a class descriptor of the
     * type sensor.
     */
    const char* sensorClass() const override { return "multispectral"; };

protected:
    cMultispectralModel(const std::string& name, QObject* parent = nullptr);
    virtual ~cMultispectralModel() = default;
};

