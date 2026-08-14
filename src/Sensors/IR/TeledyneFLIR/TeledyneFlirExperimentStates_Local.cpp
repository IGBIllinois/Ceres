
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
	using namespace nlohmann;

	try
	{
		if (stateDoc.contains("mode"))
		{
			auto mode = stateDoc["mode"];
			if (nStringUtils::iequal(mode, "photo"))
			{
				mMode = static_cast<int>(cTeledyneFlirCameraModel::eMode::SINGLE);
			}
			else if (nStringUtils::iequal(mode, "time lapse") || nStringUtils::iequal(mode, "time-lapse"))
			{
				mMode = static_cast<int>(cTeledyneFlirCameraModel::eMode::TIME_LAPSE);
			}
			else if (nStringUtils::iequal(mode, "video") || nStringUtils::iequal(mode, "continuous"))
			{
				mMode = static_cast<int>(cTeledyneFlirCameraModel::eMode::CONTINUOUS);
			}

			mWaitingForMode = mMode != static_cast<int>(mpModel->mode());
		}

		if (stateDoc.contains("frame rate (hz)"))
		{
			mFrameRate_fps = stateDoc["frame rate (hz)"].get<double>();

			mWaitingForFrameRate = mFrameRate_fps != mpModel->frameRate_Hz();
		}

		int32_t lapse_interval_ms = -1;

		if (stateDoc.contains("time lapse interval (s)"))
			lapse_interval_ms = static_cast<int32_t>(stateDoc["time lapse interval (s)"].get<float>() * 1000.0);
		else if (stateDoc.contains("time lapse interval (ms)"))
			lapse_interval_ms = stateDoc["time lapse interval (ms)"].get<int32_t>();

		else if (stateDoc.contains("time-lapse interval (s)"))
			lapse_interval_ms = static_cast<int32_t>(stateDoc["time-lapse interval (s)"].get<float>() * 1000.0);
		else if (stateDoc.contains("time-lapse interval (ms)"))
			lapse_interval_ms = stateDoc["time-lapse interval (ms)"].get<int32_t>();

		else if (stateDoc.contains("lapse interval (s)"))
			lapse_interval_ms = static_cast<int32_t>(stateDoc["lapse interval (s)"].get<float>() * 1000.0);
		else if (stateDoc.contains("lapse interval (ms)"))
			lapse_interval_ms = stateDoc["lapse interval (ms)"].get<int32_t>();

		else if (stateDoc.contains("interval (s)"))
			lapse_interval_ms = static_cast<int32_t>(stateDoc["interval (s)"].get<float>() * 1000.0);
		else if (stateDoc.contains("interval (ms)"))
			lapse_interval_ms = stateDoc["interval (ms)"].get<int32_t>();

		if (lapse_interval_ms > 0)
		{
			mLapseInterval_ms = lapse_interval_ms;

			mWaitingForInterval = mLapseInterval_ms != mpModel->lapseInterval_ms();
		}
	}
	catch (const detail::parse_error& e)
	{
		QString msg = "Parse Error: ";
		msg += e.what();

		QMessageBox mb(QMessageBox::Critical, "Teledyne FLIR Experiment State Error", msg);
		mb.exec();

		return false;
	}
	catch (const detail::type_error& e)
	{
		QString msg = "Type Error: ";
		msg += e.what();

		QMessageBox mb(QMessageBox::Critical, "Teledyne FLIR Experiment State Error", msg);
		mb.exec();

		return false;
	}
	catch (const detail::exception& e)
	{
		QString msg = "Unknown Error: ";
		msg += e.what();

		QMessageBox mb(QMessageBox::Critical, "Teledyne FLIR Experiment State Error", msg);
		mb.exec();

		return false;
	}

	return true;
}

void cTeledyneFlirCamera_Configure_Local::run()
{
	if (mUpdateConfiguration)
	{
		if (mWaitingForMode)
			emit requestMode(mMode);

		if (mWaitingForFrameRate)
			emit requestFrameRate_Hz(mFrameRate_fps);

		if (mWaitingForInterval)
			emit requestLapseInterval_ms(mLapseInterval_ms);

		mUpdateConfiguration = false;
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
	using namespace nlohmann;

	mUpdateView = false;

	try
	{
		if (stateDoc.contains("update view"))
		{
			mUpdateView = stateDoc["update view"];
		}
	}
	catch (const detail::parse_error& e)
	{
		QString msg = "Parse Error: ";
		msg += e.what();

		QMessageBox mb(QMessageBox::Critical, "Teledyne FLIR Experiment State Error", msg);
		mb.exec();

		return false;
	}
	catch (const detail::type_error& e)
	{
		QString msg = "Type Error: ";
		msg += e.what();

		QMessageBox mb(QMessageBox::Critical, "Teledyne FLIR Experiment State Error", msg);
		mb.exec();

		return false;
	}
	catch (const detail::exception& e)
	{
		QString msg = "Unknown Error: ";
		msg += e.what();

		QMessageBox mb(QMessageBox::Critical, "Teledyne FLIR Experiment State Error", msg);
		mb.exec();

		return false;
	}

	return true;
}

void cTeledyneFlirCamera_TakePhoto_Local::run() 
{
	if (mTriggerPhoto)
	{
		if (mpModel->mode() == cTeledyneFlirCameraModel::eMode::SINGLE)
		{
			emit takePhoto(mUpdateView, true);
		}
		else
		{
			if (mUpdateView)
				emit updateView();

			mResult = cExperimentState::eRESULT::DONE;
		}

		mTriggerPhoto = false;
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
	mResult = cExperimentState::eRESULT::DONE;
};



