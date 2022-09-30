/**
 */

#include "LidarFactory.hpp"
#include "OusterFactory.hpp"

sSensorWidgets lidar::create_sensor(const std::string& name, 
    const nlohmann::json& sensorInfo, bool no_visualization)
{
    if (name.compare(ouster_id) == 0)
    {
        return ouster::create_sensor(sensorInfo, no_visualization);
    }

    return sSensorWidgets();
}

bool lidar::remove_sensor(const std::string& name, sSensorWidgets widgets)
{
    if (name.compare(ouster_id) == 0)
    {
        ouster::remove_sensor(widgets);
        return true;
    }

    return false;
}

cSensorPropertyPage* lidar::create_sensor_property_page(const std::string& name, uint32_t version,
    const std::string& remote_ip_address, uint16_t port, const std::string& local_ip_address)
{
    if (name.compare(ouster_id) == 0)
    {
        return ouster::create_sensor_property_page(version, remote_ip_address, port, local_ip_address);
    }


    return nullptr;
}
