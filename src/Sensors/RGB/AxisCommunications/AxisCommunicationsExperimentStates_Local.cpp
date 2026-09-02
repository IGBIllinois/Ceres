
#include "AxisCommunicationsExperimentStates_Local.hpp"
#include "AxisCommunicationsModel.hpp"
#include "StringUtils.hpp"

#include <QMessageBox>
#include <QThread>

#include <chrono>
#include <thread>


/***************************************************************************/
/**       Base Class for Axis Communications Local Experiment States      **/
/***************************************************************************/
cAxisCommunicationsExperimentState_Local::cAxisCommunicationsExperimentState_Local(cAxisCommunicationsModel* pModel, QObject* parent)
	: QObject(parent), mpModel(pModel)
{}

cAxisCommunicationsExperimentState_Local::~cAxisCommunicationsExperimentState_Local()
{
}

bool cAxisCommunicationsExperimentState_Local::initialize()
{
	return true;
}

void cAxisCommunicationsExperimentState_Local::cleanup()
{}


/**************************************************************************/
/** Axis Communications Experiment State to Save the State of the Camera **/
/**************************************************************************/

cAxisCommunications_SaveState_Local::cAxisCommunications_SaveState_Local(QObject* parent)
	: cAxisCommunicationsExperimentState_Local(nullptr, parent)
{}

QString cAxisCommunications_SaveState_Local::getStatusStr()
{
	return "Saving current camera state...";
}

void cAxisCommunications_SaveState_Local::run()
{
	if (mResult == cExperimentState::eRESULT::WAITING)
		emit requestSaveState();

	mResult = cExperimentState::eRESULT::DONE;
}

void cAxisCommunications_SaveState_Local::pause() {}
void cAxisCommunications_SaveState_Local::stop() {}

cExperimentState::eRESULT cAxisCommunications_SaveState_Local::finished() { return mResult; }


/*****************************************************************************/
/** Axis Communications Experiment State to Restore the State of the Camera **/
/*****************************************************************************/

cAxisCommunications_RestoreState_Local::cAxisCommunications_RestoreState_Local(QObject* parent)
	: cAxisCommunicationsExperimentState_Local(nullptr, parent)
{}

QString cAxisCommunications_RestoreState_Local::getStatusStr()
{
	return "Restoring camera state...";
}

void cAxisCommunications_RestoreState_Local::run()
{
	if (mResult == cExperimentState::eRESULT::WAITING)
		emit requestRestoreState();

	mResult = cExperimentState::eRESULT::DONE;
}

void cAxisCommunications_RestoreState_Local::pause() {}
void cAxisCommunications_RestoreState_Local::stop() {}

cExperimentState::eRESULT cAxisCommunications_RestoreState_Local::finished() { return mResult; }


/***********************************************************************/
/**   Axis Communications Experiment States to Configure Camera       **/
/***********************************************************************/

cAxisCommunications_Configure_Local::cAxisCommunications_Configure_Local(cAxisCommunicationsModel* pModel, QObject* parent)
	: cAxisCommunicationsExperimentState_Local(pModel, parent)
{
}

QString cAxisCommunications_Configure_Local::getStatusStr()
{
	return "Configuring Axis Communications camera...";
}

bool cAxisCommunications_Configure_Local::configure(const nlohmann::json& stateDoc)
{
	return cAxisCommunicationsExperimentHelper_Configure::configure(stateDoc);
}

void cAxisCommunications_Configure_Local::run()
{
	if (mUpdateConfiguration)
	{
		if (mWaitingForCameraID)
			emit requestCameraID(mCameraID);

		if (mWaitingForResolution)
			emit requestImageSize(mImageWidth, mImageHeight);

		if (mWaitingForFrameRate)
			emit requestFrameRate_Hz(mFrameRate_fps);

		if (mWaitingForMode)
			emit requestMode(mMode);

		if (mWaitingForInterval)
			emit requestLapseInterval_ms(mLapseInterval_ms);

		mUpdateConfiguration = false;
	}
}

void cAxisCommunications_Configure_Local::pause() {}
void cAxisCommunications_Configure_Local::stop() {}

cExperimentState::eRESULT cAxisCommunications_Configure_Local::finished()
{
	if (mWaitingForMode || mWaitingForCameraID || mWaitingForResolution || mWaitingForFrameRate || mWaitingForInterval)
		return cExperimentState::eRESULT::WAITING;

	return cExperimentState::eRESULT::DONE;
}

void cAxisCommunications_Configure_Local::onModeChange(int mode)
{
	mWaitingForMode = false;
}

void cAxisCommunications_Configure_Local::onCameraIdChange(int modeid)
{
	mWaitingForCameraID = false;
}

void cAxisCommunications_Configure_Local::onLapseIntervalChange(int interval_ms)
{
	mWaitingForInterval = false;
}

void cAxisCommunications_Configure_Local::onFrameRateChange(int rate_fps)
{
	mWaitingForFrameRate = false;
}

void cAxisCommunications_Configure_Local::onImageSizeChange(int width, int height)
{
	mWaitingForResolution = false;
}


/***********************************************************************/
/**      Axis Communications Experiment States to Take Photo          **/
/***********************************************************************/

cAxisCommunications_TakePhoto_Local::cAxisCommunications_TakePhoto_Local(cAxisCommunicationsModel* pModel, QObject* parent)
	: cAxisCommunicationsExperimentState_Local(pModel, parent)
{
}

QString cAxisCommunications_TakePhoto_Local::getStatusStr()
{
	if (mUpdateView)
		return "Taking Photo and updating view...";

	return "Taking Photo...";
}

bool cAxisCommunications_TakePhoto_Local::configure(const nlohmann::json& stateDoc)
{
	return cAxisCommunicationsExperimentHelper_TakePhoto::configure(stateDoc);
}

void cAxisCommunications_TakePhoto_Local::run()
{
	if (mTriggerPhoto)
	{
		if (mpModel->mode() == cAxisCommunicationsModel::eMode::SINGLE)
		{
			emit takePhoto(mUpdateView);
		}
		else
		{
			if (mUpdateView)
				emit updateView();

//			mResult = cExperimentState::eRESULT::DONE;
		}

		mTriggerPhoto = false;
	}
}

void cAxisCommunications_TakePhoto_Local::pause() {}
void cAxisCommunications_TakePhoto_Local::stop() {}

cExperimentState::eRESULT cAxisCommunications_TakePhoto_Local::finished()
{
	return mResult;
}

void cAxisCommunications_TakePhoto_Local::onPhotoTaken()
{
	--mNumOfPhotos;
	if (mNumOfPhotos < 1)
		mResult = cExperimentState::eRESULT::DONE;
	else
	{
		mTriggerPhoto = true;
	}
};



