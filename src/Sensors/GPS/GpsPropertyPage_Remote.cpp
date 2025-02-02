

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

		if (cmd == "background")
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

	if (mpLenses->count() == 0)
	{
		queryLensNames();
	}

	queryState();

	setEnabled(true);
	update();
}

void cGpsPropertyPage_Remote::onDisconnect()
{
	mpLenses->clear();
	mAcquisitionParametersValid = false;

	doCancel();
}

void cGpsPropertyPage_Remote::onCurrentState(bool valid,
	std::uint16_t average_frames, std::uint32_t frame_period_us,
	std::uint32_t min_frame_period_us, std::uint32_t integration_time_us,
	std::uint32_t max_integration_time_us, std::uint32_t num_backgrounds,
	const std::string& lens_name)
{
	if (!valid) return;

	mAcquisitionParametersValid = true;

	mpAvgFrames->setText(QString::number(average_frames));
	mpFramePeriod_us->setText(QString::number(frame_period_us));
	mpMinFramePeriod_us->setText(QString::number(min_frame_period_us));
	mpIntegrationTime_us->setText(QString::number(integration_time_us));
	mpMaxIntegrationTime_us->setText(QString::number(max_integration_time_us));
	mpNumBackgrounds->setText(QString::number(num_backgrounds));

	mDefaultAverageFrames = average_frames;
	mDefaultFramePeriod_us = frame_period_us;
	mDefaultIntegrationTime_us = integration_time_us;
	mDefaultNumBackgrounds = num_backgrounds;

	mDefaultLensName = QString::fromStdString(lens_name);

	for (int i = 0; i < mpLenses->count(); ++i)
	{
		if (mDefaultLensName == mpLenses->itemText(i))
		{
			mpLenses->setCurrentIndex(i);
			break;
		}
	}
}

void cGpsPropertyPage_Remote::onLensNames(const std::vector<std::string>& names)
{
	mpLenses->clear();
	for (std::size_t i = 0; i < names.size(); ++i)
	{
		mpLenses->addItem(QString::fromStdString(names[i]));
	}
}

void cGpsPropertyPage_Remote::onCommandReply(eCommandReply reply)
{}

void cGpsPropertyPage_Remote::onBackgroundReply(eBackgroundReply reply)
{
	mBackgroundValid = true;
	setEnabled(true);
	update();
}

void cGpsPropertyPage_Remote::onShutterState(eShutterState state)
{}

void cGpsPropertyPage_Remote::showPage()
{
	if (!openConnection())
	{
		QMessageBox::warning(this, "Ceres",
			"Could not connect to the GPS controller.",
			QMessageBox::Ok);

	}

	cGpsPropertyPage::showPage();
}

void cGpsPropertyPage_Remote::doCalcBackground()
{
	if (!mConnected)
		return;

	setEnabled(false);
	update();

	sendChangedData();

	calcBackground();

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

	auto frames = mpAvgFrames->text().toInt();
	auto frame_period_us = mpFramePeriod_us->text().toInt();
	auto integration_time_us = mpIntegrationTime_us->text().toInt();
	auto num_backgrounds = mpNumBackgrounds->text().toInt();

	if ((mDefaultAverageFrames != frames)
		|| (mDefaultFramePeriod_us != frame_period_us)
		|| (mDefaultIntegrationTime_us != integration_time_us)
		|| (mDefaultNumBackgrounds != num_backgrounds))
	{
		setEnabled(false);
	}

	if ((mDefaultAverageFrames != frames)
		|| (mDefaultFramePeriod_us != frame_period_us)
		|| (mDefaultIntegrationTime_us != integration_time_us))
	{
		setAcquisitionParameters(frames, frame_period_us, integration_time_us);
	}

	if (mDefaultNumBackgrounds != num_backgrounds)
	{
		setNumOfBackgrounds(num_backgrounds);
	}
}

void cGpsPropertyPage_Remote::queryState()
{
	mAcquisitionParametersValid = false;

	// We are going to try to get the acquisition parameters three times.
	for (int i = 0; i < 3; ++i)
	{
		cGpsPropertiesNetEncoder::sendQueryState();

		QTime delayTime = QTime::currentTime().addSecs(3);
		while (QTime::currentTime() < delayTime)
		{
			QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
			if (mAcquisitionParametersValid)
				return;
		}
	}
}

void cGpsPropertyPage_Remote::queryLensNames()
{
	// We are going to try to get the lens names three times.
	for (int i = 0; i < 3; ++i)
	{
		cGpsPropertiesNetEncoder::sendQueryLensNames();

		QTime delayTime = QTime::currentTime().addSecs(3);
		while (QTime::currentTime() < delayTime)
		{
			QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
			if (mpLenses->count() > 0)
				return;
		}
	}
}

void cGpsPropertyPage_Remote::setAcquisitionParameters(std::uint16_t average_frame, std::uint32_t frame_period_us, std::uint32_t integration_time_us)
{
	mAcquisitionParametersValid = false;

	// We are going to try to get the acquisition parameters three times.
	for (int i = 0; i < 3; ++i)
	{
		cGpsPropertiesNetEncoder::sendAcquisitionParameters(average_frame, frame_period_us, integration_time_us);
		cGpsPropertiesNetEncoder::sendQueryState();

		QTime delayTime = QTime::currentTime().addSecs(3);
		while (QTime::currentTime() < delayTime)
		{
			QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
			if (mAcquisitionParametersValid)
				return;
		}
	}
}

void cGpsPropertyPage_Remote::setLensName(const std::string& lens_name)
{
	mAcquisitionParametersValid = false;

	// We are going to try to get the acquisition parameters three times.
	for (int i = 0; i < 3; ++i)
	{
		cGpsPropertiesNetEncoder::sendLensName(lens_name);
		cGpsPropertiesNetEncoder::sendQueryState();

		QTime delayTime = QTime::currentTime().addSecs(3);
		while (QTime::currentTime() < delayTime)
		{
			QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
			if (mAcquisitionParametersValid)
				return;
		}
	}
}

void cGpsPropertyPage_Remote::setNumOfBackgrounds(int num_backgrounds)
{
	mAcquisitionParametersValid = false;

	// We are going to try to get the acquisition parameters three times.
	for (int i = 0; i < 3; ++i)
	{
		cGpsPropertiesNetEncoder::sendNumOfBackgrounds(num_backgrounds);
		cGpsPropertiesNetEncoder::sendQueryState();

		QTime delayTime = QTime::currentTime().addSecs(3);
		while (QTime::currentTime() < delayTime)
		{
			QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
			if (mAcquisitionParametersValid)
				return;
		}
	}
}

void cGpsPropertyPage_Remote::calcBackground()
{
	mBackgroundValid = false;

	int secs = static_cast<int>(3 * mDefaultNumBackgrounds * mDefaultAverageFrames * (mDefaultFramePeriod_us / 1'000'000.0));

	// We are going to try to get the acquisition parameters three times.
	for (int i = 0; i < 3; ++i)
	{
		cGpsPropertiesNetEncoder::sendCalcBackground();

		QTime delayTime = QTime::currentTime().addSecs(secs);
		while (QTime::currentTime() < delayTime)
		{
			QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
			if (mBackgroundValid)
				return;
		}
	}

	cGpsPropertiesNetEncoder::sendStopBackground();
}

void cGpsPropertyPage_Remote::decodeIncomingData(const void* pBuffer, std::size_t buf_length)
{
	cGpsPropertiesNetDecoder::decode(pBuffer, buf_length);
}

int cGpsPropertyPage_Remote::sendOutgoingData(const char* data, std::size_t len)
{
	return cSensorPropertyPageRemoteInterface::sendOutgoingData(data, len);
}

