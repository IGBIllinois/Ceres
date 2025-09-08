

#include "HySpexSWIR_384_PropertyPage_Remote.hpp"
#include "HySpexExperimentStates.hpp"

#include <QLineEdit>
#include <QComboBox>
#include <QTimer>
#include <QTime>
#include <QCoreApplication>
#include <QMessageBox>
#include <QPushButton>
#include <QDialogButtonBox>

namespace
{
	const QString ReTry = "ReConnect";
}


cHySpexSWIR_384_PropertyPage_Remote::cHySpexSWIR_384_PropertyPage_Remote(QWidget* parent)
	: cHySpexSWIR_384_PropertyPage(parent), cSensorPropertyPageRemoteInterface(parent),
		cHySpexSWIR_384_PropertiesNetEncoder(255)
{}

cExperimentState* cHySpexSWIR_384_PropertyPage_Remote::createState(const std::string& type, const nlohmann::json& entry, QObject* parent)
{
	if (type == "SWIR-384")
	{
		auto hostname = getHostname();
		auto port = getPort();
		auto localIp = getLocalIpAddress();
		auto use_IpV6 = usingIpV6();

		std::string cmd = entry["command"];

		if (cmd == "background")
		{
			auto* pState = new cHySpexCamera_Background_Remote(hostname, port, localIp, use_IpV6, parent);

			if (parent)
				pState->moveToThread(parent->thread());

			return pState;
		}

		if (cmd == "close shutter")
		{
			auto* pState = new cHySpexCamera_CloseShutter_Remote(hostname, port, localIp, use_IpV6, parent);

			if (parent)
				pState->moveToThread(parent->thread());

			return pState;
		}

		if (cmd == "open shutter")
		{
			auto* pState = new cHySpexCamera_OpenShutter_Remote(hostname, port, localIp, use_IpV6, parent);

			if (parent)
				pState->moveToThread(parent->thread());

			return pState;
		}
	}

	return nullptr;
}

void cHySpexSWIR_384_PropertyPage_Remote::createWidgets()
{
	cHySpexSWIR_384_PropertyPage::createWidgets();

	mpButtons->addButton(ReTry, QDialogButtonBox::ButtonRole::HelpRole);
}

void cHySpexSWIR_384_PropertyPage_Remote::enableControls(bool enable)
{
	cHySpexSWIR_384_PropertyPage::enableControls(enable);

	auto* ok = mpButtons->button(QDialogButtonBox::Ok);
	auto* apply = mpButtons->button(QDialogButtonBox::Apply);

	if (ok) ok->setEnabled(enable);
	if (apply) apply->setEnabled(enable);
}

void cHySpexSWIR_384_PropertyPage_Remote::buttonClicked(QAbstractButton* button)
{
	auto text = button->text();
	if (text == ReTry)
	{
		if (!mConnected)
		{
			if (!openConnection())
			{
				QMessageBox::warning(this, "Ceres",
					"Could not connect to the SWIR 384 controller.",
					QMessageBox::Ok);
			}
		}
		else
		{
			mReconnectActive = true;

			button->setEnabled(false);

			closeConnection();

			QTime delayTime = QTime::currentTime().addSecs(5);
			while (QTime::currentTime() < delayTime)
			{
				QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
			}

			if (!openConnection())
			{
				QMessageBox::warning(this, "Ceres",
					"Could not connect to the SWIR 384 controller.",
					QMessageBox::Ok);
			}

			button->setEnabled(true);

			mReconnectActive = false;
		}

		return;
	}

	cHySpexSWIR_384_PropertyPage::buttonClicked(button);
}


void cHySpexSWIR_384_PropertyPage_Remote::onConnect()
{
	enableControls(false);

	if (mpLenses->count() == 0)
	{
		queryLensNames();
	}

	queryState();

	enableControls(true);
	update();
}

void cHySpexSWIR_384_PropertyPage_Remote::onDisconnect()
{
	if (mReconnectActive)
		return;

	mpLenses->clear();
	mAcquisitionParametersValid = false;

	doCancel();
}

void cHySpexSWIR_384_PropertyPage_Remote::onCurrentState(bool valid, 
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

void cHySpexSWIR_384_PropertyPage_Remote::onLensNames(const std::vector<std::string>& names)
{
	mpLenses->clear();
	for (std::size_t i = 0; i < names.size(); ++i)
	{
		mpLenses->addItem(QString::fromStdString(names[i]));
	}
}

void cHySpexSWIR_384_PropertyPage_Remote::onCommandReply(eCommandReply reply)
{
}

void cHySpexSWIR_384_PropertyPage_Remote::onBackgroundReply(eBackgroundReply reply)
{
	mBackgroundValid = true;
	enableControls(true);
	update();
}

void cHySpexSWIR_384_PropertyPage_Remote::onShutterState(eShutterState state)
{}

void cHySpexSWIR_384_PropertyPage_Remote::showPage()
{
	if (!openConnection())
	{
		QMessageBox::warning(this, "Ceres",
			"Could not connect to the SWIR 384 controller.",
			QMessageBox::Ok);
	}

	cHySpexSWIR_384_PropertyPage::showPage();
}

void cHySpexSWIR_384_PropertyPage_Remote::doCalcBackground()
{
	if (!mConnected)
		return;

	enableControls(false);
	update();

	sendChangedData();

	calcBackground();

	enableControls(true);
	update();
}

void cHySpexSWIR_384_PropertyPage_Remote::doOK()
{
	sendChangedData();
	closeConnection();
	cHySpexSWIR_384_PropertyPage::doOK();
}

void cHySpexSWIR_384_PropertyPage_Remote::doCancel()
{
	closeConnection();
	cHySpexSWIR_384_PropertyPage::doCancel();
}

void cHySpexSWIR_384_PropertyPage_Remote::doApply()
{
	if (!mConnected)
		return;

	sendChangedData();

	enableControls(true);
	update();
}

void cHySpexSWIR_384_PropertyPage_Remote::reject()
{
	doCancel();
}

/*
 * Network communications methods
 */
void cHySpexSWIR_384_PropertyPage_Remote::sendChangedData()
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
		enableControls(false);
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

void cHySpexSWIR_384_PropertyPage_Remote::queryState()
{
	mAcquisitionParametersValid = false;

	// We are going to try to get the acquisition parameters three times.
	for (int i = 0; i < 3; ++i)
	{
		cHySpexSWIR_384_PropertiesNetEncoder::sendQueryState();

		QTime delayTime = QTime::currentTime().addSecs(3);
		while (QTime::currentTime() < delayTime)
		{
			QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
			if (mAcquisitionParametersValid)
				return;
		}
	}
}

void cHySpexSWIR_384_PropertyPage_Remote::queryLensNames()
{
	// We are going to try to get the lens names three times.
	for (int i = 0; i < 3; ++i)
	{
		cHySpexSWIR_384_PropertiesNetEncoder::sendQueryLensNames();

		QTime delayTime = QTime::currentTime().addSecs(3);
		while (QTime::currentTime() < delayTime)
		{
			QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
			if (mpLenses->count() > 0)
				return;
		}
	}
}

void cHySpexSWIR_384_PropertyPage_Remote::setAcquisitionParameters(std::uint16_t average_frame, std::uint32_t frame_period_us, std::uint32_t integration_time_us)
{
	mAcquisitionParametersValid = false;

	// We are going to try to get the acquisition parameters three times.
	for (int i = 0; i < 3; ++i)
	{
		cHySpexSWIR_384_PropertiesNetEncoder::sendAcquisitionParameters(average_frame, frame_period_us, integration_time_us);
		cHySpexSWIR_384_PropertiesNetEncoder::sendQueryState();

		QTime delayTime = QTime::currentTime().addSecs(3);
		while (QTime::currentTime() < delayTime)
		{
			QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
			if (mAcquisitionParametersValid)
				return;
		}
	}
}

void cHySpexSWIR_384_PropertyPage_Remote::setLensName(const std::string& lens_name)
{
	mAcquisitionParametersValid = false;

	// We are going to try to get the acquisition parameters three times.
	for (int i = 0; i < 3; ++i)
	{
		cHySpexSWIR_384_PropertiesNetEncoder::sendLensName(lens_name);
		cHySpexSWIR_384_PropertiesNetEncoder::sendQueryState();

		QTime delayTime = QTime::currentTime().addSecs(3);
		while (QTime::currentTime() < delayTime)
		{
			QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
			if (mAcquisitionParametersValid)
				return;
		}
	}
}

void cHySpexSWIR_384_PropertyPage_Remote::setNumOfBackgrounds(int num_backgrounds)
{
	mAcquisitionParametersValid = false;

	// We are going to try to get the acquisition parameters three times.
	for (int i = 0; i < 3; ++i)
	{
		cHySpexSWIR_384_PropertiesNetEncoder::sendNumOfBackgrounds(num_backgrounds);
		cHySpexSWIR_384_PropertiesNetEncoder::sendQueryState();

		QTime delayTime = QTime::currentTime().addSecs(3);
		while (QTime::currentTime() < delayTime)
		{
			QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
			if (mAcquisitionParametersValid)
				return;
		}
	}
}

void cHySpexSWIR_384_PropertyPage_Remote::calcBackground()
{
	mBackgroundValid = false;

	int secs = static_cast<int>(3 * mDefaultNumBackgrounds * mDefaultAverageFrames * (mDefaultFramePeriod_us / 1'000'000.0));

	// We are going to try to get the acquisition parameters three times.
	for (int i = 0; i < 3; ++i)
	{
		cHySpexSWIR_384_PropertiesNetEncoder::sendCalcBackground();

		QTime delayTime = QTime::currentTime().addSecs(secs);
		while (QTime::currentTime() < delayTime)
		{
			QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
			if (mBackgroundValid)
				return;
		}
	}

	cHySpexSWIR_384_PropertiesNetEncoder::sendStopBackground();
}

void cHySpexSWIR_384_PropertyPage_Remote::decodeIncomingData(const void* pBuffer, std::size_t buf_length)
{
	cHySpexSWIR_384_PropertiesNetDecoder::decode(pBuffer, buf_length);
}

int cHySpexSWIR_384_PropertyPage_Remote::sendOutgoingData(const char* data, std::size_t len)
{
	return cSensorPropertyPageRemoteInterface::sendOutgoingData(data, len);
}

