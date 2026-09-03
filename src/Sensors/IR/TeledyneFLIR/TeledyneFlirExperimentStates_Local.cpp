
#include "TeledyneFlirExperimentStates_Local.hpp"
#include "TeledyneFlirCameraModel.hpp"
#include "StringUtils.hpp"

#include <QMessageBox>
#include <QThread>

#include <chrono>
#include <thread>


/*******************************************************************/
/**  Base Class for Teledyne FLIR Local Experiment States  **/
/*******************************************************************/
cTeledyneFlirCameraExperimentState_Local::cTeledyneFlirCameraExperimentState_Local(cTeledyneFlirCameraModel* pModel, QObject* parent)
	: QObject(parent), mpModel(pModel)
{}

cTeledyneFlirCameraExperimentState_Local::~cTeledyneFlirCameraExperimentState_Local()
{
}

bool cTeledyneFlirCameraExperimentState_Local::initialize()
{
	return true;
}

void cTeledyneFlirCameraExperimentState_Local::cleanup()
{}


/**************************************************************************/
/**    Teledyne FLIR Experiment State to Save the State of the Camera    **/
/**************************************************************************/

cTeledyneFlirCamera_SaveState_Local::cTeledyneFlirCamera_SaveState_Local(QObject* parent)
	: cTeledyneFlirCameraExperimentState_Local(nullptr, parent)
{}

QString cTeledyneFlirCamera_SaveState_Local::getStatusStr()
{
	return "Saving current camera state...";
}

void cTeledyneFlirCamera_SaveState_Local::run()
{
	if (mResult == cExperimentState::eRESULT::WAITING)
		emit requestSaveState();

	mResult = cExperimentState::eRESULT::DONE;
}

void cTeledyneFlirCamera_SaveState_Local::pause() {}
void cTeledyneFlirCamera_SaveState_Local::stop() {}

cExperimentState::eRESULT cTeledyneFlirCamera_SaveState_Local::finished() { return mResult; }


/*****************************************************************************/
/**    Teledyne FLIR Experiment State to Restore the State of the Camera    **/
/*****************************************************************************/

cTeledyneFlirCamera_RestoreState_Local::cTeledyneFlirCamera_RestoreState_Local(QObject* parent)
	: cTeledyneFlirCameraExperimentState_Local(nullptr, parent)
{}

QString cTeledyneFlirCamera_RestoreState_Local::getStatusStr()
{
	return "Restoring camera state...";
}

void cTeledyneFlirCamera_RestoreState_Local::run()
{
	if (mResult == cExperimentState::eRESULT::WAITING)
		emit requestRestoreState();

	mResult = cExperimentState::eRESULT::DONE;
}

void cTeledyneFlirCamera_RestoreState_Local::pause() {}
void cTeledyneFlirCamera_RestoreState_Local::stop() {}

cExperimentState::eRESULT cTeledyneFlirCamera_RestoreState_Local::finished() { return mResult; }


/*******************************************************************/
/**    Teledyne FLIR Experiment States to Configure Camera        **/
/*******************************************************************/

cTeledyneFlirCamera_Configure_Local::cTeledyneFlirCamera_Configure_Local(cTeledyneFlirCameraModel* pModel, QObject* parent)
	: cTeledyneFlirCameraExperimentState_Local(pModel, parent)
{
}

QString cTeledyneFlirCamera_Configure_Local::getStatusStr()
{
	return "Configuring FLIR camera...";
}

bool cTeledyneFlirCamera_Configure_Local::configure(const nlohmann::json& stateDoc)
{
	return cTeledyneFlirExperimentHelper_Configure::configure(stateDoc);
}

void cTeledyneFlirCamera_Configure_Local::run()
{
	if (mWaitingForConfiguration)
	{
		if (mWaitingForMode)
			emit requestMode(mMode);

		if (mWaitingForFrameRate)
			emit requestFrameRate_Hz(mFrameRate_fps);

		if (mWaitingForInterval)
			emit requestLapseInterval_ms(mLapseInterval_ms);

		mWaitingForConfiguration = false;
	}
}

void cTeledyneFlirCamera_Configure_Local::pause() {}
void cTeledyneFlirCamera_Configure_Local::stop() {}

cExperimentState::eRESULT cTeledyneFlirCamera_Configure_Local::finished()
{
	if (mWaitingForMode || mWaitingForFrameRate || mWaitingForInterval)
		return cExperimentState::eRESULT::WAITING;

	return cExperimentState::eRESULT::DONE;
}

void cTeledyneFlirCamera_Configure_Local::modeChanged(int mode)
{
	mWaitingForMode = false;
}

void cTeledyneFlirCamera_Configure_Local::lapseIntervalChanged(int interval_ms)
{
	mWaitingForInterval = false;
}

void cTeledyneFlirCamera_Configure_Local::frameRateChanged(double rate_fps)
{
	mWaitingForFrameRate = false;
}


/*******************************************************************/
/**       Teledyne FLIR Experiment States to Take Photo           **/
/*******************************************************************/

cTeledyneFlirCamera_TakePhoto_Local::cTeledyneFlirCamera_TakePhoto_Local(cTeledyneFlirCameraModel* pModel, QObject* parent)
	: cTeledyneFlirCameraExperimentState_Local(pModel, parent)
{
}

QString cTeledyneFlirCamera_TakePhoto_Local::getStatusStr()
{
	if (mUpdateView)
		return "Taking Photo and updating view...";

	return "Taking Photo...";
}

bool cTeledyneFlirCamera_TakePhoto_Local::configure(const nlohmann::json& stateDoc)
{
	return cTeledyneFlirExperimentHelper_TakePhoto::configure(stateDoc);
}

void cTeledyneFlirCamera_TakePhoto_Local::run() 
{
	if (mTriggerImage)
	{
		if (mpModel->mode() == cTeledyneFlirCameraModel::eMode::SINGLE)
		{
			emit takePhoto(mUpdateView, true);
		}
		else
		{
			if (mUpdateView)
				emit updateView();

//			mResult = cExperimentState::eRESULT::DONE;
		}

		mTriggerImage = false;
	}
}

void cTeledyneFlirCamera_TakePhoto_Local::pause() {}
void cTeledyneFlirCamera_TakePhoto_Local::stop() {}

cExperimentState::eRESULT cTeledyneFlirCamera_TakePhoto_Local::finished()
{
	return mResult;
}

void cTeledyneFlirCamera_TakePhoto_Local::onPhotoTaken()
{
	--mNumOfPhotos;

	if (mNumOfPhotos < 1)
		mResult = cExperimentState::eRESULT::DONE;
	else
	{
		mTriggerImage = true;
	}
};



