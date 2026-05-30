/**
 */

#include "LucidVisionLabsRgbCameraFactory.hpp"
#include "LucidVisionLabsIDs.hpp"

#include <lucid_vision_labs_connect/LucidCameraFactory.hpp>
#include <lucid_vision_labs_connect/LucidTritonCamera.hpp>

#include "LucidVisionLabsRgbModel_Triton.hpp"
#include "LucidVisionLabsRgbView_Triton.hpp"
#include "LucidVisionLabsRgbStatusView.hpp"
#include "LucidVisionLabsRgbController.hpp"
#include "LucidVisionLabsRgbPropertyPage_Remote.hpp"
#include "LucidVisionLabsRgbPropertyPage_Local.hpp"

#include "StringUtils.hpp"

#include <QWidget>
#include <QString>
#include <QDockWidget>
#include <QMetaType>

// Example of how to declare a metatype in Qt
//Q_DECLARE_METATYPE(ouster::sensor_info_t);

sSensorWidgets create_triton_sensor(const nlohmann::json& sensorInfo, bool no_visualization)
{
    if (!sensorInfo.contains("serial_number"))
    {
        qCritical() << "The \"serial_number\" entry is missing from the \"lucid_vision_labs\" sensor section.";
        return sSensorWidgets();
    }

    std::string serial_number = sensorInfo["serial_number"];

    cLucidCameraFactory factory;

    auto camera = factory.getTritonCamera(serial_number);

    if (!camera)
        return sSensorWidgets();

    // Create the Triton model and view...
    cLucidVisionLabsRgbModel_Triton* pModel = new cLucidVisionLabsRgbModel_Triton(std::move(camera));

    if (no_visualization)
    {
/*
        if (protocol == "net")
        {
            auto* pView = new cLucidVisionLabsRgbStatusView(pModel);
            pView->createWidgets();
            pView->doLayout();

            QObject::connect(pModel, &cSensorModel::sensorStatusChanging, pView, &cSensorStatusView::onSensorStatusChange);
            QObject::connect(pModel, &cAxisCommunicationsModel::cameraIdChanged, pView, &cAxisCommunicationsStatusView::onCameraIdChange);
            QObject::connect(pModel, &cAxisCommunicationsModel::frameRateChanged, pView, &cAxisCommunicationsStatusView::onFrameRateChange);
            QObject::connect(pModel, &cAxisCommunicationsModel::imageSizeChanged, pView, &cAxisCommunicationsStatusView::onImageSizeChange);

            auto* pController = new cAxisCommunicationsController_F44(pModel);
            return sSensorWidgets(pModel, pController, pView);
        }
*/
        return sSensorWidgets(pModel);
    }

    auto* dockWidget = new QDockWidget();
    auto* pView = new cLucidVisionLabsRgbView_Triton(pModel, dockWidget);
    pView->initialize();

    dockWidget->setWindowTitle(pView->windowTitle());
    dockWidget->setWidget(pView);

    QObject::connect(dockWidget, &QDockWidget::dockLocationChanged, pView, &cLucidVisionLabsRgbView::dockLocationChanged);
    QObject::connect(dockWidget, &QDockWidget::topLevelChanged,     pView, &cLucidVisionLabsRgbView::topLevelChanged);

    QObject::connect(pModel, &cLucidVisionLabsRgbModel::sensorNameChanging, pView, &cLucidVisionLabsRgbView::onSensorNameChanging);
    QObject::connect(pModel, &cLucidVisionLabsRgbModel::modeChanged,        pView, &cLucidVisionLabsRgbView::onModeChange);
    QObject::connect(pModel, &cLucidVisionLabsRgbModel::imageSizeChanged,   pView, &cLucidVisionLabsRgbView::onImageSizeChange);
    QObject::connect(pModel, &cLucidVisionLabsRgbModel::onNewImage,         pView, &cLucidVisionLabsRgbView::imageUpdated);

    QObject::connect(pView, &cLucidVisionLabsRgbView::requestImage,  pModel, &cLucidVisionLabsRgbModel::requestImage);
    QObject::connect(pView, &cLucidVisionLabsRgbView::requestImages, pModel, &cLucidVisionLabsRgbModel::requestImages);

    auto pPage = new cLucidVisionLabsRgbPropertyPage_Local_Triton(pModel);

    QObject::connect(pPage, &cLucidVisionLabsRgbPropertyPage_Local::requestPhoto,           pModel, &cLucidVisionLabsRgbModel_Triton::takePhoto);

    QObject::connect(pPage, &cLucidVisionLabsRgbPropertyPage_Local::pushStreamState,        pModel, &cLucidVisionLabsRgbModel_Triton::pushStreamState);
    QObject::connect(pPage, &cLucidVisionLabsRgbPropertyPage_Local::popStreamState,         pModel, &cLucidVisionLabsRgbModel_Triton::popStreamState);
    QObject::connect(pPage, &cLucidVisionLabsRgbPropertyPage_Local::changeMode,             pModel, &cLucidVisionLabsRgbModel_Triton::requestMode);
    QObject::connect(pPage, &cLucidVisionLabsRgbPropertyPage_Local::changeFrameRate_Hz,     pModel, &cLucidVisionLabsRgbModel_Triton::requestFrameRate_Hz);
    QObject::connect(pPage, &cLucidVisionLabsRgbPropertyPage_Local::changeLapseInterval_ms, pModel, &cLucidVisionLabsRgbModel_Triton::requestLapseInterval_ms);
    QObject::connect(pPage, &cLucidVisionLabsRgbPropertyPage_Local::changePixelFormat,      pModel, &cLucidVisionLabsRgbModel_Triton::requestPixelFormat);
    QObject::connect(pPage, &cLucidVisionLabsRgbPropertyPage_Local::changeExposure,         pModel, &cLucidVisionLabsRgbModel_Triton::requestExposure);
    QObject::connect(pPage, &cLucidVisionLabsRgbPropertyPage_Local::changeGain,             pModel, &cLucidVisionLabsRgbModel_Triton::requestGain);
    QObject::connect(pPage, &cLucidVisionLabsRgbPropertyPage_Local::changeBalanceWhiteAuto, pModel, &cLucidVisionLabsRgbModel_Triton::requestBalanceWhiteAuto);
    QObject::connect(pPage, &cLucidVisionLabsRgbPropertyPage_Local::changeGamma,            pModel, &cLucidVisionLabsRgbModel_Triton::requestGamma);

    QObject::connect(pModel, &cLucidVisionLabsRgbModel_Triton::photoTaken,              pPage, &cLucidVisionLabsRgbPropertyPage_Local::photoTaken);
    QObject::connect(pModel, &cLucidVisionLabsRgbModel_Triton::pixelFormatChanged,      pPage, &cLucidVisionLabsRgbPropertyPage_Local::pixelFormatUpdated);
    QObject::connect(pModel, &cLucidVisionLabsRgbModel_Triton::exposureChanged,         pPage, &cLucidVisionLabsRgbPropertyPage_Local::exposureUpdated);
    QObject::connect(pModel, &cLucidVisionLabsRgbModel_Triton::gainChanged,             pPage, &cLucidVisionLabsRgbPropertyPage_Local::gainUpdated);
    QObject::connect(pModel, &cLucidVisionLabsRgbModel_Triton::balanceWhiteAutoChanged, pPage, &cLucidVisionLabsRgbPropertyPage_Local::balanceWhiteAutoUpdated);
    QObject::connect(pModel, &cLucidVisionLabsRgbModel_Triton::gammaChanged,            pPage, &cLucidVisionLabsRgbPropertyPage_Local::gammaUpdated);

    return sSensorWidgets(pModel, dockWidget, pPage);
}

sSensorWidgets lucid_vision_labs_rgb::create_sensor(const nlohmann::json& sensorInfo,
    bool no_visualization)
{
    if (!sensorInfo.contains("sensor"))
    {
        qCritical() << "The \"sensor\" entry is missing from the \"lucid_vision_labs\" sensor section.  ";
        qCritical() << "Valid values are: triton.";
        return sSensorWidgets();
    }

    std::string sensor = sensorInfo["sensor"];

    if (nStringUtils::iequal(sensor, lucid_triton_id))
        return create_triton_sensor(sensorInfo, no_visualization);

    return sSensorWidgets();
}

void lucid_vision_labs_rgb::remove_sensor(sSensorWidgets widgets)
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


cSensorPropertyPage* lucid_vision_labs_rgb::create_sensor_property_page(
    const std::string& model, uint32_t version,
    const std::string& remote_ip_address, uint16_t port, const std::string& local_ip_address)
{
//    cAxisCommunicationsPropertyPage_Remote* page = new cAxisCommunicationsPropertyPage_Remote();
//    page->initialize(remote_ip_address, port, false, local_ip_address);

//    return page;
    return nullptr;
}
