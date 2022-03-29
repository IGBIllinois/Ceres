
#include "SpidercamCtrlFactory.hpp"

#include "SpidercamView.hpp"
#include "SpidercamModel.hpp"
#include "SpidercamDollyStatus.hpp"

#include <QWidget>
#include <QString>
#include <QDockWidget>

Q_DECLARE_METATYPE(spidercam::sWorkingDimensions);
Q_DECLARE_METATYPE(spidercam::sPosition);

sExperimentControllerWidgets create_spidercam_controller()
{
    // Register our custom spidercam types with Qt's meta type system.  Needed for using signals/slots
    qRegisterMetaType<spidercam::sWorkingDimensions>();
    qRegisterMetaType<spidercam::sPosition>();

    // Create the Spidercam Controller...
    auto* pModel = new cSpidercamModel();
    auto* pView = new cSpidercamView();

    QObject::connect(pModel, &cSpidercamModel::limitsChanged, pView, &cSpidercamView::updateLimits);
    QObject::connect(pModel, &cSpidercamModel::positionChanged, pView, &cSpidercamView::updatePosition);
    QObject::connect(pModel, &cSpidercamModel::requestDataRecordingState, pView, &cSpidercamView::updateRecordingState);

    auto* pDockWidget = new QDockWidget();
    auto* pStatus = new cSpidercamDollyStatus(pDockWidget);

    pDockWidget->setWindowTitle(pStatus->windowTitle());
    pDockWidget->setWidget(pStatus);
    
    QObject::connect(pDockWidget, &QDockWidget::dockLocationChanged, pStatus, &cSpidercamDollyStatus::dockLocationChanged);
    QObject::connect(pDockWidget, &QDockWidget::topLevelChanged, pStatus, &cSpidercamDollyStatus::topLevelChanged);
    QObject::connect(pModel, &cSpidercamModel::limitsChanged, pStatus, &cSpidercamDollyStatus::updateLimits);
    QObject::connect(pModel, &cSpidercamModel::positionChanged, pStatus, &cSpidercamDollyStatus::updatePosition);
    QObject::connect(pModel, &cSpidercamModel::batteryLevelChanged, pStatus, &cSpidercamDollyStatus::updateBatteryLevel);

    auto* pStatusBar = new cBatteryStatus();

    QObject::connect(pModel, &cSpidercamModel::batteryLevelChanged, pStatusBar, &cBatteryStatus::updateBatteryLevel);

    return sExperimentControllerWidgets(pModel, pView, pDockWidget, pStatusBar);
}