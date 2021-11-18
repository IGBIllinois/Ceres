
#include "WeatherDataModel_Http_Wind.hpp"


cWeatherDataModel_Http_Wind::cWeatherDataModel_Http_Wind()
{
}

bool cWeatherDataModel_Http_Wind::configure(const nlohmann::json& jsonCfg)
{
	auto data_types = jsonCfg[protocol()];

//	QNetworkAccessManager* pHttpManager;
//	QUrl mUrl;

	return true;
}

void cWeatherDataModel_Http_Wind::writeDataHeader(cBlockDataFile& file)
{

}
