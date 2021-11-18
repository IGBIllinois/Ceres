
#include "WeatherDataModel.hpp"
#include "WeatherDataFactory.hpp"


cWeatherDataModel::cWeatherDataModel()
{
}

char* cWeatherDataModel::descriptor() const
{
	return weather_data_id;
}
