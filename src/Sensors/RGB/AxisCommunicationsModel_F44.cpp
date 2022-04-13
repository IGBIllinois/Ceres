
#include "AxisCommunicationsModel_F44.hpp"
#include "AxisCommunicationsFactory.hpp"

#include <QNetworkAccessManager>
#include <QMessageBox>


cAxisCommunicationsModel_F44::cAxisCommunicationsModel_F44(QObject* parent)
:
    cAxisCommunicationsModel(parent)
{
}

cAxisCommunicationsModel_F44::~cAxisCommunicationsModel_F44()
{
	stopCommunications();
}

bool cAxisCommunicationsModel_F44::configure(const nlohmann::json& jsonCfg)
{
    try
    {
        auto section = jsonCfg["F44"];
        return cAxisCommunicationsModel::configure(section);
    }
    catch (const std::exception& e)
    {
    }

    return false;
}

void cAxisCommunicationsModel_F44::writeDataHeader(cBlockDataFile& file)
{

}

void cAxisCommunicationsModel_F44::endDataRecording()
{

}


bool cAxisCommunicationsModel_F44::startCommunications()
{
	if (mpHttpManager) return true;

	mpHttpManager  = new QNetworkAccessManager(this);
//BAF	connect(mpHttpManager, &QNetworkAccessManager::finished, this, &cWeatherDataModel_Http::requestReceived);

	return true;
}

void cAxisCommunicationsModel_F44::stopCommunications()
{
    if (!mpHttpManager) return;

//BAF    disconnect(mpHttpManager, &QNetworkAccessManager::finished, this, &cWeatherDataModel_Http::requestReceived);

    delete mpHttpManager; mpHttpManager = nullptr;
}

void cAxisCommunicationsModel_F44::requestReceived(QNetworkReply* pReply)
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

void cAxisCommunicationsModel_F44::update()
{
    if (mTimer.elapsed())
    {
        getRequest();
    }
}

void cAxisCommunicationsModel_F44::getRequest()
{
    mpHttpManager->get(QNetworkRequest(mUrl));
}

