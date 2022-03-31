
#pragma once

#include "../../SensorFactory.hpp"


static char* hyspex_id = "hyspex";

sSensorWidgets create_hyspex_sensor(const nlohmann::json& sensorInfo);
void remove_hyspex_sensor(sSensorWidgets widgets);

