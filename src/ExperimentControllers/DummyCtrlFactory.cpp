
#include "DummyCtrlFactory.hpp"

#include "DummyCtrlView.hpp"
#include "DummyCtrlModel.hpp"

#include <QWidget>
#include <QString>
#include <QDockWidget>

sExperimentControllerWidgets create_dummy_controller()
{
    // Create the Dummy Controller...
    auto* pModel = new cDummyControlModel();
    auto* pView = new cDummyControlView();

    return sExperimentControllerWidgets(pModel, pView);
}