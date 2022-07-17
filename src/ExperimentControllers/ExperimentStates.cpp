
#include "ExperimentStates.hpp"
#include "PauseExperimentStateDlg.hpp"
#include "../Utilities/Constants.hpp"

#include <QApplication>


/********************************************************************
 * Delay Step
 *******************************************************************/
cExperimentState_Delay::cExperimentState_Delay()
	: mElapsedTime_sec(0), mWaitTime_sec(0)
{
}


void cExperimentState_Delay::configure(const nlohmann::json& stateDoc)
{
	mWaitTime_sec = 0;

	if (stateDoc.contains("wait (sec)"))
		mWaitTime_sec += stateDoc["wait (sec)"];

	if (stateDoc.contains("wait (min)"))
		mWaitTime_sec += 60.0 * stateDoc["wait (min)"];

	if (stateDoc.contains("wait (hr)"))
		mWaitTime_sec += 3600.0 * stateDoc["wait (hr)"];
}

QString cExperimentState_Delay::getStatusStr()
{
	QString msg = "Delaying for ";
	msg.append(std::to_string(mWaitTime_sec).c_str());
	msg += " sec.";
	return msg;
}

bool cExperimentState_Delay::recording()
{
	return false;
}

void cExperimentState_Delay::initialize()
{
	mElapsedTime_sec = 0;
	mStart = std::chrono::steady_clock::now();
}

void cExperimentState_Delay::run()
{
	auto diff = std::chrono::steady_clock::now() - mStart;
	mElapsedTime_sec = std::chrono::duration_cast<std::chrono::seconds>(diff).count();
}

void cExperimentState_Delay::pause()
{
}

void cExperimentState_Delay::stop()
{
}

cExperimentState::eRESULT cExperimentState_Delay::finished()
{
	return mElapsedTime_sec >= mWaitTime_sec ? eRESULT::DONE : eRESULT::WAITING;
}


/********************************************************************
 * Pause Step
 *******************************************************************/
cExperimentState_Pause::cExperimentState_Pause()
	: mpDlg(nullptr)
{
}

cExperimentState_Pause::~cExperimentState_Pause()
{
	mpDlg->deleteLater();
}

void cExperimentState_Pause::configure(const nlohmann::json& stateDoc)
{
	// We are in the data thread.  Any GUI object must exists in the
	// QApplication thread!
	mpDlg = new cPauseExperimentStateDlg();
	mpDlg->moveToThread(QApplication::instance()->thread());
	QObject::connect(this, &cExperimentState_Pause::showDlg, mpDlg, &cPauseExperimentStateDlg::showDlg);
}

QString cExperimentState_Pause::getStatusStr()
{
	return QString("Waiting for user...");
}

bool cExperimentState_Pause::recording()
{
	return false;
}

void cExperimentState_Pause::initialize()
{
	emit showDlg();
}

void cExperimentState_Pause::run()
{
}

void cExperimentState_Pause::pause()
{
}

void cExperimentState_Pause::stop()
{
}

cExperimentState::eRESULT cExperimentState_Pause::finished()
{
	auto result = mpDlg->result();

	if (result == cPauseExperimentStateDlg::eRESULT::NONE)
		return eRESULT::WAITING;

	return result == cPauseExperimentStateDlg::eRESULT::ABORT ? eRESULT::ABORT : eRESULT::DONE;
}

