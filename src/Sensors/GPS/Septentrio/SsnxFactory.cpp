
#include "SsnxFactory.hpp"

#include "SsnxModel_net.hpp"
#include "SsnxModel_direct.hpp"
#include "SsnxView.hpp"
#include "SsnxStatusView.hpp"
#include "SsnxController.hpp"
#include "SsnxPropertyPage_Remote.hpp"

#include <stdexcept>

#include <QWidget>
#include <QString>
#include <QDockWidget>


sSensorWidgets ssnx::create_sensor(const nlohmann::json& sensorInfo, bool no_visualization)
{
    // Create the SSNX model and view...
    cSsnxModel* pModel = nullptr;

    std::string protocol = sensorInfo["protocol"];

    if (protocol == "direct")
        pModel = new cSsnxModel_direct();
    else if (protocol == "net")
        pModel = new cSsnxModel_net();

    if (!pModel)
        throw std::runtime_error("SSNX: Unknown protocol type!");

    if (no_visualization)
    {
        auto widgets = sSensorWidgets(pModel);
        if (protocol == "direct")
        {
            auto* pView = new cSsnxStatusView(pModel);
            pView->createWidgets();
            pView->doLayout();

            QObject::connect(pModel, &cSsnxModel::sensorStatusChanging, pView, &cSsnxStatusView::onSensorStatusChange);
            QObject::connect(pModel, &cSsnxModel::pvtCartesianStateChanged, pView, &cSsnxStatusView::onPvtCartesianStateChange);
            QObject::connect(pModel, &cSsnxModel::pvtGeodeticStateChanged, pView, &cSsnxStatusView::onPvtGeodeticStateChange);
            QObject::connect(pModel, &cSsnxModel::posCovGeodeticStateChanged, pView, &cSsnxStatusView::onPosCovGeodeticStateChange);
            QObject::connect(pModel, &cSsnxModel::velCovGeodeticStateChanged, pView, &cSsnxStatusView::onVelCovGeodeticStateChange);
            QObject::connect(pModel, &cSsnxModel::posProjectedStateChanged, pView, &cSsnxStatusView::onPosProjectedStateChange);
            QObject::connect(pModel, &cSsnxModel::receiverTimeStateChanged, pView, &cSsnxStatusView::onReceiverTimeStateChange);
            QObject::connect(pModel, &cSsnxModel::rtcmDatumStateChanged, pView, &cSsnxStatusView::onRtcmDatumStateChange);
            QObject::connect(pModel, &cSsnxModel::solutionTypeChanged, pView, &cSsnxStatusView::onSolutionTypeChange);
            QObject::connect(pModel, &cSsnxModel::positionChanged, pView, &cSsnxStatusView::onPositionChange);
            QObject::connect(pModel, &cSsnxModel::updateGeodeticPVT, pView, &cSsnxStatusView::onGeodeticPVT_Change);
            QObject::connect(pModel, &cSsnxModel::updateUTC, pView, &cSsnxStatusView::onUTC_Change);

            widgets.pRemoteStatusView = pView;
        }

        auto* pController = new cSsnxController(pModel);

        QObject::connect(pModel, &cSsnxModel::referenceComplete, pController, &cGpsController::onReferenceComplete);

        widgets.pController = pController;

        return widgets;
    }

    auto* dockWidget = new QDockWidget();
    auto* pView = new cSsnxView(dockWidget);

    dockWidget->setWindowTitle(pView->windowTitle());
    dockWidget->setWidget(pView);
    QObject::connect(dockWidget, &QDockWidget::dockLocationChanged, pView, &cGpsView::dockLocationChanged);
    QObject::connect(dockWidget, &QDockWidget::topLevelChanged, pView, &cGpsView::topLevelChanged);

    QObject::connect(pModel, &cSsnxModel::updateGeodeticPVT, pView, &cSsnxView::updatePVT);
    QObject::connect(pModel, &cSsnxModel::updateUTC, pView, &cSsnxView::updateUTC);

    return sSensorWidgets(pModel, dockWidget);
}


void ssnx::remove_sensor(sSensorWidgets widgets)
{
    // SSNX model and view...
    auto* pModel = static_cast<cSsnxModel*>(widgets.pModel);
    auto* dockWidget = widgets.pDockableView;
    auto* pView = static_cast<cSsnxView*>(dockWidget->widget());

    QObject::disconnect(pModel, &cSsnxModel::updateGeodeticPVT, pView, &cSsnxView::updatePVT);
    QObject::disconnect(pModel, &cSsnxModel::updateUTC, pView, &cSsnxView::updateUTC);

    QObject::disconnect(dockWidget, &QDockWidget::dockLocationChanged, pView, &cGpsView::dockLocationChanged);
    QObject::disconnect(dockWidget, &QDockWidget::topLevelChanged, pView, &cGpsView::topLevelChanged);

    delete pModel;
    delete dockWidget;
}


cSensorPropertyPage* ssnx::create_sensor_property_page(const std::string& model, uint32_t version,
    const std::string& remote_ip_address, uint16_t port, const std::string& local_ip_address)
{
    if (model == ssnx_id)
    {
        auto page = new cSsnxPropertyPage_Remote();
        page->initialize(remote_ip_address, port, false, local_ip_address);
        return page;
    }

    return nullptr;
}
