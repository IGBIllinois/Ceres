

#include "HySpexSWIR_384_PropertyPage_Remote.hpp"

#include <QLineEdit>
#include <QComboBox>

cHySpexSWIR_384_PropertyPage_Remote::cHySpexSWIR_384_PropertyPage_Remote(QWidget* parent)
	: cHySpexSWIR_384_PropertyPage(parent), cSensorPropertyPageRemoteInterface(parent),
		cHySpexSWIR_384_PropertiesNetEncoder(255)
{}

void cHySpexSWIR_384_PropertyPage_Remote::onConnect()
{
	cHySpexSWIR_384_PropertiesNetEncoder::sendQueryState();
}

void cHySpexSWIR_384_PropertyPage_Remote::onCurrentState(bool valid)
{
	if (!valid) return;
}

void cHySpexSWIR_384_PropertyPage_Remote::showPage()
{
	openConnection();
	cHySpexSWIR_384_PropertyPage::showPage();
}

void cHySpexSWIR_384_PropertyPage_Remote::doOK()
{
	doApply();
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
}

void cHySpexSWIR_384_PropertyPage_Remote::decodeIncomingData(const void* pBuffer, std::size_t buf_length)
{
	cHySpexSWIR_384_PropertiesNetDecoder::decode(pBuffer, buf_length);
}

int cHySpexSWIR_384_PropertyPage_Remote::sendOutgoingData(const char* data, std::size_t len)
{
	return cSensorPropertyPageRemoteInterface::sendOutgoingData(data, len);
}

