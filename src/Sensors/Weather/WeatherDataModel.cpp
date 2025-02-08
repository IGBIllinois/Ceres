
#include "WeatherDataModel.hpp"
#include "WeatherDataIDs.hpp"


cWeatherDataModel::cWeatherDataModel(QObject* parent)
:
	cSensorModel("Weather", parent)
{
}

const char* cWeatherDataModel::descriptor() const
{
	return weather_data_id;
}
