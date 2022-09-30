
#include "SensorFactory.hpp"

#include "DummySensorFactory.hpp"
#include "Lidar/LidarFactory.hpp"
#include "GPS/GpsFactory.hpp"
#include "Weather/WeatherDataFactory.hpp"
#include "Hyperspectral/HySpexFactory.hpp"
#include "RGB/RgbCameraFactory.hpp"


sSensorWidgets create_sensor(const std::string& name, 
    const nlohmann::json& sensorInfo, bool no_visualization)
{
    if (name.compare(dummy_id) == 0)
    {
        return create_dummy_sensor(no_visualization);
    }

    if (name.compare(weather_data_id) == 0)
    {
        return create_weather_data_sensor(sensorInfo, no_visualization);
    }

    if (name.compare(hyspex_id) == 0)
    {
        return create_hyspex_sensor(sensorInfo, no_visualization);
    }

    sSensorWidgets result;

    result = gps::create_sensor(name, sensorInfo, no_visualization);

    if (result)
        return result;

    result = lidar::create_sensor(name, sensorInfo, no_visualization);

    if (result)
        return result;

    result = rgb::create_sensor(name, sensorInfo, no_visualization);

    if (result)
        return result;

    return sSensorWidgets();
}

void remove_sensor(const std::string& name, sSensorWidgets widgets)
{
    if (name.compare(dummy_id) == 0)
    {
        remove_dummy_sensor(widgets);
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

    if (gps::remove_sensor(name, widgets))
        return;

    if (lidar::remove_sensor(name, widgets))
        return;

    if (rgb::remove_sensor(name, widgets))
        return;
}


cSensorPropertyPage* create_sensor_property_page(const std::string& name, uint32_t version,
    const std::string& remote_ip_address, uint16_t port, const std::string& local_ip_address)
{
    cSensorPropertyPage* result = nullptr;

    result = gps::create_sensor_property_page(name, version, remote_ip_address, port, local_ip_address);
    if (result) return result;

    result = lidar::create_sensor_property_page(name, version, remote_ip_address, port, local_ip_address);
    if (result) return result;

    result = rgb::create_sensor_property_page(name, version, remote_ip_address, port, local_ip_address);
    if (result) return result;

    return nullptr;
}
