
#include "MeasurementSteps_FLIR.hpp"
#include "ExperimentDesignItems.hpp"

#include "DelayStepInfoDlg.hpp"
#include "MovementStepInfoDlg.hpp"

#include "Constants.hpp"

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
	if (type == "FLIR")
	{
		if (info.contains("command"))
		{
			std::string command = info["command"];

			if (command == "configure")
				return std::make_shared<cMeasurementStep_FLIR_Configure>("VNIR-3000N", "open shutter");

			if (command == "take_photo")
				return std::make_shared<cMeasurementStep_FLIR_TakePhoto>("VNIR-3000N", "close shutter");
		}
	}

	return std::shared_ptr<cMeasurementStep>();
}


/********************************************************************
 *
 * Measurement Step: FLIR Camera Configure Command
 *
 ********************************************************************/

cMeasurementStep_FLIR_Configure::cMeasurementStep_FLIR_Configure(std::string_view camera_model, std::string_view command)
{
	mModel = camera_model;
	mCommand = command;
}

cBaseStep* cMeasurementStep_FLIR_Configure::graphicsItem(const int id, eExperimentType exp_type) const
{
	auto step = new cProcessStep(id, exp_type);

	QString title = QString::fromStdString(mModel);

/*
	if (mCommand == "open shutter")
		title += " Open Shutter";

	if (mCommand == "close shutter")
		title += " Close Shutter";
*/

	step->setTitle(title);

	auto description = generateDescription();
	step->setSubHeading1(description);

	return step;
}

void cMeasurementStep_FLIR_Configure::load(const nlohmann::json& jdoc)
{
	using namespace nlohmann;
}

nlohmann::json cMeasurementStep_FLIR_Configure::save()
{
	nlohmann::json entry;

	entry["type"] = mModel;

	entry["command"] = mCommand;

	mDirty = false;

	return entry;
}


QString cMeasurementStep_FLIR_Configure::generateDescription() const
{
	if (mCommand == "open shutter")
		return "Wait for the shutter to open.";

	if (mCommand == "close shutter")
		return "Wait for the shutter to close.";

	if (mCommand == "background")
		return "Collect a background image for camera processing.";

	return QString();
}


/********************************************************************
 *
 * Measurement Step: FLIR Camera Take Photo Command
 *
 ********************************************************************/

cMeasurementStep_FLIR_TakePhoto::cMeasurementStep_FLIR_TakePhoto(std::string_view camera_model)
{
	mModel = camera_model;
}

cBaseStep* cMeasurementStep_FLIR_TakePhoto::graphicsItem(const int id, eExperimentType exp_type) const
{
	auto step = new cProcessStep(id, exp_type);

	QString title = QString::fromStdString(mModel);

	/*
		if (mCommand == "open shutter")
			title += " Open Shutter";

		if (mCommand == "close shutter")
			title += " Close Shutter";
	*/

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

	entry["type"] = mModel;

	mDirty = false;

	return entry;
}


QString cMeasurementStep_FLIR_TakePhoto::generateDescription() const
{
	if (mCommand == "open shutter")
		return "Wait for the shutter to open.";

	if (mCommand == "close shutter")
		return "Wait for the shutter to close.";

	if (mCommand == "background")
		return "Collect a background image for camera processing.";

	return QString();
}



