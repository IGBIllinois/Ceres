
#pragma once

#include "../../SensorFactory.hpp"


static char* ouster_id = "ouster";

sSensorWidgets create_ouster_sensor();
void remove_ouster_sensor(sSensorWidgets widgets);

