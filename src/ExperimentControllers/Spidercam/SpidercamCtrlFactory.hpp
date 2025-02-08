
#pragma once

#include "../ExperimentCtrlFactory.hpp"

namespace spidercam
{
	sExperimentControllerWidgets create_controller(const nlohmann::json& ctrlInfo, bool no_visualization = false);
}