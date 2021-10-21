
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


    QObject::connect(model, &cSpidercamModel::updatePosition, view, &cSpidercamView::onUpdatedPosition);

    /*
        auto* lidarModel = new cLidarModelOuster(parent);
        auto* dockWidget = new QDockWidget(lidarModel->getViewTitle(), parent);

        dockWidget->setWidget(lidarView);

        lidarModel->connect(lidarModel, &cLidarModelOuster::updateView, lidarView, &cOusterView::displayData);

        return std::make_pair(lidarModel, dockWidget);
    */
    return std::make_pair(model, view);
}