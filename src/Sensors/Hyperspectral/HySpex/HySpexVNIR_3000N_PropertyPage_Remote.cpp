

#include "HySpexVNIR_3000N_PropertyPage_Remote.hpp"

#include <QLineEdit>
#include <QComboBox>

cHySpexVNIR_3000N_PropertyPage_Remote::cHySpexVNIR_3000N_PropertyPage_Remote(QWidget* parent)
	: cHySpexVNIR_3000N_PropertyPage(parent), cSensorPropertyPageRemoteInterface(parent),
		cHySpexVNIR_3000N_PropertiesNetEncoder(255)
{}

void cHySpexVNIR_3000N_PropertyPage_Remote::onConnect()
{
	cHySpexVNIR_3000N_PropertiesNetEncoder::sendQueryState();
}

void cHySpexVNIR_3000N_PropertyPage_Remote::onCurrentState(bool valid)
{
	if (!valid) return;
}

void cHySpexVNIR_3000N_PropertyPage_Remote::showPage()
{
	openConnection();
	cHySpexVNIR_3000N_PropertyPage::showPage();
}

void cHySpexVNIR_3000N_PropertyPage_Remote::doOK()
{
	doApply();
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
}

void cHySpexVNIR_3000N_PropertyPage_Remote::decodeIncomingData(const void* pBuffer, std::size_t buf_length)
{
	cHySpexVNIR_3000N_PropertiesNetDecoder::decode(pBuffer, buf_length);
}

int cHySpexVNIR_3000N_PropertyPage_Remote::sendOutgoingData(const char* data, std::size_t len)
{
	return cSensorPropertyPageRemoteInterface::sendOutgoingData(data, len);
}

