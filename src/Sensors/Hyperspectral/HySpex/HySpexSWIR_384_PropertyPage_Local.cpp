

#include "HySpexSWIR_384_PropertyPage_Local.hpp"
#include "HySpexExperimentStates_Local.hpp"

#include <QLineEdit>
#include <QComboBox>
#include <QTimer>
#include <QTime>
#include <QCoreApplication>
#include <QMessageBox>
#include <QPushButton>
#include <QDialogButtonBox>


cHySpexSWIR_384_PropertyPage_Local::cHySpexSWIR_384_PropertyPage_Local(QWidget* parent)
	: cHySpexSWIR_384_PropertyPage(parent)
{
}

cExperimentState* cHySpexSWIR_384_PropertyPage_Local::createState(const std::string& type, const nlohmann::json& entry, QObject* parent)
{
/*
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
*/

	return nullptr;
}

void cHySpexSWIR_384_PropertyPage_Local::createWidgets()
{
	cHySpexSWIR_384_PropertyPage::createWidgets();
}

void cHySpexSWIR_384_PropertyPage_Local::enableControls(bool enable)
{
	cHySpexSWIR_384_PropertyPage::enableControls(enable);

	auto* ok = mpButtons->button(QDialogButtonBox::Ok);
	auto* apply = mpButtons->button(QDialogButtonBox::Apply);

	if (ok) ok->setEnabled(enable);
	if (apply) apply->setEnabled(enable);
}

void cHySpexSWIR_384_PropertyPage_Local::buttonClicked(QAbstractButton* button)
{
	cHySpexSWIR_384_PropertyPage::buttonClicked(button);
}


void cHySpexSWIR_384_PropertyPage_Local::showPage()
{
	cHySpexSWIR_384_PropertyPage::showPage();
}

void cHySpexSWIR_384_PropertyPage_Local::doCalcBackground()
{
}

void cHySpexSWIR_384_PropertyPage_Local::doOK()
{
	cHySpexSWIR_384_PropertyPage::doOK();
}

void cHySpexSWIR_384_PropertyPage_Local::doCancel()
{
	cHySpexSWIR_384_PropertyPage::doCancel();
}

void cHySpexSWIR_384_PropertyPage_Local::doApply()
{
	enableControls(true);
	update();
}

void cHySpexSWIR_384_PropertyPage_Local::reject()
{
	doCancel();
}

