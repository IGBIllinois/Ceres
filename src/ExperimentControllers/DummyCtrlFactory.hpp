
#pragma once

#include "ExperimentCtrlFactory.hpp"


namespace dummy
{
	sExperimentControllerWidgets create_controller(const nlohmann::json& ctrlInfo, bool no_visualization = false);
}