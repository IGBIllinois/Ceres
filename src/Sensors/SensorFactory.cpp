
#include "SensorFactory.hpp"

#include "DummySensorIDs.hpp"
#include "DummySensorFactory.hpp"
#include "Lidar/LidarFactory.hpp"
#include "GPS/GpsFactory.hpp"
#include "Weather/WeatherDataIDs.hpp"
#include "Weather/WeatherDataFactory.hpp"
#include "IR/IrCameraFactory.hpp"
#include "Hyperspectral/HyperspectralFactory.hpp"
#include "RGB/RgbCameraFactory.hpp"
#include "TimeOfFlight/TofCameraFactory.hpp"


sSensorWidgets create_sensor(const std::string& sensor_id, 
    const nlohmann::json& sensorInfo, bool no_visualization)
{
    if (sensor_id.compare(dummy_class_id) == 0)
    {
        return dummy::create_sensor(no_visualization);
    }

    if (sensor_id.compare(weather_data_id) == 0)
    {
        return weather_data::create_sensor(sensorInfo, no_visualization);
    }

    sSensorWidgets result;

    result = gps::create_sensor(sensor_id, sensorInfo, no_visualization);

    if (result)
        return result;

    result = lidar::create_sensor(sensor_id, sensorInfo, no_visualization);

    if (result)
        return result;

    result = rgb::create_sensor(sensor_id, sensorInfo, no_visualization);

    if (result)
        return result;

    result = hyperspectral::create_sensor(sensor_id, sensorInfo, no_visualization);

    if (result)
        return result;

    result = ir::create_sensor(sensor_id, sensorInfo, no_visualization);

    if (result)
        return result;

    result = tof::create_sensor(sensor_id, sensorInfo, no_visualization);

    if (result)
        return result;

    return sSensorWidgets();
}

void remove_sensor(const std::string& sensor_id, sSensorWidgets widgets)
{
    if (sensor_id.compare(dummy_class_id) == 0)
    {
        dummy::remove_sensor(widgets);
        return;
    }

    if (sensor_id.compare(weather_data_id) == 0)
    {
        weather_data::remove_sensor(widgets);
        return;
    }

    if (hyperspectral::remove_sensor(sensor_id, widgets))
        return;

    if (gps::remove_sensor(sensor_id, widgets))
        return;

    if (lidar::remove_sensor(sensor_id, widgets))
        return;

    if (rgb::remove_sensor(sensor_id, widgets))
        return;

    if (ir::remove_sensor(sensor_id, widgets))
        return;

    if (tof::remove_sensor(sensor_id, widgets))
        return;
}


cSensorPropertyPage* create_sensor_property_page(
    const std::string& sensor_id, const std::string& model, uint32_t version,
    const std::string& remote_ip_address, uint16_t port, const std::string& local_ip_address)
{
    cSensorPropertyPage* result = nullptr;

    result = gps::create_sensor_property_page(sensor_id,model, version, remote_ip_address, port, local_ip_address);
    if (result) return result;

    result = lidar::create_sensor_property_page(sensor_id, model, version, remote_ip_address, port, local_ip_address);
    if (result) return result;

    result = rgb::create_sensor_property_page(sensor_id, model, version, remote_ip_address, port, local_ip_address);
    if (result) return result;

    result = hyperspectral::create_sensor_property_page(sensor_id, model, version, remote_ip_address, port, local_ip_address);
    if (result) return result;

    result = ir::create_sensor_property_page(sensor_id, model, version, remote_ip_address, port, local_ip_address);
    if (result) return result;

    result = tof::create_sensor_property_page(sensor_id, model, version, remote_ip_address, port, local_ip_address);
    if (result) return result;

    return nullptr;
}
