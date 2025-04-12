
#pragma once

#include "../SensorModel.hpp"


class cIrCameralModel : public cSensorModel
{
public:

    /*
     * Returns a device identifier used by the sensor.  The ids are
     * only unique within a device type: IR Camera
     */
    uint8_t device_id() const override;

    /*
     * Returns a string used as a class descriptor of the
     * type sensor.
     */
    const char* sensorClass() const override { return "ir"; };

protected:
    cIrCameralModel(const std::string& name, QObject* parent = nullptr);
    virtual ~cIrCameralModel() = default;

private:
    const uint8_t mDeviceID;
};

