/**
 */

#include "HyperspectralFactory.hpp"
#include "./HySpex/HySpexFactory.hpp"


sSensorWidgets hyperspectral::create_sensor(const std::string& sensor_id,
    const nlohmann::json& sensorInfo, bool no_visualization)
{
    if (sensor_id.compare(hyspex_id) == 0)
    {
        return hyspex::create_sensor(sensorInfo, no_visualization);
    }

    return sSensorWidgets();
}

bool hyperspectral::remove_sensor(const std::string& sensor_id, sSensorWidgets widgets)
{
    if (sensor_id.compare(hyspex_id) == 0)
    {
        hyspex::remove_sensor(widgets);
        return true;
    }

    return false;
}

cSensorPropertyPage* hyperspectral::create_sensor_property_page(const std::string& sensor_id,
    const std::string& model, uint32_t version,
    const std::string& remote_ip_address, uint16_t port, const std::string& local_ip_address)
{
    if (sensor_id.compare(hyspex_id) == 0)
    {
 //       return hyspex::create_sensor_property_page(version, remote_ip_address, port, local_ip_address);
    }


    return nullptr;
}
