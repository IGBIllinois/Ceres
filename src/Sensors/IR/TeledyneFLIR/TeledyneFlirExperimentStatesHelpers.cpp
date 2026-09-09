
#include "TeledyneFlirExperimentStatesHelpers.hpp"
#include "TeledyneFlirCameraModel.hpp"
#include "StringUtils.hpp"


#include <QString>
#include <QMessageBox>


/***********************************************************************/
/** Axis Communications Experiment State Helper to Configure Camera   **/
/***********************************************************************/

cTeledyneFlirExperimentHelper_Configure::cTeledyneFlirExperimentHelper_Configure()
{}

bool cTeledyneFlirExperimentHelper_Configure::configure(const nlohmann::json& stateDoc)
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
		}

		if (stateDoc.contains("frame rate (hz)"))
		{
			mFrameRate_fps = stateDoc["frame rate (hz)"];
		}

		int32_t lapse_interval_ms = -1;

		if (stateDoc.contains("time-lapse interval (s)"))
			lapse_interval_ms = static_cast<int32_t>(stateDoc["time-lapse interval (s)"].get<float>() * 1000.0);
		else if (stateDoc.contains("time-lapse interval (ms)"))
			lapse_interval_ms = stateDoc["time-lapse interval (ms)"].get<int32_t>();

		else if (stateDoc.contains("time lapse interval (s)"))
			lapse_interval_ms = static_cast<int32_t>(stateDoc["time lapse interval (s)"].get<float>() * 1000.0);
		else if (stateDoc.contains("time lapse interval (ms)"))
			lapse_interval_ms = stateDoc["time lapse interval (ms)"].get<int32_t>();

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
		}
	}
	catch (const detail::parse_error& e)
	{
		QString msg = "Parse Error: ";
		msg += e.what();

		QMessageBox mb(QMessageBox::Critical, "Teledyne FLIR Configure Experiment State Error", msg);
		mb.exec();

		return false;
	}
	catch (const detail::type_error& e)
	{
		QString msg = "Type Error: ";
		msg += e.what();

		QMessageBox mb(QMessageBox::Critical, "Teledyne FLIR Configure Experiment State Error", msg);
		mb.exec();

		return false;
	}
	catch (const detail::exception& e)
	{
		QString msg = "Unknown Error: ";
		msg += e.what();

		QMessageBox mb(QMessageBox::Critical, "Teledyne FLIR Configure Experiment State Error", msg);
		mb.exec();

		return false;
	}

	return true;
}


/***********************************************************************/
/**      Teledyne FLIR Experiment State Helper to Take Photo          **/
/***********************************************************************/

cTeledyneFlirExperimentHelper_TakePhoto::cTeledyneFlirExperimentHelper_TakePhoto()
{
}

bool cTeledyneFlirExperimentHelper_TakePhoto::configure(const nlohmann::json& stateDoc)
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

		if (stateDoc.contains("number of photos"))
		{
			mNumOfPhotos = stateDoc["number of photos"];
		}

		if (stateDoc.contains("number_of_photos"))
		{
			mNumOfPhotos = stateDoc["number_of_photos"];
		}
	}
	catch (const detail::parse_error& e)
	{
		QString msg = "Parse Error: ";
		msg += e.what();

		QMessageBox mb(QMessageBox::Critical, "Teledyne FLIR Take Photo Experiment State Error", msg);
		mb.exec();

		return false;
	}
	catch (const detail::type_error& e)
	{
		QString msg = "Type Error: ";
		msg += e.what();

		QMessageBox mb(QMessageBox::Critical, "Teledyne FLIR Take Photo Experiment State Error", msg);
		mb.exec();

		return false;
	}
	catch (const detail::exception& e)
	{
		QString msg = "Unknown Error: ";
		msg += e.what();

		QMessageBox mb(QMessageBox::Critical, "Teledyne FLIR Take Photo Experiment State Error", msg);
		mb.exec();

		return false;
	}

	return true;
}

QString cTeledyneFlirExperimentHelper_TakePhoto::getStatusMessage()
{
	if (mNumOfPhotos > 1)
	{
		QString msg;

		if (mUpdateView)
		{
			msg = "Taking ";
			msg += QString::number(mNumOfPhotos);
			msg += " Photos and updating view...";
			return msg;
		}

		msg = "Taking ";
		msg += QString::number(mNumOfPhotos);
		msg += " Photos...";
		return msg;
	}

	if (mUpdateView)
		return "Taking Photo and updating view...";

	return "Taking Photo...";
}

