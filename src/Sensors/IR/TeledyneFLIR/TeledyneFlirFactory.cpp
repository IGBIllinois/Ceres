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

#include <TeledyneAtlasConnect/TeledyneFlirCameraFactory.hpp>
#include <TeledyneAtlasConnect/TeledyneFlirCamera.hpp>

#include <QWidget>
#include <QString>
#include <QDockWidget>
#include <QMetaType>
#include <QDebug>


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

    factory.discoverCameras(protocol);

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

        QObject::connect(pModel, &cSensorModel::sensorStatusChanging, pView, &cSensorStatusView::onSensorStatusChange);
        QObject::connect(pModel, &cTeledyneFlirCameraModel::modeChanged, pView, &cTeledyneFlirStatusView::onModeChange);
        QObject::connect(pModel, &cTeledyneFlirCameraModel::frameIntervalChanged, pView, &cTeledyneFlirStatusView::onFrameIntervalChange);
        QObject::connect(pModel, &cTeledyneFlirCameraModel::frameRateChanged, pView, &cTeledyneFlirStatusView::onFrameRateChange);
        QObject::connect(pModel, &cTeledyneFlirCameraModel::imageSizeChanged, pView, &cTeledyneFlirStatusView::onImageSizeChange);

        auto* pController = new cTeledyneFlirController_T1K(pModel);
        return sSensorWidgets(pModel, pController, pView);
    }

    auto* dockWidget = new QDockWidget();
    auto* pView = new cTeledyneFlirCameraView_T1K(pModel, dockWidget);
    pView->initialize();

    dockWidget->setWindowTitle(pView->windowTitle());
    dockWidget->setWidget(pView);
    QObject::connect(dockWidget, &QDockWidget::dockLocationChanged, pView, &cTeledyneFlirCameraView::dockLocationChanged);
    QObject::connect(dockWidget, &QDockWidget::topLevelChanged, pView, &cTeledyneFlirCameraView::topLevelChanged);

//    QObject::connect(pModel, &cAxisCommunicationsModel_F44::enableCamera, pView, &cAxisCommunicationsView_F44::enableCamera);
    QObject::connect(pModel, &cTeledyneFlirCameraModel::onNewImage, pView, &cTeledyneFlirCameraView::imageUpdated);

//    QObject::connect(pView, &cAxisCommunicationsView_F44::activateCamera, pModel, &cAxisCommunicationsModel_F44::setActiveCamera);

    auto page = new cTeledyneFlirPropertyPage_Local(pModel);

    return sSensorWidgets(pModel, dockWidget, page);
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
/*
    // Ouster model and view...
    auto* pModel = static_cast<cOusterModel*>(widgets.pModel);
    auto* dockWidget = widgets.pDockableView;
    auto* pView = static_cast<cOusterView*>(dockWidget->widget());

    QObject::disconnect(pModel, &cOusterModel::updateBeamIntrinsics, pView, &cOusterView::beamIntrinsicsChanged);
    QObject::disconnect(pModel, &cOusterModel::updateImuIntrinsics, pView, &cOusterView::imuIntrinsicsChanged);
    QObject::disconnect(pModel, &cOusterModel::updateLidarIntrinsics, pView, &cOusterView::lidarIntrinsicsChanged);
    QObject::disconnect(pModel, &cOusterModel::updateDataFormat, pView, &cOusterView::dataFormatChanged);
    QObject::disconnect(pModel, &cOusterModel::updateAzimuthWindow, pView, &cOusterView::azimuthWindowChanged);
//    QObject::disconnect(pModel, &cOusterModel::updateEncoderCount, pView, &cOusterView::encoderCountChanged);
    QObject::disconnect(pModel, &cOusterModel::updateImuData, pView, &cOusterView::imuDataChanged);
    QObject::disconnect(pModel, &cOusterModel::updateLidarData, pView, &cOusterView::displayData);

    delete pModel;
    delete dockWidget;
*/
}


cSensorPropertyPage* teledyne_flir::create_sensor_property_page(
    const std::string& model, uint32_t version,
    const std::string& remote_ip_address, uint16_t port, const std::string& local_ip_address)
{
    if (model == teledyne_flir_id)
    {
        auto page = new cTeledyneFlirPropertyPage_Remote();
        page->initialize(remote_ip_address, port, false, local_ip_address);
        return page;
    }

    return nullptr;
}
