
#pragma once

#include "../SensorModel.hpp"


class cHyperspectralModel : public cSensorModel
{
public:

    /*
     * Returns a string used as a class descriptor of the
     * type sensor.
     */
    char* sensorClass() const override { return "hyperspectral"; };

protected:
    cHyperspectralModel();
    virtual ~cHyperspectralModel() = default;
};

