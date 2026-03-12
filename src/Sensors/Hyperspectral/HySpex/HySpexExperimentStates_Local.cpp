
#include "HySpexExperimentStates_Local.hpp"

#include <QMessageBox>
#include <QThread>

#include <chrono>
#include <thread>

/*******************************************************************/
/**       Base Class for Remote HySpex Experiment States          **/
/*******************************************************************/
cHySpexCamera_ExperimentState_Local::cHySpexCamera_ExperimentState_Local
	(QObject* parent)
{}

cHySpexCamera_ExperimentState_Local::~cHySpexCamera_ExperimentState_Local()
{}

bool cHySpexCamera_ExperimentState_Local::initialize()
{
	return true;
}

void cHySpexCamera_ExperimentState_Local::cleanup()
{}

bool cHySpexCamera_ExperimentState_Local::recording()
{
	return false;
}


/*******************************************************************/
/**         HySpex Experiment States to Control Shutter           **/
/*******************************************************************/
cHySpexCamera_ShutterCtrl_Local::cHySpexCamera_ShutterCtrl_Local(eShutterState desired_state, QObject* parent)
	:
	mDesiredState(desired_state)
{
//	mShutterTimer.interval_sec(5);
//	mShutterTimer.stop();
}

bool cHySpexCamera_ShutterCtrl_Local::configure(const nlohmann::json& stateDoc)
{
	return true;
}

void cHySpexCamera_ShutterCtrl_Local::run()
{
//	if (mShutterTimer.elapsed())
	{
//		std::this_thread::sleep_for(std::chrono::milliseconds(NETWORK_DELAY_MS));

//		mShutterTimer.reset();
	}
}

void cHySpexCamera_ShutterCtrl_Local::pause() {}
void cHySpexCamera_ShutterCtrl_Local::stop() {}

cExperimentState::eRESULT cHySpexCamera_ShutterCtrl_Local::finished()
{
	//if (mShutterState == eShutterState::ERROR)
	//	return eRESULT::ABORT;

	//if (mShutterState == mDesiredState)
	//	return eRESULT::DONE;

	return eRESULT::WAITING;
}


/*** Experimental State to Close Shutter ***/
cHySpexCamera_CloseShutter_Local::cHySpexCamera_CloseShutter_Local(QObject* parent)
	:
	cHySpexCamera_ShutterCtrl_Local(eShutterState::CLOSED, parent)
{}

QString cHySpexCamera_CloseShutter_Local::getStatusStr()
{
	return "Closing Shutter...";
}

/*** Experimental State to Open Shutter ***/
cHySpexCamera_OpenShutter_Local::cHySpexCamera_OpenShutter_Local(QObject* parent)
	:
	cHySpexCamera_ShutterCtrl_Local(eShutterState::OPEN, parent)
{}

QString cHySpexCamera_OpenShutter_Local::getStatusStr()
{
	return "Opening Shutter...";
}


/*******************************************************************/
/**       HySpex Experiment States to Control Acquisition         **/
/*******************************************************************/
cHySpexCamera_Acquisition_Local::cHySpexCamera_Acquisition_Local(QObject* parent)
	: 
	cHySpexCamera_ExperimentState_Local(parent)
{}

cHySpexCamera_Acquisition_Local::~cHySpexCamera_Acquisition_Local()
{}

bool cHySpexCamera_Acquisition_Local::configure(const nlohmann::json& stateDoc)
{
	using namespace nlohmann;

	mHasAcquisitionState = false;

	try
	{
		if (stateDoc.contains("average frames"))
		{
			mDesiredAverageFrames = stateDoc["average frames"];
			mHasAcquisitionState = true;
		}

		if (stateDoc.contains("frame period (us)"))
		{
			mDesiredFramePeriod_us = stateDoc["frame period(us)"];
			mHasAcquisitionState = true;
		}

		if (stateDoc.contains("integration time (us)"))
		{
			mDesiredIntegrationTime_us = stateDoc["integration time (us)"];
			mHasAcquisitionState = true;
		}
	}
	catch (const detail::parse_error& e)
	{
		QString msg = "Parse Error: ";
		msg += e.what();

		QMessageBox mb(QMessageBox::Critical, "HySpex Experiment State Error", msg);
		mb.exec();

		return false;
	}
	catch (const detail::type_error& e)
	{
		QString msg = "Type Error: ";
		msg += e.what();

		QMessageBox mb(QMessageBox::Critical, "HySpex Experiment State Error", msg);
		mb.exec();

		return false;
	}
	catch (const detail::exception& e)
	{
		QString msg = "Unknown Error: ";
		msg += e.what();

		QMessageBox mb(QMessageBox::Critical, "HySpex Experiment State Error", msg);
		mb.exec();

		return false;
	}

	return true;
}

void cHySpexCamera_Acquisition_Local::run() {}
void cHySpexCamera_Acquisition_Local::pause() {}
void cHySpexCamera_Acquisition_Local::stop() {}


/*** Experimental State to Adjust Acquisition Parameters ***/
cHySpexCamera_AcqParameters_Local::cHySpexCamera_AcqParameters_Local(QObject* parent)
	:
	cHySpexCamera_Acquisition_Local(parent) 
{}

QString cHySpexCamera_AcqParameters_Local::getStatusStr()
{
	return "Updating acquisition parameters...";
}

bool cHySpexCamera_AcqParameters_Local::initialize()
{
	if (!mHasAcquisitionState)
	{
		return true;
	}

	return cHySpexCamera_Acquisition_Local::initialize();
}

cExperimentState::eRESULT cHySpexCamera_AcqParameters_Local::finished()
{
	if (!mHasAcquisitionState)
		return eRESULT::DONE;

	return eRESULT::WAITING;
}


/*** Experimental State to Do Background Measurement ***/
cHySpexCamera_Background_Local::cHySpexCamera_Background_Local(QObject* parent)
	:
	cHySpexCamera_Acquisition_Local(parent)
{}


bool cHySpexCamera_Background_Local::configure(const nlohmann::json& stateDoc)
{
	using namespace nlohmann;

	try
	{
		if (stateDoc.contains("number of backgrounds"))
		{
			mDesiredNumBackgrounds = static_cast<std::uint32_t>(stateDoc["number of backgrounds"]);
		}
	}
	catch (const detail::parse_error& e)
	{
		QString msg = "Parse Error: ";
		msg += e.what();

		QMessageBox mb(QMessageBox::Critical, "HySpex Experiment State Error", msg);
		mb.exec();

		return false;
	}
	catch (const detail::type_error& e)
	{
		QString msg = "Type Error: ";
		msg += e.what();

		QMessageBox mb(QMessageBox::Critical, "HySpex Experiment State Error", msg);
		mb.exec();

		return false;
	}
	catch (const detail::exception& e)
	{
		QString msg = "Unknown Error: ";
		msg += e.what();

		QMessageBox mb(QMessageBox::Critical, "HySpex Experiment State Error", msg);
		mb.exec();

		return false;
	}

	return cHySpexCamera_Acquisition_Local::configure(stateDoc);
}

QString cHySpexCamera_Background_Local::getStatusStr()
{
	QString msg;
	if (mHasAcquisitionState)
	{
		msg = "Updating acquisition parameters and collecting a background image...";
	}
	else
	{
		msg = "Waiting for camera to collect background image...";
	}
	return msg;
}

cExperimentState::eRESULT cHySpexCamera_Background_Local::finished()
{
	if (mState == eSTATE::ERROR)
		return eRESULT::ABORT;

	if (mState == eSTATE::COMPLETE)
		return eRESULT::DONE;

	return eRESULT::WAITING;
}


