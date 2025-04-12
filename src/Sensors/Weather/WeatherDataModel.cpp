
#include "WeatherDataModel.hpp"
#include "WeatherDataIDs.hpp"


namespace
{
	static uint8_t weather_id = 0;
}


cWeatherDataModel::cWeatherDataModel(QObject* parent)
:
	cSensorModel("Weather", parent), mDeviceID(++weather_id)
{
}

uint8_t cWeatherDataModel::device_id() const
{
	return mDeviceID;
}

const char* cWeatherDataModel::descriptor() const
{
	return weather_data_id;
}
