
#include "SpidercamCtrlFactory.hpp"

#include "SpidercamView.hpp"
#include "SpidercamModel.hpp"
#include "SpidercamDollyStatus.hpp"

#include <QWidget>
#include <QString>
#include <QDockWidget>



sExperimentControllerWidgets create_spidercam_controller()
{
    // Create the Spidercam Controller...
    auto* pModel = new cSpidercamModel();
    auto* pView = new cSpidercamView();

    QObject::connect(pModel, &cSpidercamModel::limitsChanged, pView, &cSpidercamView::updateLimits);
    QObject::connect(pModel, &cSpidercamModel::positionChanged, pView, &cSpidercamView::updatedPosition);
    QObject::connect(pModel, &cSpidercamModel::updateRecordingState, pView, &cSpidercamView::recordingStateUpdated);

    auto* pDockWidget = new QDockWidget();
    auto* pStatus = new cSpidercamDollyStatus(pDockWidget);

    pDockWidget->setWindowTitle(pStatus->windowTitle());
    pDockWidget->setWidget(pStatus);
    
    QObject::connect(pDockWidget, &QDockWidget::dockLocationChanged, pStatus, &cSpidercamDollyStatus::dockLocationChanged);
    QObject::connect(pDockWidget, &QDockWidget::topLevelChanged, pStatus, &cSpidercamDollyStatus::topLevelChanged);
    QObject::connect(pModel, &cSpidercamModel::limitsChanged, pStatus, &cSpidercamDollyStatus::updateLimits);
    QObject::connect(pModel, &cSpidercamModel::positionChanged, pStatus, &cSpidercamDollyStatus::updatePosition);
    QObject::connect(pModel, &cSpidercamModel::batteryLevelChanged, pStatus, &cSpidercamDollyStatus::updateBatteryLevel);

    return sExperimentControllerWidgets(pModel, pView, pDockWidget);
}