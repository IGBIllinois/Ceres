
#include "SensorFactory.hpp"

#include "GpsModelSsnx.hpp"
#include "GpsView.hpp"

#include "LidarModelOuster.hpp"
#include "OusterView.hpp"

#include <QDockWidget>
#include <QString>


std::pair<cSensorModel*, QDockWidget*> createSsnxSensor(QWidget* parent)
{
    // Create the SSNX model and view...
    auto* gpsModel = new cGpsModelSsnx(parent);
    auto* dockWidget = new QDockWidget(gpsModel->getViewTitle(), parent);
    auto* gpsView = new cGpsView(dockWidget);

    dockWidget->setWidget(gpsView);
    dockWidget->connect(dockWidget, &QDockWidget::dockLocationChanged, gpsView, &cGpsView::dockLocationChanged);
    dockWidget->connect(dockWidget, &QDockWidget::topLevelChanged, gpsView, &cGpsView::topLevelChanged);

    gpsModel->connect(gpsModel, &cGpsModelSsnx::updatePVT, gpsView, &cGpsView::updatePVT);
    gpsModel->connect(gpsModel, &cGpsModelSsnx::updateUTC, gpsView, &cGpsView::updateUTC);

    return std::make_pair( gpsModel, dockWidget);
}

std::pair<cSensorModel*, QDockWidget*> createOusterSensor(QWidget* parent)
{
    // Create the Ouster model and view...
    auto* lidarModel = new cLidarModelOuster(parent);
    auto* dockWidget = new QDockWidget(lidarModel->getViewTitle(), parent);
    auto* lidarView = new cOusterView(dockWidget);

    dockWidget->setWidget(lidarView);
 //   dockWidget->connect(dockWidget, &QDockWidget::dockLocationChanged, gpsView, &cGpsView::dockLocationChanged);
 //   dockWidget->connect(dockWidget, &QDockWidget::topLevelChanged, gpsView, &cGpsView::topLevelChanged);

//    gpsModel->connect(gpsModel, &cGpsModelSsnx::updatePVT, gpsView, &cGpsView::updatePVT);
//    gpsModel->connect(gpsModel, &cGpsModelSsnx::updateUTC, gpsView, &cGpsView::updateUTC);

    return std::make_pair(lidarModel, dockWidget);
}


std::pair<cSensorModel*, QDockWidget*> create_sensor(const std::string& name, QWidget* parent)
{
    if (name.compare("ssnx") == 0)
    {
        return createSsnxSensor(parent);
    }

    if (name.compare("ouster") == 0)
    {
        return createOusterSensor(parent);
    }

    return std::make_pair(nullptr, nullptr);
}