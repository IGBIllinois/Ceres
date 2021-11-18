
#include "WeatherDataModel_Http_Wind.hpp"
#include "../../Utilities/Constants.hpp"

#include <QEventLoop>
#include <QMessageBox>


cWeatherDataModel_Http_Wind::cWeatherDataModel_Http_Wind()
	:
		mData()
{
	mMaxWindSpeed_mps = 100;
	mWindSpeed_mps = 0.0;
	mWindDirection_deg = 0.0;
	mDataValid = false;
}

bool cWeatherDataModel_Http_Wind::configure(const nlohmann::json& jsonCfg)
{
	bool valid_cfg = false;

	auto sensors = jsonCfg[protocol()];
	for (auto sensor : sensors)
	{
		if (sensor["data"] == data_type())
		{
			valid_cfg = cWeatherDataModel_Http::configure(sensor);
			
			mMaxWindSpeed_mps = sensor["max wind speed (mph)"].get<double>() * nConstants::MPH_TO_MPS;
			break;
		}
	}

	emit statusMessage("Testing the connection to the weather station...");


	QNetworkAccessManager manager; // = new QNetworkAccessManager();
	QNetworkReply* reply = manager.get(QNetworkRequest(url()));

	reply->waitForReadyRead(5000);

	QEventLoop loop;
	connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
	loop.exec();

	reply->deleteLater();

	QString replyText;

	if (reply->error() == QNetworkReply::NoError)
	{
		// Get the http status code
		int v = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
		if (v >= 200 && v < 300) // Success
		{
			// Here we got the final reply 
			replyText = reply->readAll();
		}
	}
	else
	{
		// Error
		QMessageBox msg(QMessageBox::Critical, "Wind Data Communication Error", reply->errorString());
		msg.exec();

		return false;
	}

	try
	{
		nlohmann::json jsonDoc = nlohmann::json::parse(replyText.toStdString());
		auto& jsonHead = jsonDoc["head"];

		/*
		 * We are checking the meta data field information to make sure it matches what we expect:
		 * 
		 * 	"fields":  
		 *	[
		 *		{
		 *			"name":  "WS_ms",
		 *			"type":  "xsd:float",
		 *			"units":  "m/s",
		 *			"process":  "Smp",
		 * 			"settable":  false
		 *		},
		 *		{
		 *			"name":  "WindDir",
		 *			"type":  "xsd:float",
		 *			"units":  "degrees",
		 *			"process":  "Smp",
		 *			"settable":  false
		 *		},
		 *		{
		 *			"name":  "Invalid_Wind",
		 *			"type":  "xsd:float",
		 *			"process":  "Smp",
		 *			"settable":  false
		 *		}
		 *	]
		 */
		auto& jsonFields = jsonHead["fields"];
		auto& wind_speed = jsonFields[0];
		if ((wind_speed["name"] != "WS_ms") || (wind_speed["type"] != "xsd:float") || (wind_speed["units"] != "m/s"))
			throw std::invalid_argument("Invalid JSON wind speed data format.");

		auto& wind_dir = jsonFields[1];
		if ((wind_dir["name"] != "WindDir") || (wind_dir["type"] != "xsd:float") || (wind_dir["units"] != "degrees"))
			throw std::invalid_argument("Invalid JSON wind direction data format.");

		auto& wind_valid = jsonFields[2];
		if ((wind_valid["name"] != "Invalid_Wind") || (wind_valid["type"] != "xsd:float"))
			throw std::invalid_argument("Invalid JSON wind valid data format.");

		/*
		 * Process the data fields:
		 *
		 * 	"data": 
		 *	[
		 *		{
		 *			"time":  "2021-11-17T12:08:00",
		 *			"no":  319474,
		 *			"vals": [7.37,213,0]
		 *		}
		 *	]
		 */
		auto jsonData = jsonDoc["data"][0];

		mData = jsonData["vals"];

		mWindSpeed_mps = mData[0];
		mWindDirection_deg = mData[1];
		mDataValid = mData[2] == 0;

		emit windDataChanged(mDataValid, mWindSpeed_mps, mWindDirection_deg);
	}
	catch (const std::exception& e)
	{
		QMessageBox msg(QMessageBox::Critical, "Wind Data Communication Error", e.what());
		msg.exec();

		return false;
	}

	return true;
}

void cWeatherDataModel_Http_Wind::processReply(const std::string& reply)
{
	nlohmann::json jsonDoc = nlohmann::json::parse(reply);

	auto jsonData = jsonDoc["data"][0];

	mData = jsonData["vals"];

	mWindSpeed_mps = mData[0];
	mWindDirection_deg = mData[1];
	mDataValid = mData[2] == 0;

	if (mWindSpeed_mps > mMaxWindSpeed_mps)
	{
		emit statusMessage("WARNING	HIGH WIND SPEEDS!");
	}


	emit windDataChanged(mDataValid, mWindSpeed_mps, mWindDirection_deg);
}
