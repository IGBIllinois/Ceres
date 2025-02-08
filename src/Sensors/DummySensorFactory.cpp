/**
 */

#include "DummySensorFactory.hpp"
#include "DummySensorIDs.hpp"

#include "DummySensorModel.hpp"
#include "DummySensorView.hpp"

#include <QWidget>
#include <QString>
#include <QDockWidget>
#include <QMetaType>


sSensorWidgets dummy::create_sensor(bool no_visualization)
{
    // Create the dummy model and view...
    auto* pModel = new cDummyModel();

    if (no_visualization)
    {
        return sSensorWidgets(pModel);
    }

    auto* dockWidget = new QDockWidget();
    auto* pView = new cDummyView(dockWidget);
    
    dockWidget->setWindowTitle(pView->windowTitle());
    dockWidget->setWidget(pView);
    QObject::connect(dockWidget, &QDockWidget::dockLocationChanged, pView, &cDummyView::dockLocationChanged);
    QObject::connect(dockWidget, &QDockWidget::topLevelChanged, pView, &cDummyView::topLevelChanged);

    return sSensorWidgets(pModel, dockWidget);
}

void dummy::remove_sensor(sSensorWidgets widgets)
{
    // Dummy model and view...
    auto* pModel = static_cast<cDummyModel*>(widgets.pModel);
    auto* dockWidget = widgets.pDockableView;
    auto* pView = static_cast<cDummyView*>(dockWidget->widget());

    QObject::disconnect(dockWidget, &QDockWidget::dockLocationChanged, pView, &cDummyView::dockLocationChanged);
    QObject::disconnect(dockWidget, &QDockWidget::topLevelChanged, pView, &cDummyView::topLevelChanged);

    delete pModel;
    delete dockWidget;
}
