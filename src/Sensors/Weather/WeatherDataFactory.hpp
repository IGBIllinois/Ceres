
#pragma once

#include "../SensorFactory.hpp"


namespace weather_data
{
	sSensorWidgets create_sensor(const nlohmann::json& sensorInfo,
		bool no_visualization);

	void remove_sensor(sSensorWidgets widgets);
}
