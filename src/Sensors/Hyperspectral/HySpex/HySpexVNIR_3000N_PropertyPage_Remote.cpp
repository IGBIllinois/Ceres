

#include "HySpexVNIR_3000N_PropertyPage_Remote.hpp"
#include "HySpexExperimentStates.hpp"

#include <QLineEdit>
#include <QComboBox>

cHySpexVNIR_3000N_PropertyPage_Remote::cHySpexVNIR_3000N_PropertyPage_Remote(QWidget* parent)
	: cHySpexVNIR_3000N_PropertyPage(parent), cSensorPropertyPageRemoteInterface(parent),
		cHySpexVNIR_3000N_PropertiesNetEncoder(255)
{}

cExperimentState* cHySpexVNIR_3000N_PropertyPage_Remote::createState(const std::string& type, const nlohmann::json& entry)
{
	if (type == "VNIR-3000N")
	{
		std::string cmd = entry["command"];
		return new cHySpexVNIR_3000N_Properties_Remote(this);
	}

	return nullptr;
}


void cHySpexVNIR_3000N_PropertyPage_Remote::onConnect()
{
	setEnabled(false);
	cHySpexVNIR_3000N_PropertiesNetEncoder::sendQueryLensNames();
	cHySpexVNIR_3000N_PropertiesNetEncoder::sendQueryState();
}

void cHySpexVNIR_3000N_PropertyPage_Remote::onCurrentState(bool valid,
	std::uint16_t average_frames, std::uint32_t frame_period_us,
	std::uint32_t min_frame_period_us, std::uint32_t integration_time_us,
	std::uint32_t max_integration_time_us, std::uint32_t num_backgrounds,
	const std::string& lens_name)
{
	if (!valid) return;

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

	if (!mWaitingForBackgroundReply)
		setEnabled(true);
	update();
}

void cHySpexVNIR_3000N_PropertyPage_Remote::onLensNames(const std::vector<std::string>& names)
{
	for (std::size_t i = 0; i < names.size(); ++i)
	{
		mpLenses->addItem(QString::fromStdString(names[i]));
	}
}

void cHySpexVNIR_3000N_PropertyPage_Remote::onBackgroundReply(eBackgroundReply reply)
{
	mWaitingForBackgroundReply = false;
	setEnabled(true);
	update();
}

void cHySpexVNIR_3000N_PropertyPage_Remote::onShutterState(eShutterState state)
{
}

void cHySpexVNIR_3000N_PropertyPage_Remote::showPage()
{
	openConnection();
	cHySpexVNIR_3000N_PropertyPage::showPage();
}

void cHySpexVNIR_3000N_PropertyPage_Remote::doCalcBackground()
{
	if (!mConnected)
		return;

	mWaitingForBackgroundReply = true;

	setEnabled(false);

	doApply();

	sendCalcBackground();
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

	bool needs_update = false;

	sendChangedData(&needs_update);

	if (needs_update)
	{
		setEnabled(false);
		sendQueryState();
		update();
	}
}

void cHySpexVNIR_3000N_PropertyPage_Remote::sendChangedData(bool* pNeedsUpdate)
{
	if (!mConnected)
		return;

	bool needs_update = false;

	auto frames = mpAvgFrames->text().toInt();
	auto frame_period_us = mpFramePeriod_us->text().toInt();
	auto integration_time_us = mpIntegrationTime_us->text().toInt();

	if ((mDefaultAverageFrames != frames)
		|| (mDefaultFramePeriod_us != frame_period_us)
		|| (mDefaultIntegrationTime_us != integration_time_us))
	{
		sendAcquisitionParameters(frames, frame_period_us, integration_time_us);
		needs_update = true;
	}

	auto num_backgrounds = mpNumBackgrounds->text().toInt();

	if (mDefaultNumBackgrounds != num_backgrounds)
	{
		sendNumOfBackgrounds(num_backgrounds);
		needs_update = true;
	}

	if (pNeedsUpdate)
		*pNeedsUpdate = needs_update;
}

void cHySpexVNIR_3000N_PropertyPage_Remote::decodeIncomingData(const void* pBuffer, std::size_t buf_length)
{
	cHySpexVNIR_3000N_PropertiesNetDecoder::decode(pBuffer, buf_length);
}

int cHySpexVNIR_3000N_PropertyPage_Remote::sendOutgoingData(const char* data, std::size_t len)
{
	return cSensorPropertyPageRemoteInterface::sendOutgoingData(data, len);
}

