
#include "OusterFactory.hpp"

#include "OusterModel.hpp"
#include "OusterView.hpp"

#include <QWidget>
#include <QString>
#include <QDockWidget>

sSensorWidgets create_ouster_sensor()
{
    // Create the Ouster model and view...
    auto* pModel = new cOusterModel();
    auto* dockWidget = new QDockWidget();
    auto* pView = new cOusterView(pModel, dockWidget);
    
    dockWidget->setWindowTitle(pView->windowTitle());
    dockWidget->setWidget(pView);
    QObject::connect(dockWidget, &QDockWidget::dockLocationChanged, pView, &cOusterView::dockLocationChanged);
    QObject::connect(dockWidget, &QDockWidget::topLevelChanged, pView, &cOusterView::topLevelChanged);

    QObject::connect(pModel, &cOusterModel::updateView, pView, &cOusterView::displayData);

    return sSensorWidgets(pModel, dockWidget);
}

void remove_ouster_sensor(sSensorWidgets widgets)
{
    // Ouster model and view...
    auto* pModel = static_cast<cOusterModel*>(widgets.pModel);
    auto* dockWidget = widgets.pDockableView;
    auto* pView = static_cast<cOusterView*>(dockWidget->widget());

    QObject::disconnect(pModel, &cOusterModel::updateView, pView, &cOusterView::displayData);

    delete pModel;
    delete dockWidget;
}
