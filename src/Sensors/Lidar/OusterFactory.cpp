/**
 */

#include "OusterFactory.hpp"

#include "OusterModel_net.hpp"
#include "OusterView.hpp"
#include "OusterController.hpp"
#include <ouster/ouster_defs.h>

#include <QWidget>
#include <QString>
#include <QDockWidget>
#include <QMetaType>

Q_DECLARE_METATYPE(ouster::sensor_info_2_t);
Q_DECLARE_METATYPE(ouster::time_info_2_t);
Q_DECLARE_METATYPE(ouster::beam_intrinsics_2_t);
Q_DECLARE_METATYPE(ouster::imu_intrinsics_2_t);
Q_DECLARE_METATYPE(ouster::lidar_intrinsics_2_t);
Q_DECLARE_METATYPE(ouster::lidar_data_format_2_t);
Q_DECLARE_METATYPE(ouster::azimuth_range_t);
Q_DECLARE_METATYPE(ouster::imu_data_t);


sSensorWidgets ouster::create_sensor(const nlohmann::json& sensorInfo, bool no_visualization)
{
    // Create the Ouster model and view...
    cOusterModel* pModel = nullptr;

    std::string protocol = sensorInfo["protocol"];

    if (protocol == "net")
    {
        pModel = new cOusterModel_net();
    }

    if (!pModel)
        throw std::runtime_error("OUSTER: Unknown protocol type!");

    if (no_visualization)
    {
        auto* pController = new cOusterController(pModel);
        return sSensorWidgets(pModel, pController);
    }

    auto* dockWidget = new QDockWidget();
    auto* pView = new cOusterView(pModel, dockWidget);
    
    dockWidget->setWindowTitle(pView->windowTitle());
    dockWidget->setWidget(pView);
    QObject::connect(dockWidget, &QDockWidget::dockLocationChanged, pView, &cOusterView::dockLocationChanged);
    QObject::connect(dockWidget, &QDockWidget::topLevelChanged, pView, &cOusterView::topLevelChanged);

    QObject::connect(pModel, &cOusterModel::updateBeamIntrinsics, pView, &cOusterView::beamIntrinsicsChanged);
    QObject::connect(pModel, &cOusterModel::updateImuIntrinsics, pView, &cOusterView::imuIntrinsicsChanged);
    QObject::connect(pModel, &cOusterModel::updateLidarIntrinsics, pView, &cOusterView::lidarIntrinsicsChanged);
    QObject::connect(pModel, &cOusterModel::updateDataFormat, pView, &cOusterView::dataFormatChanged);
    QObject::connect(pModel, &cOusterModel::updateAzimuthWindow, pView, &cOusterView::azimuthWindowChanged);
//    QObject::connect(pModel, &cOusterModel::updateImuData, pView, &cOusterView::imuDataChanged);
//    QObject::connect(pModel, &cOusterModel::updateLidarData, pView, &cOusterView::displayData);
    QObject::connect(pModel, &cOusterModel::updateImuData, pView, &cOusterView::imuDataChanged, Qt::QueuedConnection);
    QObject::connect(pModel, &cOusterModel::updateLidarData, pView, &cOusterView::displayData, Qt::QueuedConnection);

    return sSensorWidgets(pModel, dockWidget);
}

void ouster::remove_sensor(sSensorWidgets widgets)
{
    // Ouster model and view...
    auto* pModel = static_cast<cOusterModel*>(widgets.pModel);
    auto* dockWidget = widgets.pDockableView;
    auto* pView = static_cast<cOusterView*>(dockWidget->widget());

    QObject::disconnect(pModel, &cOusterModel::updateBeamIntrinsics, pView, &cOusterView::beamIntrinsicsChanged);
    QObject::disconnect(pModel, &cOusterModel::updateImuIntrinsics, pView, &cOusterView::imuIntrinsicsChanged);
    QObject::disconnect(pModel, &cOusterModel::updateLidarIntrinsics, pView, &cOusterView::lidarIntrinsicsChanged);
    QObject::disconnect(pModel, &cOusterModel::updateDataFormat, pView, &cOusterView::dataFormatChanged);
    QObject::disconnect(pModel, &cOusterModel::updateAzimuthWindow, pView, &cOusterView::azimuthWindowChanged);
    QObject::disconnect(pModel, &cOusterModel::updateImuData, pView, &cOusterView::imuDataChanged);
    QObject::disconnect(pModel, &cOusterModel::updateLidarData, pView, &cOusterView::displayData);

    delete pModel;
    delete dockWidget;
}

cSensorPropertyPageRemote* ouster::create_sensor_property_page(uint32_t version)
{
    return nullptr;
}

