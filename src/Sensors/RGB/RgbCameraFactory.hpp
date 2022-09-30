
#pragma once

#include "../SensorFactory.hpp"


namespace rgb
{
	sSensorWidgets create_sensor(const std::string& name, 
		const nlohmann::json& sensorInfo, bool no_visualization = false);

	bool remove_sensor(const std::string& name, sSensorWidgets widgets);

	cSensorPropertyPage* create_sensor_property_page(const std::string& name, uint32_t version,
		const std::string& remote_ip_address, uint16_t port, const std::string& local_ip_address);
}
