
#include "ExperimentCtrlFactory.hpp"

#include "ExperimentControllers/DummyCtrlFactory.hpp"
#include "ExperimentControllers/Spidercam/SpidercamCtrlFactory.hpp"


sExperimentControllerWidgets create_experiment_controller(const std::string& name, bool no_visualization)
{
    if (name.compare(dummy_id) == 0)
    {
        return create_dummy_controller(no_visualization);
    }

    if (name.compare(spidercam_id) == 0)
    {
        return create_spidercam_controller(no_visualization);
    }

    return sExperimentControllerWidgets();
}