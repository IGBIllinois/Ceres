
#include "ExperimentCtrlFactory.hpp"

#include "ExperimentControllers/SpidercamCtrlFactory.hpp"


std::pair<cExperimentControlModel*, cExperimentControlView*> create_experiment_controller(const std::string& name)
{
    if (name.compare(spidercam_id) == 0)
    {
        return create_spidercam_controller();
    }

    return std::make_pair(nullptr, nullptr);
}