/**
 */

#include "AxisCommunicationsFactory.hpp"

#include "AxisCommunicationsModel_F44.hpp"
#include "AxisCommunicationsView_F44.hpp"

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
        return sSensorWidgets(pModel, nullptr);

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

    return sSensorWidgets(pModel, dockWidget);
}

sSensorWidgets create_axis_communications_sensor(const nlohmann::json& sensorInfo,
    bool no_visualization)
{
    std::string sensor = sensorInfo["sensor"];

    if (sensor == "F44")
        return create_axis_communications_f44_sensor(sensorInfo, no_visualization);

    return sSensorWidgets();
}

void remove_axis_communications_sensor(sSensorWidgets widgets)
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
