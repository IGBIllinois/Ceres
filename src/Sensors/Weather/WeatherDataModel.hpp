
#pragma once

#include "../SensorModel.hpp"



class cWeatherDataModel : public cSensorModel
{
public:
    /*
     * Returns a string used as a class descriptor of the
     * type sensor.
     */
    char* sensorClass() const override { return "weather_data"; };

    /*
     * Returns a string used as a descriptor of the sensor.
     */
    char* descriptor() const override;

protected:
    cWeatherDataModel(QObject* parent = nullptr);
    virtual ~cWeatherDataModel() = default;
};

