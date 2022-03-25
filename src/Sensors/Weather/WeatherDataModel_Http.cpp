
#include "WeatherDataModel_Http.hpp"

#include <QNetworkAccessManager>
#include <QMessageBox>


cWeatherDataModel_Http::cWeatherDataModel_Http()
	: mpHttpManager(nullptr)
{
    mTimer.interval_sec(static_cast<uint32_t>(10));
}

cWeatherDataModel_Http::~cWeatherDataModel_Http()
{
	stopCommunications();
}

bool cWeatherDataModel_Http::configure(const nlohmann::json& jsonCfg)
{
    std::string url;

    try
    {
        url = jsonCfg["url"];
        mUrl = QUrl(QString(url.c_str()));

        double interval_sec = jsonCfg["update interval (sec)"];
        if ((interval_sec <= 0) || (interval_sec > 600.0))
        {
            QString str = "Invalid \"update interval (sec)\" in the \"weather_data\" configuration.\n";
            str.append("The interval must be in the range >0 to <600.  The value will be ignored.");
            QMessageBox msg(QMessageBox::Critical, "Configuration Error", str);
            msg.exec();
        }
        else
        {
            mTimer.interval_sec(static_cast<uint32_t>(interval_sec));
        }

    }
    catch (const std::exception& e)
    {
        return false;
    }

    return true;
}


bool cWeatherDataModel_Http::startCommunications()
{
	if (mpHttpManager) return true;

	mpHttpManager  = new QNetworkAccessManager(this);
	connect(mpHttpManager, &QNetworkAccessManager::finished, this, &cWeatherDataModel_Http::requestReceived);

	return true;
}

void cWeatherDataModel_Http::stopCommunications()
{
    if (!mpHttpManager) return;

    disconnect(mpHttpManager, &QNetworkAccessManager::finished, this, &cWeatherDataModel_Http::requestReceived);

    delete mpHttpManager; mpHttpManager = nullptr;
}

void cWeatherDataModel_Http::requestReceived(QNetworkReply* pReply)
{
	pReply->deleteLater();

    if (pReply->error() == QNetworkReply::NoError) {
        // Get the http status code
        int v = pReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        if (v >= 200 && v < 300) // Success
        {
            // Here we got the final reply 
            QString replyText = pReply->readAll();
            processReply(replyText.toStdString());
        }
        else if (v >= 300 && v < 400) // Redirection
        {
            // Get the redirection url
            QUrl newUrl = pReply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl();
            // Because the redirection url can be relative, 
            // we have to use the previous one to resolve it 
            mUrl = pReply->url().resolved(newUrl);
        }
    }
    else
    {
        // Error
        pReply->errorString();
    }
}

void cWeatherDataModel_Http::update()
{
    if (mTimer.elapsed())
    {
        getRequest();
    }
}

void cWeatherDataModel_Http::getRequest()
{
    mpHttpManager->get(QNetworkRequest(mUrl));
}

