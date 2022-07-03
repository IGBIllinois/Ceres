
#pragma once

#include "../common/SensorFactory.hpp"


static char* dummy_id = "dummy";

sSensorWidgets create_dummy_sensor(bool no_visualization);

void remove_dummy_sensor(sSensorWidgets widgets);

