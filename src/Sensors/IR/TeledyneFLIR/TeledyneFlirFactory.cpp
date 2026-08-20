/**
 */

#include "TeledyneFlirFactory.hpp"
#include "TeledyneFlirIDs.hpp"
#include "TeledyneFlirCameraModel_T1K.hpp"
#include "TeledyneFlirCameraView_T1K.hpp"
#include "TeledyneFlirStatusView.hpp"
#include "TeledyneFlirController.hpp"
#include "TeledyneFlirPropertyPage.hpp"
#include "TeledyneFlirPropertyPage_Remote.hpp"
#include "TeledyneFlirPropertyPage_Local.hpp"

#include <teledyne_atlas_connect/TeledyneFlirCameraFactory.hpp>
#include <teledyne_atlas_connect/TeledyneFlirCamera.hpp>

#include <QWidget>
#include <QString>
#include <QDockWidget>
#include <QMetaType>
#include <QDebug>
#include <QOverload>


// Example of how to declare a metatype in Qt
//Q_DECLARE_METATYPE(ouster::sensor_info_t);

sSensorWidgets create_teledyne_flir_TIK_sensor(const std::string& sensorName, const nlohmann::json& sensorInfo, bool no_visualization)
{
    if (!sensorInfo.contains("protocol"))
    {
        qCritical() << "The \"protocol\" entry is missing from the \"teledyne_flir\" sensor section.  ";
        qCritical() << "Valid values are: usb, network, or emulator";
        return sSensorWidgets();
    }

    cTeledyneFlirCameraFactory factory;

    std::string protocol = sensorInfo["protocol"];

    long long timeout_sec = 5;
    if (sensorInfo.contains("timeout (s)"))
        timeout_sec = sensorInfo["timeout (s)"].get<long long>();

    factory.discoverCameras(protocol, timeout_sec);

    if (factory.empty())
    {
        qCritical() << "No cameras were found!";
        return sSensorWidgets();
    }

    auto camera = factory.getCamera(sensorName);

    if (!camera)
        return sSensorWidgets();

    // Create the Teledyne FLIR T1K model and view...
    cTeledyneFlirCameraModel_T1K* pModel = new cTeledyneFlirCameraModel_T1K(std::move(camera));

    if (no_visualization)
    {
        auto* pView = new cTeledyneFlirStatusView(pModel);

        pView->createWidgets();
        pView->doLayout();

        pView->connectToModel();

        auto* pController = new cTeledyneFlirController_T1K(pModel);

        pController->connectToModel();

        return sSensorWidgets(pModel, pController, pView);
    }

    auto* dockWidget = new QDockWidget();
    auto* pView = new cTeledyneFlirCameraView_T1K(pModel, dockWidget);

    pView->initialize();
    pView->connectToModel();

    dockWidget->setWindowTitle(pView->windowTitle());
    dockWidget->setWidget(pView);

    QObject::connect(dockWidget, &QDockWidget::dockLocationChanged, pView, &cTeledyneFlirCameraView::dockLocationChanged);
    QObject::connect(dockWidget, &QDockWidget::topLevelChanged,     pView, &cTeledyneFlirCameraView::topLevelChanged);

    auto pPage = new cTeledyneFlirPropertyPage_T1K_Local(pModel);

    pPage->connectToModel();

    return sSensorWidgets(pModel, dockWidget, pPage);
}

sSensorWidgets teledyne_flir::create_sensor(const nlohmann::json& sensorInfo, bool no_visualization)
{
    if (!sensorInfo.contains("sensor"))
    {
        qCritical() << "The \"sensor\" entry is missing from the \"teledyne_flir\" sensor section.  ";
        qCritical() << "Valid values are: UVC Camera";
        return sSensorWidgets();
    }

    std::string sensor = sensorInfo["sensor"];

    if ((sensor == "UVC Camera") || (sensor.starts_with("Emulated")))
        return create_teledyne_flir_TIK_sensor(sensor, sensorInfo, no_visualization);

    return sSensorWidgets();
}

void teledyne_flir::remove_sensor(sSensorWidgets widgets)
{
    auto* pModel = widgets.pModel;

    auto* dockWidget = widgets.pDockableView;
    auto* pView = dockWidget->widget();

    pModel->disconnect();
    pView->disconnect();

    pModel->deleteLater();
    dockWidget->deleteLater();;
}


cSensorPropertyPage* teledyne_flir::create_sensor_property_page(
    const std::string& model, uint32_t version,
    const std::string& remote_ip_address, uint16_t port, const std::string& local_ip_address)
{
//    if (model == teledyne_flir_id)    <- We only need this if we are model dependant
    {
        auto page = new cTeledyneFlirPropertyPage_Remote();
        page->initialize(remote_ip_address, port, false, local_ip_address);
        return page;
    }

    return nullptr;
}
