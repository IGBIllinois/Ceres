/**
 */

#include "OusterFactory.hpp"

#include "OusterModel_net.hpp"
#include "OusterView.hpp"
#include "OusterStatusView.hpp"
#include "OusterController.hpp"
#include "OusterPropertyPage.hpp"
#include "OusterPropertyPage_Local.hpp"
#include "OusterPropertyPage_Remote.hpp"
#include <ouster_connect/ouster_defs.h>

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
        auto* pView = new cOusterStatusView(pModel);

        pView->createWidgets();
        pView->doLayout();


        auto* pController = new cOusterController(pModel);

        pController->connectToModel();

        return sSensorWidgets(pModel, pController, pView);
    }

    auto* dockWidget = new QDockWidget();
    auto* pView = new cOusterView(pModel, dockWidget);

    pView->connectToModel();

    dockWidget->setWindowTitle(pView->windowTitle());
    dockWidget->setWidget(pView);

    QObject::connect(dockWidget, &QDockWidget::dockLocationChanged, pView, &cOusterView::dockLocationChanged);
    QObject::connect(dockWidget, &QDockWidget::topLevelChanged,     pView, &cOusterView::topLevelChanged);

    auto* pPage = new cOusterPropertyPage_Local(pModel);

    pPage->connectToModel();

    return sSensorWidgets(pModel, dockWidget, pPage);
}

void ouster::remove_sensor(sSensorWidgets widgets)
{
    auto* pModel = widgets.pModel;

    auto* dockWidget = widgets.pDockableView;
    auto* pView = dockWidget->widget();

    pView->disconnect();
    pModel->disconnect();

    pModel->deleteLater();
    dockWidget->deleteLater();
}

cSensorPropertyPage* ouster::create_sensor_property_page(uint32_t version,
    const std::string& remote_ip_address, uint16_t port, const std::string& local_ip_address)
{
    cOusterPropertyPage_Remote* page = new cOusterPropertyPage_Remote();
    page->initialize(remote_ip_address, port, false, local_ip_address);

    return page;
}

cSensorPropertyPage* ouster::create_sensor_property_page(const std::string& instance, uint32_t version,
    const std::string& remote_ip_address, uint16_t port, const std::string& local_ip_address)
{
    cOusterPropertyPage_Remote* page = new cOusterPropertyPage_Remote();
    page->initialize(remote_ip_address, port, false, local_ip_address);

    return page;
}

