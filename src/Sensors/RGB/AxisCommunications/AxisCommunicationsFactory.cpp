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
            pModel->autoEmitImages(false);

            auto* pView = new cAxisCommunicationsStatusView(pModel);

            pView->createWidgets();
            pView->doLayout();

            QObject::connect(pModel, &cAxisCommunicationsModel_F44::cameraIdChanged,  pView, &cAxisCommunicationsStatusView::onCameraIdChange);
            QObject::connect(pModel, &cAxisCommunicationsModel::imageSizeChanged,     pView, &cAxisCommunicationsStatusView::onImageSizeChange);
            QObject::connect(pModel, &cAxisCommunicationsModel::modeChanged,          pView, &cAxisCommunicationsStatusView::onModeChange);
            QObject::connect(pModel, &cAxisCommunicationsModel::frameRateChanged,     pView, &cAxisCommunicationsStatusView::onFrameRateChange);
            QObject::connect(pModel, &cAxisCommunicationsModel::lapseIntervalChanged, pView, &cAxisCommunicationsStatusView::onLapseIntervalChange);
            QObject::connect(pModel, &cAxisCommunicationsModel::onNewImage,           pView, &cAxisCommunicationsStatusView::imageUpdated);

            QObject::connect(pView, &cAxisCommunicationsStatusView::requestImage, pModel, &cAxisCommunicationsModel::requestImage);

            auto* pController = new cAxisCommunicationsController_F44();

            QObject::connect(pModel, &cAxisCommunicationsModel::modeChanged,          pController, &cAxisCommunicationsController_F44::modeChanged);
            QObject::connect(pModel, &cAxisCommunicationsModel::lapseIntervalChanged, pController, &cAxisCommunicationsController_F44::lapseIntervalChanged);
            QObject::connect(pModel, &cAxisCommunicationsModel::frameRateChanged,     pController, &cAxisCommunicationsController_F44::frameRateChanged);
            QObject::connect(pModel, &cAxisCommunicationsModel::imageSizeChanged,     pController, &cAxisCommunicationsController_F44::imageSizeChanged);

            QObject::connect(pModel, &cAxisCommunicationsModel_F44::cameraIdChanged,  pController, &cAxisCommunicationsController_F44::cameraIdChanged);
            QObject::connect(pModel, &cAxisCommunicationsModel_F44::stateInfoUpdate,  pController, &cAxisCommunicationsController_F44::stateUpdated);

            QObject::connect(pModel, &cAxisCommunicationsModel::photoTaken,           pController, &cAxisCommunicationsController_F44::photoTaken);

            QObject::connect(pController, &cAxisCommunicationsController_F44::queryMode,          pModel, &cAxisCommunicationsModel::modeQueried);
            QObject::connect(pController, &cAxisCommunicationsController_F44::queryState,         pModel, &cAxisCommunicationsModel_F44::stateQueried);
            QObject::connect(pController, &cAxisCommunicationsController_F44::queryCameraId,      pModel, &cAxisCommunicationsModel_F44::cameraIdQueried);
            QObject::connect(pController, &cAxisCommunicationsController_F44::queryImageSize,     pModel, &cAxisCommunicationsModel::imageSizeQueried);
            QObject::connect(pController, &cAxisCommunicationsController_F44::queryFrameRate,     pModel, &cAxisCommunicationsModel::frameRateQueried);
            QObject::connect(pController, &cAxisCommunicationsController_F44::queryLapseInterval, pModel, &cAxisCommunicationsModel::lapseIntervalQueried);

            QObject::connect(pController, &cAxisCommunicationsController_F44::requestImageSize,        pModel, &cAxisCommunicationsModel::requestImageSize);
            QObject::connect(pController, &cAxisCommunicationsController_F44::requestMode,             pModel, &cAxisCommunicationsModel::requestMode);
            QObject::connect(pController, &cAxisCommunicationsController_F44::requestFrameRate_Hz,     pModel, &cAxisCommunicationsModel::requestFrameRate_Hz);
            QObject::connect(pController, &cAxisCommunicationsController_F44::requestLapseInterval_ms, pModel, &cAxisCommunicationsModel::requestLapseInterval_ms);
            QObject::connect(pController, &cAxisCommunicationsController_F44::requestImage,            pModel, &cAxisCommunicationsModel::requestImage);
            QObject::connect(pController, &cAxisCommunicationsController_F44::requestImages,           pModel, &cAxisCommunicationsModel::requestImages);

            QObject::connect(pController, &cAxisCommunicationsController_F44::requestSaveState,    pModel, &cAxisCommunicationsModel_F44::onSaveState);
            QObject::connect(pController, &cAxisCommunicationsController_F44::requestRestoreState, pModel, &cAxisCommunicationsModel_F44::onRestoreState);

            QObject::connect(pController, qOverload<bool>(&cAxisCommunicationsController_F44::requestPhoto),       pModel, qOverload<bool>(&cAxisCommunicationsModel::takePhoto));
            QObject::connect(pController, qOverload<bool, bool>(&cAxisCommunicationsController_F44::requestPhoto), pModel, qOverload<bool, bool>(&cAxisCommunicationsModel::takePhoto));

            return sSensorWidgets(pModel, pController, pView);
        }

        return sSensorWidgets(pModel);
    }

    auto* dockWidget = new QDockWidget();
    auto* pView = new cAxisCommunicationsView_F44(pModel, dockWidget);

    pView->initialize();

    pView->connectToModel();

    dockWidget->setWindowTitle(pView->windowTitle());
    dockWidget->setWidget(pView);

    QObject::connect(dockWidget, &QDockWidget::dockLocationChanged, pView, &cAxisCommunicationsView::dockLocationChanged);
    QObject::connect(dockWidget, &QDockWidget::topLevelChanged,     pView, &cAxisCommunicationsView::topLevelChanged);


    auto pPage = new cAxisCommunicationsPropertyPage_Local_F44(pModel);

    pPage->connectToModel();

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
    auto* pModel = widgets.pModel;
    auto* dockWidget = widgets.pDockableView;
    auto* pView = dockWidget->widget();

    pModel->disconnect();
    pView->disconnect();

    pModel->deleteLater();
    dockWidget->deleteLater();
}


cSensorPropertyPage* axis_communications::create_sensor_property_page(
    const std::string& model, uint32_t version,
    const std::string& remote_ip_address, uint16_t port, const std::string& local_ip_address)
{
    cAxisCommunicationsPropertyPage_Remote* page = new cAxisCommunicationsPropertyPage_Remote_F44();

    page->initialize(remote_ip_address, port, false, local_ip_address);

    return page;
}
