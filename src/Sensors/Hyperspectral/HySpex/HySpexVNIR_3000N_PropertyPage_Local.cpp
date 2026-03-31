

#include "HySpexVNIR_3000N_PropertyPage_Local.hpp"
#include "HySpexIDs.hpp"
#include "HySpexExperimentStates_Local.hpp"

#include "StringUtils.hpp"

#include <QLineEdit>
#include <QComboBox>
#include <QTimer>
#include <QTime>
#include <QCoreApplication>
#include <QMessageBox>
#include <QPushButton>
#include <QDialogButtonBox>


cHySpexVNIR_3000N_PropertyPage_Local::cHySpexVNIR_3000N_PropertyPage_Local(QWidget* parent)
	: cHySpexVNIR_3000N_PropertyPage(parent)
{}

cExperimentState* cHySpexVNIR_3000N_PropertyPage_Local::createState(const std::string& type, const nlohmann::json& entry, QObject* parent)
{
/*
	if (nStringUtils::iequal(type, vnir_3000N_id))
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


void cHySpexVNIR_3000N_PropertyPage_Local::createWidgets()
{
	cHySpexVNIR_3000N_PropertyPage::createWidgets();
}

void cHySpexVNIR_3000N_PropertyPage_Local::enableControls(bool enable)
{
	cHySpexVNIR_3000N_PropertyPage::enableControls(enable);

	auto* ok = mpButtons->button(QDialogButtonBox::Ok);
	//	auto* cancel = mpButtons->button(QDialogButtonBox::Cancel);
	auto* apply = mpButtons->button(QDialogButtonBox::Apply);

	if (ok) ok->setEnabled(enable);
	//	if (cancel) cancel->setEnabled(enable);
	if (apply) apply->setEnabled(enable);
}

void cHySpexVNIR_3000N_PropertyPage_Local::buttonClicked(QAbstractButton* button)
{
	cHySpexVNIR_3000N_PropertyPage::buttonClicked(button);
}


void cHySpexVNIR_3000N_PropertyPage_Local::showPage()
{
	cHySpexVNIR_3000N_PropertyPage::showPage();
}

void cHySpexVNIR_3000N_PropertyPage_Local::doCalcBackground()
{
	enableControls(false);
	update();
}

void cHySpexVNIR_3000N_PropertyPage_Local::doOK()
{
	cHySpexVNIR_3000N_PropertyPage::doOK();
}

void cHySpexVNIR_3000N_PropertyPage_Local::doCancel()
{
	cHySpexVNIR_3000N_PropertyPage::doCancel();
}

void cHySpexVNIR_3000N_PropertyPage_Local::doApply()
{
	enableControls(true);
	update();
}

void cHySpexVNIR_3000N_PropertyPage_Local::reject()
{
	doCancel();
}

