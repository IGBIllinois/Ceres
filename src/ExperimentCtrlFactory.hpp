
#pragma once

#include <string>
#include <utility>
#include <memory>

class cExperimentControlView;
class cExperimentControlModel;

std::pair<cExperimentControlModel*, cExperimentControlView*> create_experiment_controller(const std::string& name);