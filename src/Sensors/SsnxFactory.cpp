
#include "SsnxFactory.hpp"

#include "SsnxModel.hpp"
#include "SsnxView.hpp"


#include <QWidget>
#include <QString>
#include <QDockWidget>


sSensorWidgets create_ssnx_sensor(QWidget* parent)
{
    // Create the SSNX model and view...
    auto* pModel = new cSsnxModel(parent);
    auto* dockWidget = new QDockWidget(parent);
    auto* pView = new cSsnxView(dockWidget);

    dockWidget->setWindowTitle(pView->windowTitle());
    dockWidget->setWidget(pView);
    dockWidget->connect(dockWidget, &QDockWidget::dockLocationChanged, pView, &cGpsView::dockLocationChanged);
    dockWidget->connect(dockWidget, &QDockWidget::topLevelChanged, pView, &cGpsView::topLevelChanged);

    QObject::connect(pModel, &cSsnxModel::updatePVT, pView, &cSsnxView::updatePVT);
    QObject::connect(pModel, &cSsnxModel::updateUTC, pView, &cSsnxView::updateUTC);

    return sSensorWidgets(pModel, dockWidget);
}

