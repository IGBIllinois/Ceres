

#include "OusterPropertyPage_Remote.hpp"

#include <QLineEdit>
#include <QComboBox>

cOusterPropertyPage_Remote::cOusterPropertyPage_Remote(QWidget* parent)
	: cOusterPropertyPage(parent), cSensorPropertyPageRemoteInterface(parent),
		cOusterPropertiesNetEncoder(255)
{}

bool cOusterPropertyPage_Remote::queryState()
{
	cOusterPropertiesNetEncoder::sendQueryState();
	return true;
}

void cOusterPropertyPage_Remote::onCurrentState(bool valid, const std::string& mode,
	double min_deg, double max_deg)
{
	if (!valid) return;

	auto n = mpModes->count();
	for (int i = 0; i < n; ++i)
	{
		auto data = mpModes->itemText(i);
		if (0 == data.compare(QString::fromStdString(mode)))
		{
			mpModes->setCurrentIndex(i);
			break;
		}
	}

	mpMinAzimuthAngle_deg->setText(QString::number(min_deg));
	mpMaxAzimuthAngle_deg->setText(QString::number(max_deg));
}

void cOusterPropertyPage_Remote::decodeIncomingData(const void* pBuffer, std::size_t buf_length)
{
	cOusterPropertiesNetDecoder::decode(pBuffer, buf_length);
}

int cOusterPropertyPage_Remote::sendOutgoingData(const char* data, std::size_t len)
{
	return cSensorPropertyPageRemoteInterface::sendOutgoingData(data, len);
}

