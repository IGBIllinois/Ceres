
#pragma once

#include "../SensorModel.hpp"


class cTofCameralModel : public cSensorModel
{
public:

    /*
     * Returns a device identifier used by the sensor.  The ids are
     * only unique within a device type: Time-Of-Flight Camera
     */
    uint8_t device_id() const override;

    /*
     * Returns a string used as a class descriptor of the
     * type sensor.
     */
    const char* sensorClass() const override { return "tof"; };

protected:
    cTofCameralModel(const std::string& name, QObject* parent = nullptr);
    virtual ~cTofCameralModel() = default;

private:
    const uint8_t mDeviceID;
};

