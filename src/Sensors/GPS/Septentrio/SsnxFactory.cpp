
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

            QObject::connect(pModel, &cSsnxModel::pvtCartesianDataValid,   pView, &cSsnxStatusView::onPvtCartesianStateChange);
            QObject::connect(pModel, &cSsnxModel::pvtGeodeticDataValid,    pView, &cSsnxStatusView::onPvtGeodeticStateChange);
            QObject::connect(pModel, &cSsnxModel::posCovGeodeticDataValid, pView, &cSsnxStatusView::onPosCovGeodeticStateChange);
            QObject::connect(pModel, &cSsnxModel::velCovGeodeticDataValid, pView, &cSsnxStatusView::onVelCovGeodeticStateChange);
            QObject::connect(pModel, &cSsnxModel::posProjectedDataValid,   pView, &cSsnxStatusView::onPosProjectedStateChange);
            QObject::connect(pModel, &cSsnxModel::receiverTimeDataValid,   pView, &cSsnxStatusView::onReceiverTimeStateChange);
            QObject::connect(pModel, &cSsnxModel::rtcmDatumDataValid,      pView, &cSsnxStatusView::onRtcmDatumStateChange);
            QObject::connect(pModel, &cSsnxModel::receiverStatusDataValid, pView, &cSsnxStatusView::onReceiverStatusStateChange);
            QObject::connect(pModel, &cSsnxModel::wifiClientDataValid,     pView, &cSsnxStatusView::onWifiClientStateChange);
            QObject::connect(pModel, &cSsnxModel::ntripClientDataValid,    pView, &cSsnxStatusView::onNtripStateChange);

            QObject::connect(pModel, &cSsnxModel::receiverStatusChanged,       pView, &cSsnxStatusView::onReceiverStateChange);
            QObject::connect(pModel, &cSsnxModel::ntripClientStatusChanged,    pView, &cSsnxStatusView::onNtripClientChange);
            QObject::connect(pModel, &cSsnxModel::wifiClientConnectionChanged, pView, &cSsnxStatusView::onWifiConnectionChange);

            QObject::connect(pModel, &cSsnxModel::solutionTypeChanged,      pView, &cSsnxStatusView::onSolutionTypeChange);
            QObject::connect(pModel, &cSsnxModel::positionChanged,          pView, &cSsnxStatusView::onPositionChange);
            QObject::connect(pModel, &cSsnxModel::updateGeodeticPVT,        pView, &cSsnxStatusView::onGeodeticPVT_Change);
            QObject::connect(pModel, &cSsnxModel::updateUTC,                pView, &cSsnxStatusView::onUTC_Change);
            QObject::connect(pModel, &cSsnxModel::referencePositionChanged, pView, &cSsnxStatusView::onReferencePositionChange);

            if (protocol == "direct")
                QObject::connect(pView, &cSsnxStatusView::tryGpsReconnection, static_cast<cSsnxModel_direct*>(pModel), &cSsnxModel_direct::reconnectToGps);

            widgets.pRemoteStatusView = pView;
        }

        auto* pController = new cSsnxController();

        // Base GPS model <-> controller connections
        QObject::connect(pModel, &cGpsModel::referenceComplete,          pController, &cGpsController::onReferenceComplete);
        QObject::connect(pModel, &cGpsModel::referenceStateChanged,      pController, &cGpsController::referenceStateUpdated);
        QObject::connect(pModel, &cGpsModel::referenceParametersChanged, pController, &cGpsController::referenceParametersUpdated);
        QObject::connect(pModel, &cGpsModel::referenceDataChanged,       pController, &cGpsController::referenceDataUpdated);
        QObject::connect(pModel, &cGpsModel::referencePositionChanged,   pController, &cGpsController::referencePositionUpdated);

        QObject::connect(pController, &cGpsController::queryReferenceState,       pModel, &cGpsModel::referenceStateQueried);
        QObject::connect(pController, &cGpsController::queryReferenceParameters,  pModel, &cGpsModel::referenceParametersQueried);
        QObject::connect(pController, &cGpsController::queryReferenceData,        pModel, &cGpsModel::referenceDataQueried);
        QObject::connect(pController, &cGpsController::updateReferenceParameters, pModel, &cGpsModel::updateReferenceParameters);
        QObject::connect(pController, &cGpsController::startReferenceComputation, pModel, &cGpsModel::startReferenceComputation);
        QObject::connect(pController, &cGpsController::abortReferenceCompute,     pModel, &cGpsModel::abortReferenceCompute);

        widgets.pController = pController;

        return widgets;
    }

    auto* dockWidget = new QDockWidget();
    auto* pView = new cSsnxView(dockWidget);

    // Connect the model signals to the view's slots
    QObject::connect(pModel, &cSsnxModel::updateGeodeticPVT, pView, &cSsnxView::updatePVT);
    QObject::connect(pModel, &cSsnxModel::updateUTC,         pView, &cSsnxView::updateUTC);


    dockWidget->setWindowTitle(pView->windowTitle());
    dockWidget->setWidget(pView);

    QObject::connect(dockWidget, &QDockWidget::dockLocationChanged, pView, &cGpsView::dockLocationChanged);
    QObject::connect(dockWidget, &QDockWidget::topLevelChanged, pView, &cGpsView::topLevelChanged);

    return sSensorWidgets(pModel, dockWidget);
}


void ssnx::remove_sensor(sSensorWidgets widgets)
{
    auto* pModel = widgets.pModel;
    auto* dockWidget = widgets.pDockableView;
    auto* pView = dockWidget->widget();

    pModel->disconnect();
    pView->disconnect();

    pModel->deleteLater();
    dockWidget->deleteLater();
}


cSensorPropertyPage* ssnx::create_sensor_property_page(const std::string& model, uint32_t version,
    const std::string& remote_ip_address, uint16_t port, const std::string& local_ip_address)
{
    auto page = new cSsnxPropertyPage_Remote();
    page->initialize(remote_ip_address, port, false, local_ip_address);
    return page;
}


cSensorPropertyPage* ssnx::create_sensor_property_page(const std::string& instance, const std::string& model, uint32_t version,
    const std::string& remote_ip_address, uint16_t port, const std::string& local_ip_address)
{
    auto page = new cSsnxPropertyPage_Remote(instance);
    page->initialize(remote_ip_address, port, false, local_ip_address);
    return page;
}
