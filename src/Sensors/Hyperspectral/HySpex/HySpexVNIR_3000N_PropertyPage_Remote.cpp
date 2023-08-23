

#include "HySpexVNIR_3000N_PropertyPage_Remote.hpp"
#include "HySpexExperimentStates.hpp"

#include <QLineEdit>
#include <QComboBox>
#include <QTimer>
#include <QTime>
#include <QCoreApplication>
#include <QMessageBox>


cHySpexVNIR_3000N_PropertyPage_Remote::cHySpexVNIR_3000N_PropertyPage_Remote(QWidget* parent)
	: cHySpexVNIR_3000N_PropertyPage(parent), cSensorPropertyPageRemoteInterface(parent),
		cHySpexVNIR_3000N_PropertiesNetEncoder(255)
{}

cExperimentState* cHySpexVNIR_3000N_PropertyPage_Remote::createState(const std::string& type, const nlohmann::json& entry)
{
	if (type == "VNIR-3000N")
	{
		auto hostname = getHostname();
		auto port = getPort();
		auto localIp = getLocalIpAddress();
		auto use_IpV6 = usingIpV6();

		std::string cmd = entry["command"];

		if (cmd == "background")
			return new cHySpexCamera_Background_Remote(hostname, port, localIp, use_IpV6);

		if (cmd == "close shutter")
			return new cHySpexCamera_CloseShutter_Remote(hostname, port, localIp, use_IpV6);

		if (cmd == "open shutter")
			return new cHySpexCamera_OpenShutter_Remote(hostname, port, localIp, use_IpV6);
	}

	return nullptr;
}


void cHySpexVNIR_3000N_PropertyPage_Remote::onConnect()
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

void cHySpexVNIR_3000N_PropertyPage_Remote::onDisconnect()
{
	mpLenses->clear();
	mAcquisitionParametersValid = false;

	doCancel();
}

void cHySpexVNIR_3000N_PropertyPage_Remote::onCurrentState(bool valid,
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

void cHySpexVNIR_3000N_PropertyPage_Remote::onLensNames(const std::vector<std::string>& names)
{
	for (std::size_t i = 0; i < names.size(); ++i)
	{
		mpLenses->addItem(QString::fromStdString(names[i]));
	}
}

void cHySpexVNIR_3000N_PropertyPage_Remote::onCommandReply(eCommandReply reply)
{}

void cHySpexVNIR_3000N_PropertyPage_Remote::onBackgroundReply(eBackgroundReply reply)
{
	mBackgroundValid = true;
	setEnabled(true);
	update();
}

void cHySpexVNIR_3000N_PropertyPage_Remote::onShutterState(eShutterState state)
{}

void cHySpexVNIR_3000N_PropertyPage_Remote::showPage()
{
	if (!openConnection())
	{
		QMessageBox::warning(this, "Ceres",
			"Could not connect to the VNIR 3000N controller.",
			QMessageBox::Ok);

	}

	cHySpexVNIR_3000N_PropertyPage::showPage();
}

void cHySpexVNIR_3000N_PropertyPage_Remote::doCalcBackground()
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

void cHySpexVNIR_3000N_PropertyPage_Remote::doOK()
{
	sendChangedData();
	closeConnection();
	cHySpexVNIR_3000N_PropertyPage::doOK();
}

void cHySpexVNIR_3000N_PropertyPage_Remote::doCancel()
{
	closeConnection();
	cHySpexVNIR_3000N_PropertyPage::doCancel();
}

void cHySpexVNIR_3000N_PropertyPage_Remote::doApply()
{
	if (!mConnected)
		return;

	sendChangedData();

	setEnabled(true);
	update();
}

void cHySpexVNIR_3000N_PropertyPage_Remote::reject()
{
	doCancel();
}

/*
 * Network communications methods
 */
void cHySpexVNIR_3000N_PropertyPage_Remote::sendChangedData()
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

void cHySpexVNIR_3000N_PropertyPage_Remote::queryState()
{
	mAcquisitionParametersValid = false;

	// We are going to try to get the acquisition parameters three times.
	for (int i = 0; i < 3; ++i)
	{
		cHySpexVNIR_3000N_PropertiesNetEncoder::sendQueryState();

		QTime delayTime = QTime::currentTime().addSecs(3);
		while (QTime::currentTime() < delayTime)
		{
			QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
			if (mAcquisitionParametersValid)
				return;
		}
	}
}

void cHySpexVNIR_3000N_PropertyPage_Remote::queryLensNames()
{
	// We are going to try to get the lens names three times.
	for (int i = 0; i < 3; ++i)
	{
		cHySpexVNIR_3000N_PropertiesNetEncoder::sendQueryLensNames();

		QTime delayTime = QTime::currentTime().addSecs(3);
		while (QTime::currentTime() < delayTime)
		{
			QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
			if (mpLenses->count() > 0)
				return;
		}
	}
}

void cHySpexVNIR_3000N_PropertyPage_Remote::setAcquisitionParameters(std::uint16_t average_frame, std::uint32_t frame_period_us, std::uint32_t integration_time_us)
{
	mAcquisitionParametersValid = false;

	// We are going to try to get the acquisition parameters three times.
	for (int i = 0; i < 3; ++i)
	{
		cHySpexVNIR_3000N_PropertiesNetEncoder::sendAcquisitionParameters(average_frame, frame_period_us, integration_time_us);
		cHySpexVNIR_3000N_PropertiesNetEncoder::sendQueryState();

		QTime delayTime = QTime::currentTime().addSecs(3);
		while (QTime::currentTime() < delayTime)
		{
			QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
			if (mAcquisitionParametersValid)
				return;
		}
	}
}

void cHySpexVNIR_3000N_PropertyPage_Remote::setLensName(const std::string& lens_name)
{
	mAcquisitionParametersValid = false;

	// We are going to try to get the acquisition parameters three times.
	for (int i = 0; i < 3; ++i)
	{
		cHySpexVNIR_3000N_PropertiesNetEncoder::sendLensName(lens_name);
		cHySpexVNIR_3000N_PropertiesNetEncoder::sendQueryState();

		QTime delayTime = QTime::currentTime().addSecs(3);
		while (QTime::currentTime() < delayTime)
		{
			QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
			if (mAcquisitionParametersValid)
				return;
		}
	}
}

void cHySpexVNIR_3000N_PropertyPage_Remote::setNumOfBackgrounds(int num_backgrounds)
{
	mAcquisitionParametersValid = false;

	// We are going to try to get the acquisition parameters three times.
	for (int i = 0; i < 3; ++i)
	{
		cHySpexVNIR_3000N_PropertiesNetEncoder::sendNumOfBackgrounds(num_backgrounds);
		cHySpexVNIR_3000N_PropertiesNetEncoder::sendQueryState();

		QTime delayTime = QTime::currentTime().addSecs(3);
		while (QTime::currentTime() < delayTime)
		{
			QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
			if (mAcquisitionParametersValid)
				return;
		}
	}
}

void cHySpexVNIR_3000N_PropertyPage_Remote::calcBackground()
{
	mBackgroundValid = false;

	int secs = static_cast<int>(3 * mDefaultNumBackgrounds * mDefaultAverageFrames * (mDefaultFramePeriod_us / 1'000'000.0));

	// We are going to try to get the acquisition parameters three times.
	for (int i = 0; i < 3; ++i)
	{
		cHySpexVNIR_3000N_PropertiesNetEncoder::sendCalcBackground();

		QTime delayTime = QTime::currentTime().addSecs(secs);
		while (QTime::currentTime() < delayTime)
		{
			QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
			if (mBackgroundValid)
				return;
		}
	}

	cHySpexVNIR_3000N_PropertiesNetEncoder::sendStopBackground();
}

void cHySpexVNIR_3000N_PropertyPage_Remote::decodeIncomingData(const void* pBuffer, std::size_t buf_length)
{
	cHySpexVNIR_3000N_PropertiesNetDecoder::decode(pBuffer, buf_length);
}

int cHySpexVNIR_3000N_PropertyPage_Remote::sendOutgoingData(const char* data, std::size_t len)
{
	return cSensorPropertyPageRemoteInterface::sendOutgoingData(data, len);
}

