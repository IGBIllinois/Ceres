

#include "GpsPropertyPage_Remote.hpp"
#include "GpsExperimentStates.hpp"

#include <QLineEdit>
#include <QComboBox>
#include <QTimer>
#include <QTime>
#include <QCoreApplication>
#include <QMessageBox>


cGpsPropertyPage_Remote::cGpsPropertyPage_Remote(QWidget* parent)
	: cGpsPropertyPage(parent), cSensorPropertyPageRemoteInterface(parent),
		cGpsPropertiesNetEncoder(255)
{}

cExperimentState* cGpsPropertyPage_Remote::createState(const std::string& type, const nlohmann::json& entry, QObject* parent)
{
	if (type == "GPS")
	{
		auto hostname = getHostname();
		auto port = getPort();
		auto localIp = getLocalIpAddress();
		auto use_IpV6 = usingIpV6();

		std::string cmd = entry["command"];

		if (cmd == "reference")
		{
			auto* pState = new cGpsReferenceAcquisition_Remote(hostname, port, localIp, use_IpV6, parent);

			if (parent)
				pState->moveToThread(parent->thread());

			return pState;
		}
	}

	return nullptr;
}


void cGpsPropertyPage_Remote::onConnect()
{
	setEnabled(false);


	setEnabled(true);
	update();
}

void cGpsPropertyPage_Remote::onDisconnect()
{
	mReferenceParametersValid = false;

	doCancel();
}

void cGpsPropertyPage_Remote::showPage()
{

}

void cGpsPropertyPage_Remote::onReferenceParameters(bool valid, uint16_t min_integration_time_sec, uint16_t max_integration_time_sec)
{
	if (!valid) return;

	mReferenceParametersValid = true;

	mpMinIntegrationTime_sec->setText(QString::number(min_integration_time_sec));
	mpMaxIntegrationTime_sec->setText(QString::number(max_integration_time_sec));

	mDefaultMinIntegrationTime_sec = min_integration_time_sec;
	mDefaultMaxIntegrationTime_sec = max_integration_time_sec;
}

void cGpsPropertyPage_Remote::onReferenceData(bool valid, double avg_lat_rad, double avg_lng_rad, double avg_height_m,
	double std_lat_rad, double std_lng_rad, double std_height_m, bool height_valid)
{

}

void cGpsPropertyPage_Remote::onReferenceCommandReply(eReferenceReply reply)
{
	mReferenceValid = true;
	setEnabled(true);
	update();
}

void cGpsPropertyPage_Remote::doCalcReference()
{
	if (!mConnected)
		return;

	setEnabled(false);
	update();

	sendChangedData();

	calcReference();

	setEnabled(true);
	update();
}

void cGpsPropertyPage_Remote::doOK()
{
	sendChangedData();
	closeConnection();
	cGpsPropertyPage::doOK();
}

void cGpsPropertyPage_Remote::doCancel()
{
	closeConnection();
	cGpsPropertyPage::doCancel();
}

void cGpsPropertyPage_Remote::doApply()
{
	if (!mConnected)
		return;

	sendChangedData();

	setEnabled(true);
	update();
}

void cGpsPropertyPage_Remote::reject()
{
	doCancel();
}

/*
 * Network communications methods
 */
void cGpsPropertyPage_Remote::sendChangedData()
{
	if (!mConnected)
		return;

	auto min_integration_time_sec = mpMinIntegrationTime_sec->text().toInt();
	auto max_integration_time_sec = mpMaxIntegrationTime_sec->text().toInt();

	if (max_integration_time_sec < min_integration_time_sec)
		std::swap(min_integration_time_sec, max_integration_time_sec);

	if ((mDefaultMinIntegrationTime_sec != min_integration_time_sec)
		|| (mDefaultMaxIntegrationTime_sec != max_integration_time_sec))
	{
		setEnabled(false);
	}

	if ((mDefaultMinIntegrationTime_sec != min_integration_time_sec)
		|| (mDefaultMaxIntegrationTime_sec != max_integration_time_sec))
	{
		setReferenceParameters(min_integration_time_sec, max_integration_time_sec);
	}
}

void cGpsPropertyPage_Remote::queryReferenceParameters()
{
	mReferenceParametersValid = false;

	// We are going to try to get the acquisition parameters three times.
	for (int i = 0; i < 3; ++i)
	{
		cGpsPropertiesNetEncoder::sendQueryReferenceParameters();

		QTime delayTime = QTime::currentTime().addSecs(3);
		while (QTime::currentTime() < delayTime)
		{
			QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
			if (mReferenceParametersValid)
				return;
		}
	}
}

void cGpsPropertyPage_Remote::queryReferenceData()
{
	// We are going to try to get the lens names three times.
	for (int i = 0; i < 3; ++i)
	{
		cGpsPropertiesNetEncoder::sendQueryReferenceData();

		QTime delayTime = QTime::currentTime().addSecs(3);
		while (QTime::currentTime() < delayTime)
		{
			QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
				return;
		}
	}
}

void cGpsPropertyPage_Remote::setReferenceParameters(std::uint16_t integration_time_sec, std::uint16_t max_integration_time_sec)
{
	mReferenceParametersValid = false;

	// We are going to try to get the reference parameters three times.
	for (int i = 0; i < 3; ++i)
	{
		cGpsPropertiesNetEncoder::sendReferenceParameters(integration_time_sec, max_integration_time_sec);
		cGpsPropertiesNetEncoder::sendQueryReferenceParameters();

		QTime delayTime = QTime::currentTime().addSecs(3);
		while (QTime::currentTime() < delayTime)
		{
			QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
			if (mReferenceParametersValid)
				return;
		}
	}
}

void cGpsPropertyPage_Remote::calcReference()
{
	mReferenceValid = false;

	int secs = 1000; // static_cast<int>(3 * mDefaultNumBackgrounds * mDefaultAverageFrames * (mDefaultFramePeriod_us / 1'000'000.0));

	// We are going to try to get the acquisition parameters three times.
	for (int i = 0; i < 3; ++i)
	{
		cGpsPropertiesNetEncoder::sendCalcReference();

		QTime delayTime = QTime::currentTime().addSecs(secs);
		while (QTime::currentTime() < delayTime)
		{
			QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
			if (mReferenceValid)
				return;
		}
	}

	cGpsPropertiesNetEncoder::sendStopReference();
}

void cGpsPropertyPage_Remote::decodeIncomingData(const void* pBuffer, std::size_t buf_length)
{
	cGpsPropertiesNetDecoder::decode(pBuffer, buf_length);
}

int cGpsPropertyPage_Remote::sendOutgoingData(const char* data, std::size_t len)
{
	return cSensorPropertyPageRemoteInterface::sendOutgoingData(data, len);
}

