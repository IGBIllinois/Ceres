
#include "WeatherDataModel_Http.hpp"

#include <QNetworkAccessManager>

cWeatherDataModel_Http::cWeatherDataModel_Http()
	: mpHttpManager(nullptr)
{
}

cWeatherDataModel_Http::~cWeatherDataModel_Http()
{
	stopCommunications();
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

