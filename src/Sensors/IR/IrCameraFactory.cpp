/**
 */

#include "IrCameraFactory.hpp"
#include "./TeledyneFLIR/TeledyneFlirFactory.hpp"
#include "./TeledyneFLIR/TeledyneFlirIDs.hpp"


sSensorWidgets ir::create_sensor(const std::string& sensor_id,
    const nlohmann::json& sensorInfo, bool no_visualization)
{
    if (sensor_id.compare(teledyne_flir_id) == 0)
    {
        return teledyne_flir::create_sensor(sensorInfo, no_visualization);
    }

    return sSensorWidgets();
}

bool ir::remove_sensor(const std::string& sensor_id, sSensorWidgets widgets)
{
    if (sensor_id.compare(teledyne_flir_id) == 0)
    {
        teledyne_flir::remove_sensor(widgets);
        return true;
    }

    return false;
}


cSensorPropertyPage* ir::create_sensor_property_page(const std::string& sensor_id, 
    const std::string& model, uint32_t version,
    const std::string& remote_ip_address, uint16_t port, const std::string& local_ip_address)
{
    if (sensor_id.compare(teledyne_flir_id) == 0)
    {
        return teledyne_flir::create_sensor_property_page(model, version, remote_ip_address, port, local_ip_address);
    }

    return nullptr;
}
