
#include "WeatherDataModel_Http_Wind_T_RH_PAR.hpp"
#include "../../Utilities/Constants.hpp"

#include <QEventLoop>
#include <QNetworkReply>

#include <fstream>


cWeatherDataModel_Http_Wind_T_RH_PAR::cWeatherDataModel_Http_Wind_T_RH_PAR(QObject* parent)
:
	cWeatherDataModel_Http(parent),
	mData(),
	mSerializer(4096)
{
	mMaxWindSpeed_mps = 100;
	mWindSpeed_mps = 0.0;
	mWindDirection_deg = 0.0;
	mDataValid = false;
}

uint16_t cWeatherDataModel_Http_Wind_T_RH_PAR::data_class_id() const
{
	return mSerializer.classID();
}

void cWeatherDataModel_Http_Wind_T_RH_PAR::updateViews()
{

}

bool cWeatherDataModel_Http_Wind_T_RH_PAR::configure(const nlohmann::json& jsonCfg)
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
		QString msg = "Wind Data Communication Error: ";
		msg += reply->errorString();
		emit logMessage(logERROR, q_name(), msg);

		return false;
	}

	try
	{
		auto msg = replyText.toStdString();
		nlohmann::json jsonDoc = nlohmann::json::parse(msg);
		auto& jsonHead = jsonDoc["head"];

		/*
		 * We are checking the meta data field information to make sure it matches what we expect:
		 * 
		 * 	"fields":  
		 *	[
		 * 		{
		 *			"name":  "WS_ms",
		 *			"type":  "xsd:float",
		 *			"units":  "m/s",
		 *			"process":  "Smp",
		 *			"settable":  false
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
		 *		},
		 *		{
		 *			"name":  "Temp_2M",
		 *			"type":  "xsd:float",
		 *			"process":  "Smp",
		 *			"settable":  false
		 *		},
		 *		{
		 *			"name":  "RH_2M",
		 *			"type":  "xsd:float",
		 *			"process":  "Smp",
		 *			"settable":  false
		 *		},
		 *		{
		 *			"name":  "PAR",
		 *			"type":  "xsd:float",
		 *			"units":  "umol/s/m^2",
		 *			"process":  "Smp",
		 *			"settable":  false
		 *		}
		 *	]
		 */
		auto& jsonFields = jsonHead["fields"];
		mConfigInfo = to_string(jsonFields);
		auto& wind_speed = jsonFields[0];
		if ((wind_speed["name"] != "WS_ms") || (wind_speed["type"] != "xsd:float") || (wind_speed["units"] != "m/s"))
			throw std::invalid_argument("Invalid JSON wind speed data format.");

		auto& wind_dir = jsonFields[1];
		if ((wind_dir["name"] != "WindDir") || (wind_dir["type"] != "xsd:float") || (wind_dir["units"] != "degrees"))
			throw std::invalid_argument("Invalid JSON wind direction data format.");

		auto& wind_valid = jsonFields[2];
		if ((wind_valid["name"] != "Invalid_Wind") || (wind_valid["type"] != "xsd:float"))
			throw std::invalid_argument("Invalid JSON wind valid data format.");

		auto& temp = jsonFields[3];
		if ((temp["name"] != "Temp_2M") || (temp["type"] != "xsd:float"))
			throw std::invalid_argument("Invalid JSON temperature data format.");

		auto& rh = jsonFields[4];
		if ((rh["name"] != "RH_2M") || (rh["type"] != "xsd:float"))
			throw std::invalid_argument("Invalid JSON relative humidity data format.");

		auto& par = jsonFields[5];
		if ((par["name"] != "PAR") || (par["type"] != "xsd:float"))
			throw std::invalid_argument("Invalid JSON PAR data format.");

		/*
		 * Process the data fields:
		 *
		 * 	"data": 
		 *	[
		 *		{
		 *			"time":  "2021-11-17T12:08:00",
		 *			"no":  319474,
		 *			"vals": [2.17,172.5,0,25.06,69.25,691.7]
		 *		}
		 *	]
		 */
		auto jsonData = jsonDoc["data"][0];

		mData = jsonData["vals"];

		mWindSpeed_mps = mData[0];
		mWindDirection_deg = mData[1];
		mDataValid = mData[2] == 0;
		mOAT_C = mData[3];
		mRH_pct = mData[4];
		mPAR_umole = mData[5];

		emit windDataChanged(mDataValid, mWindSpeed_mps, mWindDirection_deg);
		emit temperatureChanged(mOAT_C);
		emit relativeHumidityChanged(mRH_pct);
		emit parChanged(mPAR_umole);
	}
	catch (const std::exception& e)
	{
		QString msg = "Wind Data Communication Error: ";
		msg += e.what();
		emit logMessage(logERROR, q_name(), msg);

		return false;
	}

	return true;
}

void cWeatherDataModel_Http_Wind_T_RH_PAR::enableDataRecording(cBlockDataFileWriter& file)
{
	mSerializer.attach(&file);
}

void cWeatherDataModel_Http_Wind_T_RH_PAR::disableDataRecording()
{
	cWeatherDataModel_Http::disableDataRecording();
	mSerializer.detach();
}

void cWeatherDataModel_Http_Wind_T_RH_PAR::writeDataHeader()
{
	mSerializer.writeConfigInfo(mConfigInfo);
	mSerializer.writeWindData_mps(mDataValid, mWindSpeed_mps, mWindDirection_deg);
	mSerializer.writeTemperature_C(mOAT_C);
	mSerializer.writeRelativeHumidity_pct(mRH_pct);
	mSerializer.writePAR_umole(mPAR_umole);
}


void cWeatherDataModel_Http_Wind_T_RH_PAR::processReply(const std::string& reply)
{
	nlohmann::json jsonDoc = nlohmann::json::parse(reply);

	auto jsonData = jsonDoc["data"][0];

	mData = jsonData["vals"];

	bool wind_changed = mWindSpeed_mps != mData[0];
	mWindSpeed_mps = mData[0];

	wind_changed |= mWindDirection_deg != mData[1];
	mWindDirection_deg = mData[1];

	wind_changed |= mDataValid != (mData[2] == 0);
	mDataValid = mData[2] == 0;
	
	bool oat_changed = mOAT_C != mData[3];
	mOAT_C = mData[3];

	bool rh_changed = mRH_pct != mData[4];
	mRH_pct = mData[4];

	bool par_changed = mPAR_umole != mData[5];
	mPAR_umole = mData[5];

	if (mIsRecording && static_cast<bool>(mSerializer))
	{
		mSerializer.writeWindData_mps(mDataValid, mWindSpeed_mps, mWindDirection_deg);
		mSerializer.writeTemperature_C(mOAT_C);
		mSerializer.writeRelativeHumidity_pct(mRH_pct);
		mSerializer.writePAR_umole(mPAR_umole);
	}

	if (mWindSpeed_mps > mMaxWindSpeed_mps)
	{
		emit statusMessage("WARNING	HIGH WIND SPEEDS!");
	}

	if (wind_changed)
		emit windDataChanged(mDataValid, mWindSpeed_mps, mWindDirection_deg);

	if (oat_changed)
		emit temperatureChanged(mOAT_C);

	if (rh_changed)
		emit relativeHumidityChanged(mRH_pct);

	if (par_changed)
		emit parChanged(mPAR_umole);
}
