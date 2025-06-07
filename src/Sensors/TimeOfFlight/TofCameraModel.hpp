
#pragma once

#include "../SensorModel.hpp"


class cTofCameraModel : public cSensorModel
{
public:

    /*
     * Returns a device identifier used by the sensor.  The ids are
     * only unique within a device type: Time-Of-Flight Camera
     */
//    uint8_t device_id() const override;

    /*
     * Returns a string used as a class descriptor of the
     * type sensor.
     */
    const char* sensorClass() const override { return "time-of-flight"; };

protected:
    cTofCameraModel(const std::string& name, QObject* parent = nullptr);
    virtual ~cTofCameraModel() = default;
};

