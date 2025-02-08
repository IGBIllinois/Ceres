
#pragma once

#include "SensorFactory.hpp"


namespace dummy
{
	sSensorWidgets create_sensor(bool no_visualization);

	void remove_sensor(sSensorWidgets widgets);
}

