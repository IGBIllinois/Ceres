
#include "CartCtrlFactory.hpp"

//#include "SpidercamView.hpp"
//#include "SpidercamModel.hpp"
//#include "SpidercamModel_net.hpp"
//#include "SpidercamModel_sim.hpp"
//#include "SpidercamDollyStatus.hpp"
//#include "SpidercamToolbar.hpp"

#include <QWidget>
#include <QString>
#include <QDockWidget>
#include <QMessageBox>


//Q_DECLARE_METATYPE(spidercam::sWorkingDimensions);
//Q_DECLARE_METATYPE(spidercam::sPosition_1_t);

sExperimentControllerWidgets cart::create_controller(const nlohmann::json& ctrlInfo, bool no_visualization)
{
    // Register our custom spidercam types with Qt's meta type system.  Needed for using signals/slots
	
/*
    qRegisterMetaType<spidercam::sWorkingDimensions>();
    qRegisterMetaType<spidercam::sPosition_1_t>();

    bool use_sim = false;

    if (!ctrlInfo.empty())
    {
        if (ctrlInfo.contains("use_sim"))
        {
            use_sim = ctrlInfo["use_sim"];
        }

        if (use_sim)
        {
            // Message box asking if you really want to use simulation mode 
            QMessageBox msgBox;
            msgBox.setText("You are using the Spidercam Simulator.");
            msgBox.setInformativeText("Are you sure you want the system in simulation mode?");
            msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
            msgBox.setDefaultButton(QMessageBox::Save);
            int ret = msgBox.exec();

            if (ret == QMessageBox::No)
            {
                use_sim = false;
            }
            else if (ret == QMessageBox::Cancel)
            {
                return sExperimentControllerWidgets();
            }
        }
    }


    // Create the Spidercam Controller...
    cSpidercamModel* pModel = nullptr;
    
    if (use_sim)
        pModel = new cSpidercamModel_sim();
    else
        pModel = new cSpidercamModel_net();

    if (no_visualization)
        return sExperimentControllerWidgets(pModel, nullptr);

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

    auto* pToolBar = new cSpidercamToolbar();

//    QObject::connect(pModel, &cSpidercamModel::batteryLevelChanged, pStatusBar, &cBatteryStatus::updateBatteryLevel);

    return sExperimentControllerWidgets(pModel, pView, pDockWidget, pStatusBar, pToolBar);
*/

    return sExperimentControllerWidgets();
}