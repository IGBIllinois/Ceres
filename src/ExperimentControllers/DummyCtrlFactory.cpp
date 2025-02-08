
#include "DummyCtrlFactory.hpp"

#include "DummyCtrlView.hpp"
#include "DummyCtrlModel.hpp"

sExperimentControllerWidgets dummy::create_controller(const nlohmann::json& ctrlInfo, bool no_visualization)
{
    // Create the Dummy Controller...
    auto* pModel = new cDummyControlModel();

    if (no_visualization)
        return sExperimentControllerWidgets(pModel, nullptr);

    auto* pView = new cDummyControlView();

    return sExperimentControllerWidgets(pModel, pView);
}