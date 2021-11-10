
#include "OusterFactory.hpp"

#include "OusterModel.hpp"
#include "OusterView.hpp"

#include <QWidget>
#include <QString>
#include <QDockWidget>

sSensorWidgets
create_ouster_sensor(QWidget* parent)
{
    // Create the Ouster model and view...
    auto* pModel = new cOusterModel();
    auto* dockWidget = new QDockWidget(parent);
    auto* pView = new cOusterView(pModel, dockWidget);
    
    dockWidget->setWindowTitle(pView->windowTitle());
    dockWidget->setWidget(pView);

    QObject::connect(pModel, &cOusterModel::updateView, pView, &cOusterView::displayData);

    return sSensorWidgets(pModel, dockWidget);
}

