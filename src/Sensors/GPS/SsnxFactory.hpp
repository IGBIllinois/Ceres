
#pragma once

#include "../../SensorFactory.hpp"


static char* ssnx_id = "ssnx";

sSensorWidgets create_ssnx_sensor(bool no_visualization);

void remove_ssnx_sensor(sSensorWidgets widgets);
