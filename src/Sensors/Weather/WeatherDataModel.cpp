
#include "WeatherDataModel.hpp"
#include "WeatherDataFactory.hpp"


cWeatherDataModel::cWeatherDataModel(QObject* parent)
:
	cSensorModel("Weather", parent)
{
}

char* cWeatherDataModel::descriptor() const
{
	return weather_data_id;
}
