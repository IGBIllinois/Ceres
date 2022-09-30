/**
 */

#include "GpsFactory.hpp"
#include "SsnxFactory.hpp"

sSensorWidgets gps::create_sensor(const std::string& name, 
    const nlohmann::json& sensorInfo, bool no_visualization)
{
    if (name.compare(ssnx_id) == 0)
    {
        return ssnx::create_sensor(sensorInfo, no_visualization);
    }

    return sSensorWidgets();
}

bool gps::remove_sensor(const std::string& name, sSensorWidgets widgets)
{
    if (name.compare(ssnx_id) == 0)
    {
        ssnx::remove_sensor(widgets);
        return true;
    }

    return false;
}

cSensorPropertyPage* gps::create_sensor_property_page(const std::string& name, uint32_t version,
    const std::string& remote_ip_address, uint16_t port, const std::string& local_ip_address)
{
    return nullptr;
}
