
#pragma once

#include "../SensorModel.hpp"


class cRgbCameraModel : public cSensorModel
{
public:

    /*
     * Returns a string used as a class descriptor of the
     * type sensor.
     */
    char* sensorClass() const override { return "rgb"; };

protected:
    cRgbCameraModel(const std::string& name, QObject* parent = nullptr);
    virtual ~cRgbCameraModel() = default;
};

