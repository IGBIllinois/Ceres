/**
 */

#include "OusterFactory.hpp"

#include "OusterModel.hpp"
#include "OusterView.hpp"
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


sSensorWidgets create_ouster_sensor(bool no_visualization)
{
    // Create the Ouster model and view...
    auto* pModel = new cOusterModel();

    if (no_visualization)
        return sSensorWidgets(pModel, nullptr);

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
    QObject::connect(pModel, &cOusterModel::updateImuData, pView, &cOusterView::imuDataChanged);
    QObject::connect(pModel, &cOusterModel::updateLidarData, pView, &cOusterView::displayData);

    return sSensorWidgets(pModel, dockWidget);
}

void remove_ouster_sensor(sSensorWidgets widgets)
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
