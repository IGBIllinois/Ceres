/**
 */

#include "RgbCameraFactory.hpp"
#include "AxisCommunications/AxisCommunicationsIDs.hpp"
#include "AxisCommunications/AxisCommunicationsFactory.hpp"

sSensorWidgets rgb::create_sensor(const std::string& sensor_id,
    const nlohmann::json& sensorInfo, bool no_visualization)
{
    if (sensor_id.compare(axis_communications_id) == 0)
    {
        return axis_communications::create_sensor(sensorInfo, no_visualization);
    }

    return sSensorWidgets();
}

bool rgb::remove_sensor(const std::string& sensor_id, sSensorWidgets widgets)
{
    if (sensor_id.compare(axis_communications_id) == 0)
    {
        axis_communications::remove_sensor(widgets);
        return true;
    }

    return false;
}


cSensorPropertyPage* rgb::create_sensor_property_page(const std::string& sensor_id, 
    const std::string& model, uint32_t version,
    const std::string& remote_ip_address, uint16_t port, const std::string& local_ip_address)
{
    if (sensor_id.compare(axis_communications_id) == 0)
    {
        return axis_communications::create_sensor_property_page(model, version,
            remote_ip_address, port, local_ip_address);
    }


    return nullptr;
}
