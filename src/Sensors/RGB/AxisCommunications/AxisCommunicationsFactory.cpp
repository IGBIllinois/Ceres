/**
 */

#include "AxisCommunicationsFactory.hpp"

#include "AxisCommunicationsModel_F44.hpp"
#include "AxisCommunicationsView_F44.hpp"
#include "AxisCommunicationsStatusView.hpp"
#include "AxisCommunicationsController.hpp"
#include "AxisCommunicationsPropertyPage_Remote.hpp"
#include "AxisCommunicationsPropertyPage_Local.hpp"

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
    {
        if (protocol == "net")
        {
            pModel->autoEmitImages(false);

            auto* pView = new cAxisCommunicationsStatusView(pModel);

            pView->createWidgets();
            pView->doLayout();

            pView->connectToModel();

            auto* pController = new cAxisCommunicationsController_F44(pModel);

            pController->connectToModel();

            return sSensorWidgets(pModel, pController, pView);
        }

        return sSensorWidgets(pModel);
    }

    auto* dockWidget = new QDockWidget();
    auto* pView = new cAxisCommunicationsView_F44(pModel, dockWidget);

    pView->initialize();

    pView->connectToModel();

    dockWidget->setWindowTitle(pView->windowTitle());
    dockWidget->setWidget(pView);

    QObject::connect(dockWidget, &QDockWidget::dockLocationChanged, pView, &cAxisCommunicationsView::dockLocationChanged);
    QObject::connect(dockWidget, &QDockWidget::topLevelChanged,     pView, &cAxisCommunicationsView::topLevelChanged);


    auto pPage = new cAxisCommunicationsPropertyPage_Local_F44(pModel);

    pPage->connectToModel();

    return sSensorWidgets(pModel, dockWidget, pPage);
}

sSensorWidgets axis_communications::create_sensor(const nlohmann::json& sensorInfo,
    bool no_visualization)
{
    std::string sensor = sensorInfo["sensor"];

    if (sensor == "F44")
        return create_axis_communications_f44_sensor(sensorInfo, no_visualization);

    return sSensorWidgets();
}

void axis_communications::remove_sensor(sSensorWidgets widgets)
{
    auto* pModel = widgets.pModel;
    auto* dockWidget = widgets.pDockableView;
    auto* pView = dockWidget->widget();

    pModel->disconnect();
    pView->disconnect();

    pModel->deleteLater();
    dockWidget->deleteLater();
}


cSensorPropertyPage* axis_communications::create_sensor_property_page(
    const std::string& model, uint32_t version,
    const std::string& remote_ip_address, uint16_t port, const std::string& local_ip_address)
{
    cAxisCommunicationsPropertyPage_Remote* page = new cAxisCommunicationsPropertyPage_Remote_F44();

    page->initialize(remote_ip_address, port, false, local_ip_address);

    return page;
}
