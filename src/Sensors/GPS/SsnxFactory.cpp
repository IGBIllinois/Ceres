
#include "SsnxFactory.hpp"

#include "SsnxModel.hpp"
#include "SsnxView.hpp"


#include <QWidget>
#include <QString>
#include <QDockWidget>


sSensorWidgets create_ssnx_sensor()
{
    // Create the SSNX model and view...
    auto* pModel = new cSsnxModel();
    auto* dockWidget = new QDockWidget();
    auto* pView = new cSsnxView(dockWidget);

    dockWidget->setWindowTitle(pView->windowTitle());
    dockWidget->setWidget(pView);
    QObject::connect(dockWidget, &QDockWidget::dockLocationChanged, pView, &cGpsView::dockLocationChanged);
    QObject::connect(dockWidget, &QDockWidget::topLevelChanged, pView, &cGpsView::topLevelChanged);

    QObject::connect(pModel, &cSsnxModel::updatePVT, pView, &cSsnxView::updatePVT);
    QObject::connect(pModel, &cSsnxModel::updateUTC, pView, &cSsnxView::updateUTC);

    return sSensorWidgets(pModel, dockWidget);
}


void remove_ssnx_sensor(sSensorWidgets widgets)
{
    // SSNX model and view...
    auto* pModel = static_cast<cSsnxModel*>(widgets.pModel);
    auto* dockWidget = widgets.pDockableView;
    auto* pView = static_cast<cSsnxView*>(dockWidget->widget());

    QObject::disconnect(pModel, &cSsnxModel::updatePVT, pView, &cSsnxView::updatePVT);
    QObject::disconnect(pModel, &cSsnxModel::updateUTC, pView, &cSsnxView::updateUTC);

    QObject::disconnect(dockWidget, &QDockWidget::dockLocationChanged, pView, &cGpsView::dockLocationChanged);
    QObject::disconnect(dockWidget, &QDockWidget::topLevelChanged, pView, &cGpsView::topLevelChanged);

    delete pModel;
    delete dockWidget;
}
