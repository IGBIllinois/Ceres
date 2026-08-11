/**
 */

#include "AxisCommunicationsFactory.hpp"

#include "AxisCommunicationsModel_F44.hpp"
#include "AxisCommunicationsView_F44.hpp"
#include "AxisCommunicationsStatusView.hpp"
#include "AxisCommunicationsController.hpp"
#include "AxisCommunicationsPropertyPage_Remote.hpp"
#include "AxisCommunicationsPropertyPage_Local.hpp"

#include <QWidget>
#include <QString>
#include <QDockWidget>
#include <QMetaType>

// Example of how to declare a metatype in Qt
//Q_DECLARE_METATYPE(ouster::sensor_info_t);

sSensorWidgets create_axis_communications_f44_sensor(const nlohmann::json& sensorInfo, bool no_visualization)
{
    // Create the Ouster model and view...
    cAxisCommunicationsModel_F44* pModel = nullptr;

    std::string protocol = sensorInfo["protocol"];

    if (protocol == "net")
        pModel = new cAxisCommunicationsModel_F44();
    else if (protocol == "file")
        pModel = new cAxisCommunicationsModel_F44();

    if (!pModel)
        throw std::runtime_error("Axis F44: Unknown protocol type!");


    if (no_visualization)
    {
        if (protocol == "net")
        {
            auto* pView = new cAxisCommunicationsStatusView(pModel);
            pView->createWidgets();
            pView->doLayout();

            pModel->autoEmitImages(false);

            QObject::connect(pModel, &cSensorModel::sensorStatusChanging, pView, &cSensorStatusView::onSensorStatusChange);
            QObject::connect(pModel, &cAxisCommunicationsModel::cameraIdChanged, pView, &cAxisCommunicationsStatusView::onCameraIdChange);
            QObject::connect(pModel, &cAxisCommunicationsModel::frameRateChanged, pView, &cAxisCommunicationsStatusView::onFrameRateChange);
            QObject::connect(pModel, &cAxisCommunicationsModel::imageSizeChanged, pView, &cAxisCommunicationsStatusView::onImageSizeChange);
            QObject::connect(pModel, &cAxisCommunicationsModel::onNewImage, pView, &cAxisCommunicationsStatusView::imageUpdated);

            QObject::connect(pView, &cAxisCommunicationsStatusView::requestImage, pModel, &cAxisCommunicationsModel::requestImage);

            auto* pController = new cAxisCommunicationsController_F44(pModel);
            return sSensorWidgets(pModel, pController, pView);
        }

        return sSensorWidgets(pModel);
    }

    auto* dockWidget = new QDockWidget();
    auto* pView = new cAxisCommunicationsView_F44(pModel, dockWidget);
    pView->initialize();

    dockWidget->setWindowTitle(pView->windowTitle());
    dockWidget->setWidget(pView);
    QObject::connect(dockWidget, &QDockWidget::dockLocationChanged, pView, &cAxisCommunicationsView::dockLocationChanged);
    QObject::connect(dockWidget, &QDockWidget::topLevelChanged, pView, &cAxisCommunicationsView::topLevelChanged);

    QObject::connect(pModel, &cAxisCommunicationsModel_F44::enableCamera, pView, &cAxisCommunicationsView_F44::enableCamera);
    QObject::connect(pModel, &cAxisCommunicationsModel::onNewImage, pView, &cAxisCommunicationsView::imageUpdated);

    QObject::connect(pView, &cAxisCommunicationsView_F44::activateCamera, pModel, &cAxisCommunicationsModel_F44::setActiveCamera);


    auto pPage = new cAxisCommunicationsPropertyPage_Local(pModel);

    return sSensorWidgets(pModel, dockWidget, pPage);
}

sSensorWidgets axis_communications::create_sensor(const nlohmann::json& sensorInfo,
    bool no_visualization)
{
    std::string sensor = sensorInfo["sensor"];

    if (sensor == "F44")
        return create_axis_communications_f44_sensor(sensorInfo, no_visualization);

    return sSensorWidgets();
}

void axis_communications::remove_sensor(sSensorWidgets widgets)
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


cSensorPropertyPage* axis_communications::create_sensor_property_page(
    const std::string& model, uint32_t version,
    const std::string& remote_ip_address, uint16_t port, const std::string& local_ip_address)
{
    cAxisCommunicationsPropertyPage_Remote* page = new cAxisCommunicationsPropertyPage_Remote();
    page->initialize(remote_ip_address, port, false, local_ip_address);

    return page;
}
