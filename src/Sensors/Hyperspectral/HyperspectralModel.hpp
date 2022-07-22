
#pragma once

#include "../SensorModel.hpp"


class cHyperspectralModel : public cSensorModel
{
public:

    /*
     * Returns a string used as a class descriptor of the
     * type sensor.
     */
    const char* sensorClass() const override { return "hyperspectral"; };

protected:
    cHyperspectralModel(const std::string& name, QObject* parent = nullptr);
    virtual ~cHyperspectralModel() = default;
};

