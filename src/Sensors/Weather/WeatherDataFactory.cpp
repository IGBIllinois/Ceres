
#include "WeatherDataFactory.hpp"

#include "WeatherDataModel_Http_Wind.hpp"
#include "WindSpeedAndDirection.hpp"

#include <QWidget>
#include <QString>
#include <QDockWidget>


sSensorWidgets create_http_based_sensor(const std::string& data_type, bool no_visualization)
{
    sSensorWidgets widgets;
    if (data_type == cWeatherDataModel_Http_Wind::data_type())
    {
        auto* pModel = new cWeatherDataModel_Http_Wind();

        if (no_visualization)
        {
            widgets.pModel = pModel;
        }
        else
        {
            auto* pStatusBar = new cWindSpeedAndDirection();

            QObject::connect(pModel, &cWeatherDataModel_Http_Wind::windDataChanged, pStatusBar, &cWindSpeedAndDirection::updateWindData);

            widgets.pModel = pModel;
            widgets.pStatusBar = pStatusBar;
        }
    }

    return widgets;
}

sSensorWidgets create_weather_data_sensor(const nlohmann::json& sensorInfo,
    bool no_visualization)
{
    std::string protocol = sensorInfo["protocol"];
    std::string data_type = sensorInfo["data"];

    if (protocol == "http")
        return create_http_based_sensor(data_type, no_visualization);

    return sSensorWidgets();
}

void remove_weather_data_sensor(sSensorWidgets widgets)
{
}
