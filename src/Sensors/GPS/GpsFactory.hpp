
#pragma once

#include "../SensorFactory.hpp"


namespace gps
{
	sSensorWidgets create_sensor(const std::string& sensor_id, 
		const nlohmann::json& sensorInfo, bool no_visualization = false);

	bool remove_sensor(const std::string& sensor_id, sSensorWidgets widgets);

	cSensorPropertyPage* create_sensor_property_page(const std::string& sensor_id,
		const std::string& model, uint32_t version,
		const std::string& remote_ip_address, uint16_t port, const std::string& local_ip_address);

	cSensorPropertyPage* create_sensor_property_page(const std::string& sensor_id, const std::string& instance,
		const std::string& model, uint32_t version,
		const std::string& remote_ip_address, uint16_t port, const std::string& local_ip_address);
}
