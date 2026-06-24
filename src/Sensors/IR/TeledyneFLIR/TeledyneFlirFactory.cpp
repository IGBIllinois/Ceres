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

        QObject::connect(pModel, &cSensorModel::sensorStatusChanging,             pView, &cSensorStatusView::onSensorStatusChange);
        QObject::connect(pModel, &cSensorModel::sensorNameChanging,               pView, &cTeledyneFlirStatusView::onSensorNameChanging);
        QObject::connect(pModel, &cTeledyneFlirCameraModel::modeChanged,          pView, &cTeledyneFlirStatusView::onModeChange);
        QObject::connect(pModel, &cTeledyneFlirCameraModel::lapseIntervalChanged, pView, &cTeledyneFlirStatusView::onLapseIntervalChange);
        QObject::connect(pModel, &cTeledyneFlirCameraModel::frameRateChanged,     pView, &cTeledyneFlirStatusView::onFrameRateChange);
        QObject::connect(pModel, &cTeledyneFlirCameraModel::imageSizeChanged,     pView, &cTeledyneFlirStatusView::onImageSizeChange);
        QObject::connect(pModel, &cTeledyneFlirCameraModel::onNewImage,           pView, &cTeledyneFlirStatusView::imageUpdated);

        QObject::connect(pView, &cTeledyneFlirStatusView::requestMode,             pModel, &cTeledyneFlirCameraModel::requestMode);
        QObject::connect(pView, &cTeledyneFlirStatusView::requestFrameRate_Hz,     pModel, &cTeledyneFlirCameraModel::requestFrameRate_Hz);
        QObject::connect(pView, &cTeledyneFlirStatusView::requestLapseInterval_ms, pModel, &cTeledyneFlirCameraModel::requestLapseInterval_ms);
        QObject::connect(pView, &cTeledyneFlirStatusView::requestImage,            pModel, &cTeledyneFlirCameraModel::requestImage);

        auto* pController = new cTeledyneFlirController_T1K(pModel);

        QObject::connect(pModel, &cTeledyneFlirCameraModel::modeChanged,          pController, &cTeledyneFlirController::modeChanged);
        QObject::connect(pModel, &cTeledyneFlirCameraModel::lapseIntervalChanged, pController, &cTeledyneFlirController::lapseIntervalChanged);
        QObject::connect(pModel, &cTeledyneFlirCameraModel::frameRateChanged,     pController, &cTeledyneFlirController::frameRateChanged);
        QObject::connect(pModel, &cTeledyneFlirCameraModel::imageSizeChanged,     pController, &cTeledyneFlirController::imageSizeChanged);
        QObject::connect(pModel, &cTeledyneFlirCameraModel::photoTaken,           pController, &cTeledyneFlirController::photoTaken);

        QObject::connect(pController, &cTeledyneFlirController::requestMode,             pModel, &cTeledyneFlirCameraModel::requestMode);
        QObject::connect(pController, &cTeledyneFlirController::requestFrameRate_Hz,     pModel, &cTeledyneFlirCameraModel::requestFrameRate_Hz);
        QObject::connect(pController, &cTeledyneFlirController::requestLapseInterval_ms, pModel, &cTeledyneFlirCameraModel::requestLapseInterval_ms);
        QObject::connect(pController, &cTeledyneFlirController::requestImage,            pModel, &cTeledyneFlirCameraModel::requestImage);
        QObject::connect(pController, &cTeledyneFlirController::requestImages,           pModel, &cTeledyneFlirCameraModel::requestImages);

        QObject::connect(pController, qOverload<bool>(&cTeledyneFlirController::requestPhoto),       pModel, qOverload<bool>(&cTeledyneFlirCameraModel::takePhoto));
        QObject::connect(pController, qOverload<bool, bool>(&cTeledyneFlirController::requestPhoto), pModel, qOverload<bool, bool>(&cTeledyneFlirCameraModel::takePhoto));

        return sSensorWidgets(pModel, pController, pView);
    }

    auto* dockWidget = new QDockWidget();
    auto* pView = new cTeledyneFlirCameraView_T1K(pModel, dockWidget);
    pView->initialize();

    dockWidget->setWindowTitle(pView->windowTitle());
    dockWidget->setWidget(pView);
    QObject::connect(dockWidget, &QDockWidget::dockLocationChanged, pView, &cTeledyneFlirCameraView::dockLocationChanged);
    QObject::connect(dockWidget, &QDockWidget::topLevelChanged, pView, &cTeledyneFlirCameraView::topLevelChanged);

    QObject::connect(pModel, &cTeledyneFlirCameraModel::sensorNameChanging, pView, &cTeledyneFlirCameraView::onSensorNameChanging);
    QObject::connect(pModel, &cTeledyneFlirCameraModel::modeChanged, pView, &cTeledyneFlirCameraView::onModeChange);
    QObject::connect(pModel, &cTeledyneFlirCameraModel::imageSizeChanged, pView, &cTeledyneFlirCameraView::onImageSizeChange);
    QObject::connect(pModel, &cTeledyneFlirCameraModel::onNewImage, pView, &cTeledyneFlirCameraView::imageUpdated);

    QObject::connect(pView, &cTeledyneFlirCameraView::requestImage, pModel, &cTeledyneFlirCameraModel::requestImage);
    QObject::connect(pView, &cTeledyneFlirCameraView::requestImages, pModel, &cTeledyneFlirCameraModel::requestImages);

    auto page = new cTeledyneFlirPropertyPage_T1K_Local(pModel);

//    QObject::connect(pModel, &cTeledyneFlirCameraModel::modeChanged, pController, &cTeledyneFlirController::modeChanged);
//    QObject::connect(pModel, &cTeledyneFlirCameraModel::lapseIntervalChanged, pController, &cTeledyneFlirController::frameIntervalChanged);
//    QObject::connect(pModel, &cTeledyneFlirCameraModel::frameRateChanged, pController, &cTeledyneFlirController::frameRateChanged);
//    QObject::connect(pModel, &cTeledyneFlirCameraModel::imageSizeChanged, pController, &cTeledyneFlirController::imageSizeChanged);
//    QObject::connect(pModel, &cTeledyneFlirCameraModel::photoTaken, pController, &cTeledyneFlirController::photoTaken);

    QObject::connect(page, &cTeledyneFlirPropertyPage_T1K_Local::requestMode,             pModel, &cTeledyneFlirCameraModel::requestMode);
    QObject::connect(page, &cTeledyneFlirPropertyPage_T1K_Local::requestFrameRate_Hz,     pModel, &cTeledyneFlirCameraModel::requestFrameRate_Hz);
    QObject::connect(page, &cTeledyneFlirPropertyPage_T1K_Local::requestLapseInterval_ms, pModel, &cTeledyneFlirCameraModel::requestLapseInterval_ms);
    QObject::connect(page, &cTeledyneFlirPropertyPage_T1K_Local::requestImage,            pModel, &cTeledyneFlirCameraModel::requestImage);
    QObject::connect(page, &cTeledyneFlirPropertyPage_T1K_Local::requestImages,           pModel, &cTeledyneFlirCameraModel::requestImages);

    QObject::connect(page, qOverload<bool>(&cTeledyneFlirPropertyPage_T1K_Local::requestPhoto),       pModel, qOverload<bool>(&cTeledyneFlirCameraModel::takePhoto));
    QObject::connect(page, qOverload<bool, bool>(&cTeledyneFlirPropertyPage_T1K_Local::requestPhoto), pModel, qOverload<bool, bool>(&cTeledyneFlirCameraModel::takePhoto));

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
    // Teledyne FLIR model and view...
    auto* pModel = dynamic_cast<cTeledyneFlirCameraModel*>(widgets.pModel);
    auto* dockWidget = widgets.pDockableView;
    auto* pView = dynamic_cast<cTeledyneFlirCameraView*>(dockWidget->widget());

    if (pModel && pView)
    {
        QObject::disconnect(pModel, &cTeledyneFlirCameraModel::sensorNameChanging, pView, &cTeledyneFlirCameraView::onSensorNameChanging);
        QObject::disconnect(pModel, &cTeledyneFlirCameraModel::modeChanged, pView, &cTeledyneFlirCameraView::onModeChange);
        QObject::disconnect(pModel, &cTeledyneFlirCameraModel::imageSizeChanged, pView, &cTeledyneFlirCameraView::onImageSizeChange);
        QObject::disconnect(pModel, &cTeledyneFlirCameraModel::onNewImage, pView, &cTeledyneFlirCameraView::imageUpdated);

        QObject::disconnect(pView, &cTeledyneFlirCameraView::requestImage, pModel, &cTeledyneFlirCameraModel::requestImage);
        QObject::disconnect(pView, &cTeledyneFlirCameraView::requestImages, pModel, &cTeledyneFlirCameraModel::requestImages);
    }

    delete pModel;
    delete dockWidget;
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
