

#include "OusterPropertyPage_Remote.hpp"

#include <QLineEdit>
#include <QComboBox>

cOusterPropertyPage_Remote::cOusterPropertyPage_Remote(QWidget* parent)
	: cOusterPropertyPage(parent), cSensorPropertyPageRemoteInterface(parent),
		cOusterPropertiesNetEncoder(255)
{}

void cOusterPropertyPage_Remote::onConnect()
{
	cOusterPropertiesNetEncoder::sendQueryState();
}

void cOusterPropertyPage_Remote::onCurrentState(bool valid, const std::string& mode,
	double min_deg, double max_deg)
{
	if (!valid) return;

	mModeDefault = QString::fromStdString(mode);
	mMinAzimuthAngleDefault_deg = min_deg;
	mMaxAzimuthAngleDefault_deg = max_deg;

	auto n = mpModes->count();
	for (int i = 0; i < n; ++i)
	{
		auto data = mpModes->itemText(i);
		if (0 == data.compare(mModeDefault))
		{
			mpModes->setCurrentIndex(i);
			break;
		}
	}

	mpMinAzimuthAngle_deg->setText(QString::number(min_deg));
	mpMaxAzimuthAngle_deg->setText(QString::number(max_deg));
}

void cOusterPropertyPage_Remote::showPage()
{
	openConnection();
	cOusterPropertyPage::showPage();
}

void cOusterPropertyPage_Remote::doOK()
{
	doApply();
	closeConnection();
	cOusterPropertyPage::doOK();
}

void cOusterPropertyPage_Remote::doCancel()
{
	closeConnection();
	cOusterPropertyPage::doCancel();
}

void cOusterPropertyPage_Remote::doApply()
{
	if (!mConnected)
		return;

	auto min_deg = mpMinAzimuthAngle_deg->text().toDouble();
	auto max_deg = mpMaxAzimuthAngle_deg->text().toDouble();

	if ((mMinAzimuthAngleDefault_deg != min_deg) ||
		(mMaxAzimuthAngleDefault_deg != max_deg))
	{
		sendSetAzimuthWindow(min_deg, max_deg);
		mMinAzimuthAngleDefault_deg = min_deg;
		mMaxAzimuthAngleDefault_deg = max_deg;
	}


	auto mode = mpModes->currentText();
	if (mode.compare(mModeDefault) != 0)
	{
		sendSetMode(mode.toStdString());
		mModeDefault = mode;
	}
}

void cOusterPropertyPage_Remote::decodeIncomingData(const void* pBuffer, std::size_t buf_length)
{
	cOusterPropertiesNetDecoder::decode(pBuffer, buf_length);
}

int cOusterPropertyPage_Remote::sendOutgoingData(const char* data, std::size_t len)
{
	return cSensorPropertyPageRemoteInterface::sendOutgoingData(data, len);
}

