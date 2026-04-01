
#include "ExperimentCtrlFactory.hpp"

#include "DummyCtrlIDs.hpp"
#include "DummyCtrlFactory.hpp"
#include "Spidercam/SpidercamCtrlIDs.hpp"
#include "Spidercam/SpidercamCtrlFactory.hpp"
#include "Cart/CartCtrlIDs.hpp"
#include "Cart/CartCtrlFactory.hpp"

#include <QObject>


sExperimentControllerWidgets create_experiment_controller(const std::string& name, const nlohmann::json& ctrlInfo, bool no_visualization)
{
    if (name.compare(dummy_id) == 0)
    {
        return dummy::create_controller(ctrlInfo, no_visualization);
    }

    if (name.compare(spidercam_id) == 0)
    {
        return spidercam::create_controller(ctrlInfo, no_visualization);
    }

    if (name.compare(cart_id) == 0)
    {
        return cart::create_controller(ctrlInfo, no_visualization);
    }

    return sExperimentControllerWidgets();
}