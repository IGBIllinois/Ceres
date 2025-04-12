
#pragma once

#include "../SensorModel.hpp"


class cMultispectralModel : public cSensorModel
{
public:

    /*
     * Returns a device identifier used by the sensor.  The ids are
     * only unique within a device type: Multispectral Camera
     */
    uint8_t device_id() const override;

    /*
     * Returns a string used as a class descriptor of the
     * type sensor.
     */
    const char* sensorClass() const override { return "multispectral"; };

protected:
    cMultispectralModel(const std::string& name, QObject* parent = nullptr);
    virtual ~cMultispectralModel() = default;

private:
    const uint8_t mDeviceID;
};

