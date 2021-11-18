
#include "SensorFactory.hpp"

#include "Sensors/OusterFactory.hpp"
#include "Sensors/SsnxFactory.hpp"
#include "Sensors/Weather/WeatherDataFactory.hpp"


sSensorWidgets create_sensor(const std::string& name, const nlohmann::json& sensorInfo)
{
    if (name.compare(ssnx_id) == 0)
    {
        return create_ssnx_sensor();
    }

    if (name.compare(ouster_id) == 0)
    {
        return create_ouster_sensor();
    }

    if (name.compare(weather_data_id) == 0)
    {
        return create_weather_data_sensor(sensorInfo);
    }

    return sSensorWidgets();
}

void remove_sensor(const std::string& name, sSensorWidgets widgets)
{
    if (name.compare(ssnx_id) == 0)
    {
        remove_ssnx_sensor(widgets);
    }

    if (name.compare(ouster_id) == 0)
    {
        remove_ouster_sensor(widgets);
    }

    if (name.compare(weather_data_id) == 0)
    {
        remove_weather_data_sensor(widgets);
    }
}
