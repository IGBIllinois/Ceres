
#pragma once

#include "../../SensorFactory.hpp"


static char* axis_communications_id = "axis_communications";

sSensorWidgets create_axis_communications_sensor(const nlohmann::json& sensorInfo);
void remove_axis_communications_sensor(sSensorWidgets widgets);

