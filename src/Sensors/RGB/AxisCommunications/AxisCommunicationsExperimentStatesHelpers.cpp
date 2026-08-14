
#include "AxisCommunicationsExperimentStatesHelpers.hpp"
#include "AxisCommunicationsModel.hpp"
#include "StringUtils.hpp"


#include <QString>
#include <QMessageBox>


/***********************************************************************/
/** Axis Communications Experiment State Helper to Configure Camera   **/
/***********************************************************************/

cAxisCommunicationsExperimentState_Configure::cAxisCommunicationsExperimentState_Configure()
{}

bool cAxisCommunicationsExperimentState_Configure::configure(const nlohmann::json& stateDoc)
{
	using namespace nlohmann;

	try
	{
		if (stateDoc.contains("id"))
		{
			mCameraID = stateDoc["id"];
			mWaitingForCameraID = true;
		}

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

			mWaitingForMode = true;
		}

		if (stateDoc.contains("resolution"))
		{
			rgb::sImageSize_t image_size = axis::to_image_size(stateDoc["resolution"]);
			mImageWidth = image_size.width;
			mImageHeight = image_size.height;

			mWaitingForResolution = true;
		}


		if (stateDoc.contains("frame rate (hz)"))
		{
			mFrameRate_fps = stateDoc["frame rate (hz)"].get<double>();
			mWaitingForFrameRate = true;
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
			mWaitingForInterval = true;
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


/***********************************************************************/
/**   Axis Communications Experiment State Helper to Take Photo       **/
/***********************************************************************/

cAxisCommunicationsExperimentState_TakePhoto::cAxisCommunicationsExperimentState_TakePhoto()
{
}

bool cAxisCommunicationsExperimentState_TakePhoto::configure(const nlohmann::json& stateDoc)
{
	using namespace nlohmann;

	mUpdateView = false;

	try
	{
		if (stateDoc.contains("update view"))
		{
			mUpdateView = stateDoc["update view"];
		}

		if (stateDoc.contains("update_view"))
		{
			mUpdateView = stateDoc["update_view"];
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


