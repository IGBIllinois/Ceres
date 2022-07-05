
#include "SensorFactory.hpp"

#include "DummySensorFactory.hpp"
#include "Lidar/OusterFactory.hpp"
#include "GPS/SsnxFactory.hpp"
#include "Weather/WeatherDataFactory.hpp"
#include "Hyperspectral/HySpexFactory.hpp"
#include "RGB/AxisCommunicationsFactory.hpp"


sSensorWidgets create_sensor(const std::string& name, 
    const nlohmann::json& sensorInfo, bool no_visualization)
{
    if (name.compare(dummy_id) == 0)
    {
        return create_dummy_sensor(no_visualization);
    }

    if (name.compare(ssnx_id) == 0)
    {
        return create_ssnx_sensor(sensorInfo, no_visualization);
    }

    if (name.compare(ouster_id) == 0)
    {
        return create_ouster_sensor(sensorInfo, no_visualization);
    }

    if (name.compare(weather_data_id) == 0)
    {
        return create_weather_data_sensor(sensorInfo, no_visualization);
    }

    if (name.compare(hyspex_id) == 0)
    {
        return create_hyspex_sensor(sensorInfo, no_visualization);
    }

    if (name.compare(axis_communications_id) == 0)
    {
        return create_axis_communications_sensor(sensorInfo, no_visualization);
    }

    return sSensorWidgets();
}

void remove_sensor(const std::string& name, sSensorWidgets widgets)
{
    if (name.compare(dummy_id) == 0)
    {
        remove_dummy_sensor(widgets);
        return;
    }

    if (name.compare(ssnx_id) == 0)
    {
        remove_ssnx_sensor(widgets);
        return;
    }

    if (name.compare(ouster_id) == 0)
    {
        remove_ouster_sensor(widgets);
        return;
    }

    if (name.compare(weather_data_id) == 0)
    {
        remove_weather_data_sensor(widgets);
        return;
    }

    if (name.compare(hyspex_id) == 0)
    {
        remove_hyspex_sensor(widgets);
        return;
    }

    if (name.compare(axis_communications_id) == 0)
    {
        remove_axis_communications_sensor(widgets);
        return;
    }
}
