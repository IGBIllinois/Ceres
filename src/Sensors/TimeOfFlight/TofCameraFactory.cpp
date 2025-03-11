/**
 */

#include "TofCameraFactory.hpp"
#include "LucidVisionLabs/LucidVisionLabsIDs.hpp"
#include "LucidVisionLabs/LucidVisionLabsFactory.hpp"


sSensorWidgets tof::create_sensor(const std::string& sensor_id,
    const nlohmann::json& sensorInfo, bool no_visualization)
{
    if (sensor_id.compare(lucid_vision_labs_id) == 0)
    {
        return lucid_vision_labs::create_sensor(sensorInfo, no_visualization);
    }

    return sSensorWidgets();
}

bool tof::remove_sensor(const std::string& sensor_id, sSensorWidgets widgets)
{
    if (sensor_id.compare(lucid_vision_labs_id) == 0)
    {
        lucid_vision_labs::remove_sensor(widgets);
        return true;
    }

    return false;
}


cSensorPropertyPage* tof::create_sensor_property_page(const std::string& sensor_id, 
    const std::string& model, uint32_t version,
    const std::string& remote_ip_address, uint16_t port, const std::string& local_ip_address)
{
    if (sensor_id.compare(lucid_vision_labs_id) == 0)
    {
        return lucid_vision_labs::create_sensor_property_page(model, version,
            remote_ip_address, port, local_ip_address);
    }


    return nullptr;
}
