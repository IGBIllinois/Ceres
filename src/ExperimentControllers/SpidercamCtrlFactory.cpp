
#include "SpidercamCtrlFactory.hpp"

#include "SpidercamView.hpp"
#include "SpidercamModel.hpp"

#include <QWidget>
#include <QString>


std::pair<cExperimentControlModel*, cExperimentControlView*> create_spidercam_controller()
{
    // Create the Spidercam Controller...
    auto* model = new cSpidercamModel();
    auto* view = new cSpidercamView();

    QObject::connect(model, &cSpidercamModel::limitsChanged, view, &cSpidercamView::updateLimits);

    QObject::connect(model, &cSpidercamModel::positionChanged, view, &cSpidercamView::updatedPosition);

    return std::make_pair(model, view);
}