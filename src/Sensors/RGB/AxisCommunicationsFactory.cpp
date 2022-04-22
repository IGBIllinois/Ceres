/**
 */

#include "AxisCommunicationsFactory.hpp"

#include "AxisCommunicationsModel_F44.hpp"
#include "AxisCommunicationsView_F44.hpp"

#include <QWidget>
#include <QString>
#include <QDockWidget>
#include <QMetaType>

//Q_DECLARE_METATYPE(ouster::sensor_info_t);
//Q_DECLARE_METATYPE(ouster::time_info_t);
//Q_DECLARE_METATYPE(ouster::beam_intrinsics_t);
//Q_DECLARE_METATYPE(ouster::imu_intrinsics_t);
//Q_DECLARE_METATYPE(ouster::lidar_intrinsics_t);
//Q_DECLARE_METATYPE(ouster::lidar_data_format_t);
//Q_DECLARE_METATYPE(ouster::azimuth_range_t);
//Q_DECLARE_METATYPE(ouster::imu_data_t);


sSensorWidgets create_axis_communications_f44_sensor()
{
    // Create the Ouster model and view...
    auto* pModel = new cAxisCommunicationsModel_F44();
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

/*
    QObject::connect(pModel, &cOusterModel::updateLidarIntrinsics, pView, &cOusterView::lidarIntrinsicsChanged);
    QObject::connect(pModel, &cOusterModel::updateDataFormat, pView, &cOusterView::dataFormatChanged);
    QObject::connect(pModel, &cOusterModel::updateAzimuthWindow, pView, &cOusterView::azimuthWindowChanged);
    //    QObject::connect(pModel, &cOusterModel::updateEncoderCount, pView, &cOusterView::encoderCountChanged);
    QObject::connect(pModel, &cOusterModel::updateImuData, pView, &cOusterView::imuDataChanged);
    QObject::connect(pModel, &cOusterModel::updateLidarData, pView, &cOusterView::displayData);
*/

    return sSensorWidgets(pModel, dockWidget);
}

sSensorWidgets create_axis_communications_sensor(const nlohmann::json& sensorInfo)
{
    std::string sensor = sensorInfo["sensor"];

    if (sensor == "F44")
        return create_axis_communications_f44_sensor();

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
