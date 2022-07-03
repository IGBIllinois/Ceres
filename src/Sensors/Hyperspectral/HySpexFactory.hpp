
#pragma once

#include "../../common/SensorFactory.hpp"


static char* hyspex_id = "hyspex";

sSensorWidgets create_hyspex_sensor(const nlohmann::json& sensorInfo,
	bool no_visualization);

void remove_hyspex_sensor(sSensorWidgets widgets);

