/**
 */

#include "LidarFactory.hpp"
#include "Ouster/OusterIDs.hpp"
#include "Ouster/OusterFactory.hpp"
#include "Hokuyo/HokuyoFactory.hpp"


sSensorWidgets lidar::create_sensor(const std::string& sensor_id,
    const nlohmann::json& sensorInfo, bool no_visualization)
{
    if (sensor_id.compare(ouster_id) == 0)
    {
        return ouster::create_sensor(sensorInfo, no_visualization);
    }

    if (sensor_id.compare(hokuyo_id) == 0)
    {
        return hokuyo::create_sensor(sensorInfo, no_visualization);
    }

    return sSensorWidgets();
}

bool lidar::remove_sensor(const std::string& sensor_id, sSensorWidgets widgets)
{
    if (sensor_id.compare(ouster_id) == 0)
    {
        ouster::remove_sensor(widgets);
        return true;
    }

    if (sensor_id.compare(hokuyo_id) == 0)
    {
        hokuyo::remove_sensor(widgets);
        return true;
    }

    return false;
}

cSensorPropertyPage* lidar::create_sensor_property_page(const std::string& sensor_id, 
    const std::string& model, uint32_t version,
    const std::string& remote_ip_address, uint16_t port, const std::string& local_ip_address)
{
    if (sensor_id.compare(ouster_id) == 0)
    {
        return ouster::create_sensor_property_page(version, remote_ip_address, port, local_ip_address);
    }

    if (sensor_id.compare(hokuyo_id) == 0)
    {
        return hokuyo::create_sensor_property_page(version, remote_ip_address, port, local_ip_address);
    }

    return nullptr;
}
