
#pragma once

#include "../SensorFactory.hpp"


static char* weather_data_id = "weather_data";

sSensorWidgets create_weather_data_sensor(const nlohmann::json& sensorInfo,
	bool no_visualization);

void remove_weather_data_sensor(sSensorWidgets widgets);

