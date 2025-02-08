
#pragma once

#include "../../SensorFactory.hpp"


namespace hokuyo
{
	sSensorWidgets create_sensor(const nlohmann::json& sensorInfo, bool no_visualization);

	void remove_sensor(sSensorWidgets widgets);

	cSensorPropertyPage* create_sensor_property_page(uint32_t version,
		const std::string& remote_ip_address, uint16_t port, const std::string& local_ip_address);
}
