
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
	using namespace nlohmann;

	try
	{
		if (stateDoc.contains("mode"))
		{
			auto mode = stateDoc["mode"];
			if (nStringUtils::iequal(mode, "photo"))
			{
				mMode = static_cast<int>(cAxisCommunicationsModel::eMode::SINGLE);
			}
			else if (nStringUtils::iequal(mode, "time lapse") || nStringUtils::iequal(mode, "time-lapse"))
			{
				mMode = static_cast<int>(cAxisCommunicationsModel::eMode::TIME_LAPSE);
			}
			else if (nStringUtils::iequal(mode, "video") || nStringUtils::iequal(mode, "continuous"))
			{
				mMode = static_cast<int>(cAxisCommunicationsModel::eMode::CONTINUOUS);
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

		QMessageBox mb(QMessageBox::Critical, "Axis Communications Experiment State Error", msg);
		mb.exec();

		return false;
	}
	catch (const detail::type_error& e)
	{
		QString msg = "Type Error: ";
		msg += e.what();

		QMessageBox mb(QMessageBox::Critical, "Axis Communications Experiment State Error", msg);
		mb.exec();

		return false;
	}
	catch (const detail::exception& e)
	{
		QString msg = "Unknown Error: ";
		msg += e.what();

		QMessageBox mb(QMessageBox::Critical, "Axis Communications Experiment State Error", msg);
		mb.exec();

		return false;
	}

	return true;
}

void cAxisCommunications_Configure_Local::run()
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

void cAxisCommunications_Configure_Local::pause() {}
void cAxisCommunications_Configure_Local::stop() {}

cExperimentState::eRESULT cAxisCommunications_Configure_Local::finished()
{
	if (mWaitingForMode || mWaitingForFrameRate || mWaitingForInterval)
		return cExperimentState::eRESULT::WAITING;

	return cExperimentState::eRESULT::DONE;
}

void cAxisCommunications_Configure_Local::modeChanged(int mode)
{
	mWaitingForMode = false;
}

void cAxisCommunications_Configure_Local::lapseIntervalChanged(int interval_ms)
{
	mWaitingForInterval = false;
}

void cAxisCommunications_Configure_Local::frameRateChanged(double rate_fps)
{
	mWaitingForFrameRate = false;
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

		QMessageBox mb(QMessageBox::Critical, "Axis Communications Experiment State Error", msg);
		mb.exec();

		return false;
	}
	catch (const detail::type_error& e)
	{
		QString msg = "Type Error: ";
		msg += e.what();

		QMessageBox mb(QMessageBox::Critical, "Axis Communications Experiment State Error", msg);
		mb.exec();

		return false;
	}
	catch (const detail::exception& e)
	{
		QString msg = "Unknown Error: ";
		msg += e.what();

		QMessageBox mb(QMessageBox::Critical, "Axis Communications Experiment State Error", msg);
		mb.exec();

		return false;
	}

	return true;
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

			mResult = cExperimentState::eRESULT::DONE;
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
	mResult = cExperimentState::eRESULT::DONE;
};



