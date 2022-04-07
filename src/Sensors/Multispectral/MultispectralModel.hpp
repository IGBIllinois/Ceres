
#pragma once

#include "../SensorModel.hpp"


class cMultispectralModel : public cSensorModel
{
public:

    /*
     * Returns a string used as a class descriptor of the
     * type sensor.
     */
    char* sensorClass() const override { return "multispectral"; };

protected:
    cMultispectralModel(QObject* parent = nullptr);
    virtual ~cMultispectralModel() = default;
};

