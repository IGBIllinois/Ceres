
#pragma once

#include "../SensorFactory.hpp"


static char* ouster_id = "ouster";

namespace ouster
{
	sSensorWidgets create_sensor(const nlohmann::json& sensorInfo, bool no_visualization);

	void remove_sensor(sSensorWidgets widgets);

	cSensorPropertyPageRemote* create_sensor_property_page(uint32_t version);
}
