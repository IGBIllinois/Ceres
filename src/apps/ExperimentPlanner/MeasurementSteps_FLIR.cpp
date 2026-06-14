
#include "MeasurementSteps_FLIR.hpp"
#include "ExperimentDesignItems.hpp"

#include "DelayStepInfoDlg.hpp"
#include "MovementStepInfoDlg.hpp"

#include "Constants.hpp"
#include "StringUtils.hpp"

#include <QLayout>
#include <QPushButton>
#include <QGridLayout>
#include <QComboBox>
#include <QLabel>
#include <QDialogButtonBox>
#include <QMessageBox>
#include <QMenu>

#include <string>

namespace fs = std::filesystem;

std::shared_ptr<cMeasurementStep> flir::create_step(const std::string& type, const nlohmann::json& info)
{
	if (type == "flir")
	{
		if (info.contains("command"))
		{
			std::string command = info["command"];

			if (command == "configure")
				return std::make_shared<cMeasurementStep_FLIR_Configure>();

			if ((command == "take_photo") || (command == "take photo"))
				return std::make_shared<cMeasurementStep_FLIR_TakePhoto>();
		}
	}

	return std::shared_ptr<cMeasurementStep>();
}


/********************************************************************
 *
 * Measurement Step: FLIR Camera Configure Command
 *
 ********************************************************************/

cMeasurementStep_FLIR_Configure::cMeasurementStep_FLIR_Configure()
{
	mMode = eMode::TIME_LAPSE;
}

cBaseStep* cMeasurementStep_FLIR_Configure::graphicsItem(const int id, eExperimentType exp_type) const
{
	auto step = new cProcessStep(id, exp_type);

	QString title = QString::fromStdString("FLIR");

	step->setTitle(title);

	auto description = generateDescription();
	step->setSubHeading1(description);

	return step;
}

bool cMeasurementStep_FLIR_Configure::onEdit()
{
	return false;
}

void cMeasurementStep_FLIR_Configure::load(const nlohmann::json& jdoc)
{
	using namespace nlohmann;

	if (jdoc.contains("mode"))
	{
		auto mode = jdoc["mode"];
		if (nStringUtils::iequal(mode, "photo"))
		{
			mMode = eMode::SINGLE;
		}
		else if (nStringUtils::iequal(mode, "time lapse") || nStringUtils::iequal(mode, "time-lapse"))
		{
			mMode = eMode::TIME_LAPSE;
		}
		else if (nStringUtils::iequal(mode, "video") || nStringUtils::iequal(mode, "continuous"))
		{
			mMode = eMode::CONTINUOUS;
		}
	}

	if (jdoc.contains("frame rate (hz)"))
	{
		mFrameRate_fps = jdoc["frame rate (hz)"].get<double>();
	}

	int32_t lapse_interval_ms = -1;

	if (jdoc.contains("time lapse interval (s)"))
		lapse_interval_ms = static_cast<int32_t>(jdoc["time lapse interval (s)"].get<float>() * 1000.0);
	else if (jdoc.contains("time lapse interval (ms)"))
		lapse_interval_ms = jdoc["time lapse interval (ms)"].get<int32_t>();

	else if (jdoc.contains("time-lapse interval (s)"))
		lapse_interval_ms = static_cast<int32_t>(jdoc["time-lapse interval (s)"].get<float>() * 1000.0);
	else if (jdoc.contains("time-lapse interval (ms)"))
		lapse_interval_ms = jdoc["time-lapse interval (ms)"].get<int32_t>();

	else if (jdoc.contains("lapse interval (s)"))
		lapse_interval_ms = static_cast<int32_t>(jdoc["lapse interval (s)"].get<float>() * 1000.0);
	else if (jdoc.contains("lapse interval (ms)"))
		lapse_interval_ms = jdoc["lapse interval (ms)"].get<int32_t>();

	else if (jdoc.contains("interval (s)"))
		lapse_interval_ms = static_cast<int32_t>(jdoc["interval (s)"].get<float>() * 1000.0);
	else if (jdoc.contains("interval (ms)"))
		lapse_interval_ms = jdoc["interval (ms)"].get<int32_t>();

	if (lapse_interval_ms > 0)
	{
		mLapseInterval_ms = lapse_interval_ms;
	}
}

nlohmann::json cMeasurementStep_FLIR_Configure::save()
{
	nlohmann::json entry;

	entry["type"] = "flir";

	entry["command"] = "configure";



	mDirty = false;

	return entry;
}


QString cMeasurementStep_FLIR_Configure::generateDescription() const
{
	return "Configure";

	return QString();
}


/********************************************************************
 *
 * Measurement Step: FLIR Camera Take Photo Command
 *
 ********************************************************************/

cMeasurementStep_FLIR_TakePhoto::cMeasurementStep_FLIR_TakePhoto()
{}

cBaseStep* cMeasurementStep_FLIR_TakePhoto::graphicsItem(const int id, eExperimentType exp_type) const
{
	auto step = new cProcessStep(id, exp_type);

	QString title = "Take Photo";

	step->setTitle(title);

	auto description = generateDescription();
	step->setSubHeading1(description);

	return step;
}

void cMeasurementStep_FLIR_TakePhoto::load(const nlohmann::json& jdoc)
{
	using namespace nlohmann;
}

nlohmann::json cMeasurementStep_FLIR_TakePhoto::save()
{
	nlohmann::json entry;

	entry["type"] = "flir";

	entry["command"] = "take_photo";

	mDirty = false;

	return entry;

	mDirty = false;

	return entry;
}


QString cMeasurementStep_FLIR_TakePhoto::generateDescription() const
{
	return "Trigger taking a single FLIR image.";
}



