/**
 */

#include "HokuyoFactory.hpp"

#include "HokuyoModel_UST10LX.hpp"
#include "HokuyoView.hpp"
//#include "OusterController.hpp"
//#include "OusterPropertyPage.hpp"
//#include "OusterPropertyPage_Local.hpp"
//#include "OusterPropertyPage_Remote.hpp"
//#include <ouster/ouster_defs.h>

#include <QWidget>
#include <QString>
#include <QDockWidget>
#include <QMetaType>

//Q_DECLARE_METATYPE(ouster::sensor_info_2_t);
//Q_DECLARE_METATYPE(ouster::time_info_2_t);
//Q_DECLARE_METATYPE(ouster::beam_intrinsics_2_t);
//Q_DECLARE_METATYPE(ouster::imu_intrinsics_2_t);
//Q_DECLARE_METATYPE(ouster::lidar_intrinsics_2_t);
//Q_DECLARE_METATYPE(ouster::lidar_data_format_2_t);
//Q_DECLARE_METATYPE(ouster::azimuth_range_t);
//Q_DECLARE_METATYPE(ouster::imu_data_t);


sSensorWidgets create_sensor_ust_10lx(const nlohmann::json& sensorInfo, bool no_visualization)
{
    // Create the Hokuyo model and view...
    cHokuyoModel* pModel = nullptr;

    std::string protocol = sensorInfo["protocol"];

    if (protocol == "net")
    {
        pModel = new cHokuyoModel_UST10LX();
    }

    if (!pModel)
        throw std::runtime_error("Hokuyo: Unknown protocol type!");

    if (no_visualization)
    {
/*
       auto* pController = new cHokuyoController(pModel);

       QObject::connect(pController, &cHokuyoController::requestNewLidarMode,
                pModel, &cHokuyoModel::changeLidarMode);
*/

        return sSensorWidgets(pModel);
    }

    auto* dockWidget = new QDockWidget();
    auto* pView = new cHokuyoView(pModel, dockWidget);

    dockWidget->setWindowTitle(pView->windowTitle());
    dockWidget->setWidget(pView);
    QObject::connect(dockWidget, &QDockWidget::dockLocationChanged, pView, &cHokuyoView::dockLocationChanged);
    QObject::connect(dockWidget, &QDockWidget::topLevelChanged, pView, &cHokuyoView::topLevelChanged);

//    QObject::connect(pModel, &cHokuyoModel::updateBeamIntrinsics, pView, &cHokuyoView::beamIntrinsicsChanged);
    QObject::connect(pModel, &cHokuyoModel::updateLidarData, pView, &cHokuyoView::displayData, Qt::QueuedConnection);

    return sSensorWidgets(pModel, dockWidget);
}


sSensorWidgets hokuyo::create_sensor(const nlohmann::json& sensorInfo, bool no_visualization)
{
    std::string sensor = sensorInfo["sensor"];

    if (sensor == "UST-10LX")
        return create_sensor_ust_10lx(sensorInfo, no_visualization);

    return sSensorWidgets();
}

void hokuyo::remove_sensor(sSensorWidgets widgets)
{
/*
    // Hokuyo model and view...
    auto* pModel = static_cast<cHokuyoModel*>(widgets.pModel);
    auto* dockWidget = widgets.pDockableView;
    auto* pView = static_cast<cHokuyoView*>(dockWidget->widget());

    QObject::disconnect(pModel, &cHokuyoModel::updateBeamIntrinsics, pView, &cHokuyoView::beamIntrinsicsChanged);
    QObject::disconnect(pModel, &cHokuyoModel::updateImuIntrinsics, pView, &cHokuyoView::imuIntrinsicsChanged);
    QObject::disconnect(pModel, &cHokuyoModel::updateLidarIntrinsics, pView, &cHokuyoView::lidarIntrinsicsChanged);
    QObject::disconnect(pModel, &cHokuyoModel::updateDataFormat, pView, &cHokuyoView::dataFormatChanged);
    QObject::disconnect(pModel, &cHokuyoModel::updateAzimuthWindow, pView, &cHokuyoView::azimuthWindowChanged);
    QObject::disconnect(pModel, &cHokuyoModel::updateImuData, pView, &cHokuyoView::imuDataChanged);
    QObject::disconnect(pModel, &cHokuyoModel::updateLidarData, pView, &cHokuyoView::displayData);

    delete pModel;
    delete dockWidget;
*/
}

cSensorPropertyPage* hokuyo::create_sensor_property_page(uint32_t version,
    const std::string& remote_ip_address, uint16_t port, const std::string& local_ip_address)
{
/*
    cOusterPropertyPage_Remote* page = new cOusterPropertyPage_Remote();
    page->initialize(remote_ip_address, port, false, local_ip_address);

    return page;
*/

    return nullptr;
}

