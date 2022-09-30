
#pragma once

#include "../SensorFactory.hpp"


static char* ssnx_id = "ssnx";

namespace ssnx
{
	sSensorWidgets create_sensor(const nlohmann::json& sensorInfo, bool no_visualization);

	void remove_sensor(sSensorWidgets widgets);
}