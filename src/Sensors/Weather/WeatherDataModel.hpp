
#pragma once

#include "../SensorModel.hpp"



class cWeatherDataModel : public cSensorModel
{
public:

    /*
     * Returns a device identifier used by the sensor.  The ids are
     * only unique within a device type: Weather
     */
    uint8_t device_id() const override;

    /*
     * Returns a string used as a class descriptor of the
     * type sensor.
     */
    const char* sensorClass() const override { return "weather_data"; };

    /*
     * Returns a string used as a descriptor of the sensor.
     */
    const char* descriptor() const override;

protected:
    cWeatherDataModel(QObject* parent = nullptr);
    virtual ~cWeatherDataModel() = default;

private:
    const uint8_t mDeviceID;
};

