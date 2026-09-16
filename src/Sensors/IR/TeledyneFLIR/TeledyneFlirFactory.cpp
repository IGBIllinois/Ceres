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

#include "TeledyneDiscoverCameras.hpp"

#include "..\..\..\Utilities\Timers.hpp"

#include <teledyne_atlas_connect/TeledyneFlirCameraFactory.hpp>
#include <teledyne_atlas_connect/TeledyneFlirCamera.hpp>

#include <QWidget>
#include <QString>
#include <QDockWidget>
#include <QMetaType>
#include <QDebug>
#include <QOverload>
#include <QCoreApplication>
#include <QAbstractEventDispatcher>


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

    std::string protocol = sensorInfo["protocol"];

    if ((protocol != "usb") && (protocol != "network") && (protocol != "emulator"))
    {
        qCritical() << "The \"protocol\" entry is invalid: " << protocol;
        qCritical() << "Valid values are: usb, network, or emulator";
        return sSensorWidgets();
    }

    long long timeout_sec = 15;
    if (sensorInfo.contains("timeout (s)"))
        timeout_sec = sensorInfo["timeout (s)"].get<long long>();

    if (timeout_sec < 5)
        timeout_sec = 5;

    std::unique_ptr<cTeledyneFlirCameraFactory> pFactory = std::make_unique<cTeledyneFlirCameraFactory>();

    auto pThread = new cDiscoverThread(pFactory.get(), protocol, timeout_sec);

    bool discoveryComplete = false;

    QObject::connect(pThread, &cDiscoverThread::discoverComplete, [&discoveryComplete]()
        {
            discoveryComplete = true; 
        });
    QObject::connect(pThread, &cDiscoverThread::discoverComplete, pThread, &cDiscoverThread::deleteLater);

    cOneShotTimer time_out;
    time_out.time_sec(2 * timeout_sec);

    pThread->startSearchForCameras();
    time_out.start();

    QAbstractEventDispatcher* pDispatcher = QCoreApplication::instance()->eventDispatcher();

    while (!discoveryComplete)
    {
        pDispatcher->processEvents(QEventLoop::ExcludeUserInputEvents);

        if (time_out.elapsed())
            break;
    }

    if (pFactory->empty())
    {
        qCritical() << "No cameras were found!";
        return sSensorWidgets();
    }

    auto camera = pFactory->getCamera(sensorName);

    if (!camera)
        return sSensorWidgets();

    // Create the Teledyne FLIR T1K model and view...
    cTeledyneFlirCameraModel_T1K* pModel = new cTeledyneFlirCameraModel_T1K(std::move(camera));

    if (no_visualization)
    {
        auto* pView = new cTeledyneFlirStatusView(pModel);

        pView->createWidgets();
        pView->doLayout();

        QObject::connect(pModel, &cTeledyneFlirCameraModel::modeChanged,          pView, &cTeledyneFlirStatusView::onModeChange);
        QObject::connect(pModel, &cTeledyneFlirCameraModel::lapseIntervalChanged, pView, &cTeledyneFlirStatusView::onLapseIntervalChange);
        QObject::connect(pModel, &cTeledyneFlirCameraModel::frameRateChanged,     pView, &cTeledyneFlirStatusView::onFrameRateChange);
        QObject::connect(pModel, &cTeledyneFlirCameraModel::imageSizeChanged,     pView, &cTeledyneFlirStatusView::onImageSizeChange);
        QObject::connect(pModel, &cTeledyneFlirCameraModel::onNewImage,           pView, &cTeledyneFlirStatusView::imageUpdated);

        QObject::connect(pModel, &cTeledyneFlirCameraModel_T1K::thermalRangeChanged, pView, &cTeledyneFlirStatusView::thermalRangeUpdated);

        QObject::connect(pView, &cTeledyneFlirStatusView::requestMode,             pModel, &cTeledyneFlirCameraModel::requestMode);
        QObject::connect(pView, &cTeledyneFlirStatusView::requestFrameRate_Hz,     pModel, &cTeledyneFlirCameraModel::requestFrameRate_Hz);
        QObject::connect(pView, &cTeledyneFlirStatusView::requestLapseInterval_ms, pModel, &cTeledyneFlirCameraModel::requestLapseInterval_ms);
        QObject::connect(pView, &cTeledyneFlirStatusView::requestImage,            pModel, &cTeledyneFlirCameraModel::requestImage);
        QObject::connect(pView, &cTeledyneFlirStatusView::requestImages,           pModel, &cTeledyneFlirCameraModel::requestImages);

        QObject::connect(pModel, &cIrCameraModel::colorModelUpdated,           pView, &cTeledyneFlirStatusView::colorPaletteUpdated);
        QObject::connect(pView, &cTeledyneFlirStatusView::requestColorPalette, pModel, &cIrCameraModel::setColorModel);


        auto* pController = new cTeledyneFlirController_T1K();

        QObject::connect(pModel, &cTeledyneFlirCameraModel::modeChanged,          pController, &cTeledyneFlirController::modeChanged);
        QObject::connect(pModel, &cTeledyneFlirCameraModel::lapseIntervalChanged, pController, &cTeledyneFlirController::lapseIntervalChanged);
        QObject::connect(pModel, &cTeledyneFlirCameraModel::frameRateChanged,     pController, &cTeledyneFlirController::frameRateChanged);
        QObject::connect(pModel, &cTeledyneFlirCameraModel::imageSizeChanged,     pController, &cTeledyneFlirController::imageSizeChanged);
        QObject::connect(pModel, &cTeledyneFlirCameraModel::photoTaken,           pController, &cTeledyneFlirController::photoTaken);

        QObject::connect(pController, &cTeledyneFlirController::queryMode,             pModel, &cTeledyneFlirCameraModel::queryMode);
        QObject::connect(pController, &cTeledyneFlirController::queryFrameRate_Hz,     pModel, &cTeledyneFlirCameraModel::queryFrameRate);
        QObject::connect(pController, &cTeledyneFlirController::queryLapseInterval_ms, pModel, &cTeledyneFlirCameraModel::queryLapseInterval);
        QObject::connect(pController, &cTeledyneFlirController::queryImageSize,        pModel, &cTeledyneFlirCameraModel::queryImageSize);

        QObject::connect(pController, &cTeledyneFlirController::requestMode,             pModel, &cTeledyneFlirCameraModel::requestMode);
        QObject::connect(pController, &cTeledyneFlirController::requestFrameRate_Hz,     pModel, &cTeledyneFlirCameraModel::requestFrameRate_Hz);
        QObject::connect(pController, &cTeledyneFlirController::requestLapseInterval_ms, pModel, &cTeledyneFlirCameraModel::requestLapseInterval_ms);
        QObject::connect(pController, &cTeledyneFlirController::requestImageSize,        pModel, &cTeledyneFlirCameraModel::requestImageSize);

        QObject::connect(pController, &cTeledyneFlirController::requestImage,            pModel, &cTeledyneFlirCameraModel::requestImage);
        QObject::connect(pController, &cTeledyneFlirController::requestImages,           pModel, &cTeledyneFlirCameraModel::requestImages);

        QObject::connect(pController, &cTeledyneFlirController::requestSaveState,    pModel, &cTeledyneFlirCameraModel::onSaveState);
        QObject::connect(pController, &cTeledyneFlirController::requestRestoreState, pModel, &cTeledyneFlirCameraModel::onRestoreState);


        QObject::connect(pController, &cTeledyneFlirController_T1K::queryState,        pModel, &cTeledyneFlirCameraModel_T1K::onQueryState);
        QObject::connect(pController, &cTeledyneFlirController_T1K::queryThermalRange, pModel, &cTeledyneFlirCameraModel_T1K::onQueryThermalRange);

        QObject::connect(pController, &cTeledyneFlirController_T1K::requestThermalRange, pModel, &cTeledyneFlirCameraModel_T1K::requestThermalRange);

        QObject::connect(pController, &cTeledyneFlirController_T1K::grabImage,         pModel, &cTeledyneFlirCameraModel_T1K::onGrabImage);

        QObject::connect(pModel, &cTeledyneFlirCameraModel_T1K::stateChanged,        pController, &cTeledyneFlirController_T1K::stateUpdated);
        QObject::connect(pModel, &cTeledyneFlirCameraModel_T1K::thermalRangeChanged, pController, &cTeledyneFlirController_T1K::thermalRangeUpdated);


        QObject::connect(pController, qOverload<bool>(&cTeledyneFlirController::requestPhoto),       pModel, qOverload<bool>(&cTeledyneFlirCameraModel::takePhoto));
        QObject::connect(pController, qOverload<bool, bool>(&cTeledyneFlirController::requestPhoto), pModel, qOverload<bool, bool>(&cTeledyneFlirCameraModel::takePhoto));

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
