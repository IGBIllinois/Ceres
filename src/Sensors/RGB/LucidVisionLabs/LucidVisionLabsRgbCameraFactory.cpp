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

            auto* pController = new cLucidVisionLabsRgbController_Triton(pModel);


            return sSensorWidgets(pModel, pController, pView);
        }
*/
        return sSensorWidgets(pModel);
    }

    auto* dockWidget = new QDockWidget();

    auto* pView = new cLucidVisionLabsRgbView_Triton(pModel, dockWidget);

    pView->initialize();

    pView->connectToModel();

    dockWidget->setWindowTitle(pView->windowTitle());
    dockWidget->setWidget(pView);

    QObject::connect(dockWidget, &QDockWidget::dockLocationChanged, pView, &cLucidVisionLabsRgbView::dockLocationChanged);
    QObject::connect(dockWidget, &QDockWidget::topLevelChanged,     pView, &cLucidVisionLabsRgbView::topLevelChanged);

    auto pPage = new cLucidVisionLabsRgbPropertyPage_Local_Triton(pModel);

    pPage->connectToModel();

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
    auto* pModel = widgets.pModel;

    auto* dockWidget = widgets.pDockableView;
    auto* pView = dockWidget->widget();

    pModel->disconnect();
    pView->disconnect();

    pModel->deleteLater();
    dockWidget->deleteLater();
}


cSensorPropertyPage* lucid_vision_labs_rgb::create_sensor_property_page(
    const std::string& model, uint32_t version,
    const std::string& remote_ip_address, uint16_t port, const std::string& local_ip_address)
{
    cLucidVisionLabsRgbPropertyPage_Remote* page = new cLucidVisionLabsRgbPropertyPage_Remote();
    page->initialize(remote_ip_address, port, false, local_ip_address);

    return page;
}
