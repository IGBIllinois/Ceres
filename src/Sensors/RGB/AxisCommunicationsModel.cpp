
#include "AxisCommunicationsModel_F44.hpp"
#include "AxisCommunicationsFactory.hpp"

#include <QNetworkAccessManager>
#include <QMessageBox>
#include <QEventLoop>


cAxisCommunicationsModel::cAxisCommunicationsModel(QObject* parent)
:
    cRgbCameraModel(parent),
    mpHttpManager(nullptr)
{
    mpHttpManager = new QNetworkAccessManager(this);
    mTimer.interval_sec(static_cast<uint32_t>(10));
}

cAxisCommunicationsModel::~cAxisCommunicationsModel()
{
	stopCommunications();
}

char* cAxisCommunicationsModel::descriptor() const
{
    return axis_communications_id;
}

bool cAxisCommunicationsModel::configure(const nlohmann::json& jsonCfg)
{
    try
    {
        std::string url = jsonCfg["url"];
        mUrl = QUrl(QString(url.c_str()));
    }
    catch (const std::exception& e)
    {
        QString str = "Error in the \"axis_communications\" configuration: ";
        str.append(e.what());
        emit errorMessage("Configuration Error", str);
        return false;
    }
    mUrl.setPath("axis-cgi/param.cgi?action=list&group=Properties.API.HTTP.Version");
    auto s = mUrl.toString();

    QUrl url("http://172.16.2.141/axis-cgi/param.cgi?action=list&group=Properties.API.HTTP.Version");

	emit statusMessage("Testing the connection to the weather station...");


	QNetworkAccessManager manager; // = new QNetworkAccessManager();
	QNetworkReply* reply = manager.get(QNetworkRequest(url));

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
		emit errorMessage("Axis Communication Error", reply->errorString());

		return false;
	}

	try
	{
		nlohmann::json jsonDoc = nlohmann::json::parse(replyText.toStdString());
	}
	catch (const std::exception& e)
	{
		emit errorMessage("Axis Communication Error", e.what());

		return false;
	}

    return cRgbCameraModel::configure(jsonCfg);
}

void cAxisCommunicationsModel::writeDataHeader(cBlockDataFile& file)
{

}

void cAxisCommunicationsModel::endDataRecording()
{

}


bool cAxisCommunicationsModel::startCommunications()
{
	if (mpHttpManager) return true;

	mpHttpManager  = new QNetworkAccessManager(this);
//BAF	connect(mpHttpManager, &QNetworkAccessManager::finished, this, &cWeatherDataModel_Http::requestReceived);

	return true;
}

void cAxisCommunicationsModel::stopCommunications()
{
    if (!mpHttpManager) return;

//BAF    disconnect(mpHttpManager, &QNetworkAccessManager::finished, this, &cWeatherDataModel_Http::requestReceived);

    delete mpHttpManager; mpHttpManager = nullptr;
}

void cAxisCommunicationsModel::requestReceived(QNetworkReply* pReply)
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

void cAxisCommunicationsModel::update()
{
    if (mTimer.elapsed())
    {
        getRequest();
    }
}

void cAxisCommunicationsModel::getRequest()
{
    mpHttpManager->get(QNetworkRequest(mUrl));
}

