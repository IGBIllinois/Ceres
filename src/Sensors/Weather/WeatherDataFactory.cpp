
#include "WeatherDataFactory.hpp"

#include "WeatherDataModel_Http_Wind.hpp"
#include "WindSpeedAndDirection.hpp"

#include <QWidget>
#include <QString>
#include <QDockWidget>


sSensorWidgets create_http_based_sensor(const std::string& data_type)
{
    sSensorWidgets widgets;
    if (data_type == cWeatherDataModel_Http_Wind::data_type())
    {
        widgets.pModel = new cWeatherDataModel_Http_Wind();
        widgets.pStatusBar = new cWindSpeedAndDirection();
    }

    return widgets;
}

sSensorWidgets create_weather_data_sensor(const nlohmann::json& sensorInfo)
{
    std::string protocol = sensorInfo["protocol"];
    std::string data_type = sensorInfo["data"];

    if (protocol == "http")
        return create_http_based_sensor(data_type);

/*
    dockWidget->setWindowTitle(pView->windowTitle());
    dockWidget->setWidget(pView);

    QObject::connect(pModel, &cOusterModel::updateView, pView, &cOusterView::displayData);
*/

    return sSensorWidgets();
}

void remove_weather_data_sensor(sSensorWidgets widgets)
{
}
