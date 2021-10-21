
#pragma once

#include <string>
#include <utility>
#include <memory>

class cExperimentControlView;
class cExperimentControlModel;


static char* spidercam_id = "spidercam";

std::pair<cExperimentControlModel*, cExperimentControlView*> create_spidercam_controller();