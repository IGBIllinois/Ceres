

#include "GpsPropertyPage_Remote.hpp"
#include "GpsExperimentStates.hpp"

#include "RappFieldBoundary.hpp"

#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <QTimer>
#include <QTime>
#include <QCoreApplication>
#include <QMessageBox>


cGpsPropertyPage_Remote::cGpsPropertyPage_Remote(QWidget* parent)
	: cGpsPropertyPage(parent), cSensorPropertyPageRemoteInterface(parent),
		cGpsPropertiesNetEncoder(255)
{}

cGpsPropertyPage_Remote::cGpsPropertyPage_Remote(const std::string& instance, QWidget* parent)
	: cGpsPropertyPage(instance, parent), cSensorPropertyPageRemoteInterface(parent),
	cGpsPropertiesNetEncoder(255)
{
}

void cGpsPropertyPage_Remote::onConnect()
{
	setEnabled(false);

	cGpsPropertiesNetEncoder::sendQueryReferenceParameters();
}

void cGpsPropertyPage_Remote::onDisconnect()
{
	mReferenceParametersValid = false;

	doCancel();
}

void cGpsPropertyPage_Remote::showPage()
{
	openConnection();
	cGpsPropertyPage::showPage();
}

void cGpsPropertyPage_Remote::onReferenceParameters(bool valid, uint16_t min_integration_time_sec, 
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

void cGpsPropertyPage_Remote::onReferenceData(bool valid, double avg_lat_rad, double avg_lng_rad, double avg_height_m,
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

void cGpsPropertyPage_Remote::onReferenceCommandReply(eReferenceReply reply)
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
		sendQueryReferenceData();
		break;
	}

	mReferenceValid = true;
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
	doApply();
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

void cGpsPropertyPage_Remote::setReferenceParameters(std::uint16_t min_integration_time_sec, 
	std::uint16_t max_integration_time_sec, std::uint16_t ref_error_threshold_mm)
{
	mReferenceParametersValid = false;

	// We are going to try to get the reference parameters three times.
	for (int i = 0; i < 3; ++i)
	{
		cGpsPropertiesNetEncoder::sendReferenceParameters(min_integration_time_sec, max_integration_time_sec, ref_error_threshold_mm);
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

