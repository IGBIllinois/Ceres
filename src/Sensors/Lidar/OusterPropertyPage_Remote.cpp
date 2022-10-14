

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

void cOusterPropertyPage_Remote::onAzimuthWindow(double min_deg, double max_deg)
{
	mpMinAzimuthAngle_deg->setText(QString::number(min_deg));
	mpMaxAzimuthAngle_deg->setText(QString::number(max_deg));

	mDefaultMinAzimuthAngle_deg = min_deg;
	mDefaultMaxAzimuthAngle_deg = max_deg;
}

void cOusterPropertyPage_Remote::onLidarMode(const std::string& mode)
{
	QString qMode = QString::fromStdString(mode);

	auto n = mpLidarModes->count();
	for (int i = 0; i < n; ++i)
	{
		auto data = mpLidarModes->itemText(i);
		if (0 == data.compare(qMode))
		{
			mDefaultLidarMode = qMode;
			mpLidarModes->setCurrentIndex(i);
			break;
		}
	}
}

void cOusterPropertyPage_Remote::onCurrentState(bool valid, const std::string& mode,
	double min_deg, double max_deg)
{
	if (!valid) return;

	onLidarMode(mode);
	onAzimuthWindow(min_deg, max_deg);
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

	if ((mDefaultMinAzimuthAngle_deg != min_deg) ||
		(mDefaultMaxAzimuthAngle_deg != max_deg))
	{
		sendSetAzimuthWindow(min_deg, max_deg);
//		mDefaultMinAzimuthAngle_deg = min_deg;
//		mDefaultMaxAzimuthAngle_deg = max_deg;
	}

	auto mode = mpLidarModes->currentText();
	if (mode.compare(mDefaultLidarMode) != 0)
	{
		sendSetLidarMode(mode.toStdString());
//		mDefaultLidarMode = mode;
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

