
#include "SsnxFactory.hpp"

#include "SsnxModel_net.hpp"
#include "SsnxModel_direct.hpp"
#include "SsnxView.hpp"
#include "SsnxStatusView.hpp"

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

            widgets.pRemoteStatusView = pView;
        }
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
