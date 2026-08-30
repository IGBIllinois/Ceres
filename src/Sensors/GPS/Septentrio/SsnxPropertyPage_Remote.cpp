

#include "SsnxPropertyPage_Remote.hpp"
#include "SsnxIDs.hpp"
#include "../GpsExperimentStates_Remote.hpp"

#include "RappFieldBoundary.hpp"

#include <QLineEdit>
#include <QPushButton>
//#include <QComboBox>
#include <QTimer>
#include <QTime>
#include <QCoreApplication>
//#include <QMessageBox>


cSsnxPropertyPage_Remote::cSsnxPropertyPage_Remote(QWidget* parent)
	: cSsnxPropertyPage(parent), cSensorPropertyPageRemoteInterface(parent),
	cGpsPropertiesNetEncoder(255)
{}

cSsnxPropertyPage_Remote::cSsnxPropertyPage_Remote(const std::string& instance, QWidget* parent)
	: cSsnxPropertyPage(instance, parent), cSensorPropertyPageRemoteInterface(parent),
	cGpsPropertiesNetEncoder(255)
{}

cExperimentState* cSsnxPropertyPage_Remote::createState(const std::string& type, const nlohmann::json& entry, QObject* parent)
{
	if (type == ssnx_id)
	{
		auto hostname = getHostname();
		auto port = getPort();
		auto localIp = getLocalIpAddress();
		auto use_IpV6 = usingIpV6();

		std::string instance;
		if (entry.contains("instance"))
		{
			instance = entry["instance"];
		}

		if (!instance.empty())
		{
			if (instance != this->instance())
				return nullptr;
		}

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


void cSsnxPropertyPage_Remote::onConnect()
{
	setEnabled(false);

	cGpsPropertiesNetEncoder::sendQueryReferenceParametersMessage();
}

void cSsnxPropertyPage_Remote::onDisconnect()
{
	mReferenceParametersValid = false;

	doCancel();
}

void cSsnxPropertyPage_Remote::showPage()
{
	openConnection();
	cSsnxPropertyPage::showPage();
}

void cSsnxPropertyPage_Remote::onReferenceParametersMessage(bool valid, uint16_t min_integration_time_sec,
	uint16_t max_integration_time_sec, uint16_t ref_error_threshold_mm)
{
	if (!valid) return;

	mReferenceParametersValid = true;

	mpMinIntegrationTime_sec->setText(QString::number(min_integration_time_sec));
	mpMaxIntegrationTime_sec->setText(QString::number(max_integration_time_sec));
	mpErrorThreshold_mm->setText(QString::number(ref_error_threshold_mm));

	mDefaultMinIntegrationTime_sec = min_integration_time_sec;
	mDefaultMaxIntegrationTime_sec = max_integration_time_sec;
	mDefaultErrorThreshold_mm = ref_error_threshold_mm;

	setEnabled(true);
	update();
}

void cSsnxPropertyPage_Remote::onReferenceDataMessage(bool valid, double avg_lat_rad, double avg_lng_rad, double avg_height_m,
	double std_lat_rad, double std_lng_rad, double std_height_m, bool height_valid)
{
	if (!valid)
	{
		mpRef_X_mm->setText("");
		mpRef_Y_mm->setText("");
		mpRef_Z_mm->setText("");
		mpRef_Error_mm->setText("");
		return;
	}

	auto p = rfb::fromGPS(avg_lat_rad, avg_lng_rad, avg_height_m);

	auto p1 = rfb::fromGPS(avg_lat_rad - std_lat_rad, avg_lng_rad - std_lng_rad, avg_height_m - std_height_m);
	auto p2 = rfb::fromGPS(avg_lat_rad + std_lat_rad, avg_lng_rad + std_lng_rad, avg_height_m + std_height_m);

	double dx = p2.x_mm - p1.x_mm;
	double dy = p2.y_mm - p1.y_mm;
	double dz = p2.z_mm - p1.z_mm;

	double error = sqrt(dx * dx + dy * dy + dz * dz);

	mpRef_X_mm->setText(QString::number(p.x_mm));
	mpRef_Y_mm->setText(QString::number(p.y_mm));

	if (height_valid)
		mpRef_Z_mm->setText(QString::number(p.z_mm));
	else
		mpRef_Z_mm->setText("");

	mpRef_Error_mm->setText(QString::number(error));
}


void cSsnxPropertyPage_Remote::onReferencePositionMessage(int x_mm, int y_mm, int z_mm, double error_mm, int count)
{
	if ((x_mm == -1) && (y_mm == -1) && (z_mm == -1) && (error_mm < 0.0))
	{
		mpRef_X_mm->setText("");
		mpRef_Y_mm->setText("");
		mpRef_Z_mm->setText("");
		mpRef_Error_mm->setText("");
		return;
	}

	mpRef_X_mm->setText(QString::number(x_mm));
	mpRef_Y_mm->setText(QString::number(y_mm));
	mpRef_Z_mm->setText(QString::number(z_mm));

	mpRef_Error_mm->setText(QString::number(error_mm));
}

void cSsnxPropertyPage_Remote::onReferenceCommandReplyMessage(eReferenceReply reply)
{
	setEnabled(true);

	switch (reply)
	{
	case eReferenceReply::PENDING:
		mpDoReference->setEnabled(false);
		break;
	case eReferenceReply::ABORTED:
	case eReferenceReply::FAILED:
		mpDoReference->setEnabled(true);
		break;
	case eReferenceReply::GOOD:
		mpDoReference->setEnabled(true);
		sendQueryReferenceDataMessage();
		break;
	}

	mReferenceValid = true;
	update();
}

void cSsnxPropertyPage_Remote::doCalcReference()
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

void cSsnxPropertyPage_Remote::doOK()
{
	doApply();
	closeConnection();
	cSsnxPropertyPage::doOK();
}

void cSsnxPropertyPage_Remote::doCancel()
{
	closeConnection();
	cSsnxPropertyPage::doCancel();
}

void cSsnxPropertyPage_Remote::doApply()
{
	if (!mConnected)
		return;

	sendChangedData();

	setEnabled(true);
	update();
}

void cSsnxPropertyPage_Remote::reject()
{
	doCancel();
}

/*
 * Network communications methods
 */
void cSsnxPropertyPage_Remote::sendChangedData()
{
	if (!mConnected)
		return;

	auto min_integration_time_sec = mpMinIntegrationTime_sec->text().toInt();
	auto max_integration_time_sec = mpMaxIntegrationTime_sec->text().toInt();
	auto error_threshold_mm = mpErrorThreshold_mm->text().toInt();

	if (max_integration_time_sec < min_integration_time_sec)
		std::swap(min_integration_time_sec, max_integration_time_sec);

	if ((mDefaultMinIntegrationTime_sec != min_integration_time_sec)
		|| (mDefaultMaxIntegrationTime_sec != max_integration_time_sec))
	{
		setEnabled(false);
	}

	if ((mDefaultMinIntegrationTime_sec != min_integration_time_sec)
		|| (mDefaultMaxIntegrationTime_sec != max_integration_time_sec)
		|| (mDefaultErrorThreshold_mm != error_threshold_mm))
	{
		setReferenceParameters(min_integration_time_sec, max_integration_time_sec, error_threshold_mm);
	}

	mDefaultMinIntegrationTime_sec = min_integration_time_sec;
	mDefaultMaxIntegrationTime_sec = max_integration_time_sec;
	mDefaultErrorThreshold_mm = error_threshold_mm;
}

void cSsnxPropertyPage_Remote::queryReferenceParameters()
{
	mReferenceParametersValid = false;

	// We are going to try to get the acquisition parameters three times.
	for (int i = 0; i < 3; ++i)
	{
		cGpsPropertiesNetEncoder::sendQueryReferenceParametersMessage();

		QTime delayTime = QTime::currentTime().addSecs(3);
		while (QTime::currentTime() < delayTime)
		{
			QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
			if (mReferenceParametersValid)
				return;
		}
	}
}

void cSsnxPropertyPage_Remote::queryReferenceData()
{
	// We are going to try to get the lens names three times.
	for (int i = 0; i < 3; ++i)
	{
		cGpsPropertiesNetEncoder::sendQueryReferenceDataMessage();

		QTime delayTime = QTime::currentTime().addSecs(3);
		while (QTime::currentTime() < delayTime)
		{
			QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
			return;
		}
	}
}

void cSsnxPropertyPage_Remote::setReferenceParameters(std::uint16_t min_integration_time_sec,
	std::uint16_t max_integration_time_sec, std::uint16_t ref_error_threshold_mm)
{
	mReferenceParametersValid = false;

	// We are going to try to get the reference parameters three times.
	for (int i = 0; i < 3; ++i)
	{
		cGpsPropertiesNetEncoder::sendReferenceParametersMessage(min_integration_time_sec, max_integration_time_sec, ref_error_threshold_mm);
		cGpsPropertiesNetEncoder::sendQueryReferenceParametersMessage();

		QTime delayTime = QTime::currentTime().addSecs(3);
		while (QTime::currentTime() < delayTime)
		{
			QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
			if (mReferenceParametersValid)
				return;
		}
	}
}

void cSsnxPropertyPage_Remote::calcReference()
{
	mReferenceValid = false;

	int secs = 1000; // static_cast<int>(3 * mDefaultNumBackgrounds * mDefaultAverageFrames * (mDefaultFramePeriod_us / 1'000'000.0));

	// We are going to try to get the acquisition parameters three times.
	for (int i = 0; i < 3; ++i)
	{
		cGpsPropertiesNetEncoder::sendCalcReferenceMessage();

		QTime delayTime = QTime::currentTime().addSecs(secs);
		while (QTime::currentTime() < delayTime)
		{
			QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
			if (mReferenceValid)
				return;
		}
	}

	cGpsPropertiesNetEncoder::sendStopReferenceMessage();
}

void cSsnxPropertyPage_Remote::decodeIncomingData(const void* pBuffer, std::size_t buf_length)
{
	cGpsPropertiesNetDecoder::decode(pBuffer, buf_length);
}

int cSsnxPropertyPage_Remote::sendOutgoingData(const char* data, std::size_t len)
{
	return cSensorPropertyPageRemoteInterface::sendOutgoingData(data, len);
}

