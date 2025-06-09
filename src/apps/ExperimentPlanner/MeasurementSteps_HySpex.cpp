
#include "MeasurementSteps_HySpex.hpp"
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

std::shared_ptr<cMeasurementStep> hyspex::create_step(const std::string& type, const nlohmann::json& info)
{
	if (type == "VNIR-3000N")
	{
		if (info.contains("command"))
		{
			std::string command = info["command"];

			if (command == "open shutter")
				return std::make_shared<cMeasurementStep_HySpex_Command>("VNIR-3000N", "open shutter");

			if (command == "close shutter")
				return std::make_shared<cMeasurementStep_HySpex_Command>("VNIR-3000N", "close shutter");

			if (command == "background")
				return std::make_shared<cMeasurementStep_HySpex_Command>("VNIR-3000N", "background");
		}
	}

	if (type == "SWIR-384")
	{
		if (info.contains("command"))
		{
			std::string command = info["command"];

			if (command == "open shutter")
				return std::make_shared<cMeasurementStep_HySpex_Command>("SWIR-384", "open shutter");

			if (command == "close shutter")
				return std::make_shared<cMeasurementStep_HySpex_Command>("SWIR-384", "close shutter");

			if (command == "background")
				return std::make_shared<cMeasurementStep_HySpex_Command>("SWIR-384", "background");
		}
	}

	return std::shared_ptr<cMeasurementStep>();
}


/********************************************************************
 *
 * Measurement Step: HySpex Camera Command
 *
 ********************************************************************/

cMeasurementStep_HySpex_Command::cMeasurementStep_HySpex_Command(std::string_view camera_model, std::string_view command)
{
	mModel = camera_model;
	mCommand = command;
}

cBaseStep* cMeasurementStep_HySpex_Command::graphicsItem(const int id) const
{
	auto step = new cProcessStep(id);

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

void cMeasurementStep_HySpex_Command::load(const nlohmann::json& jdoc)
{
	using namespace nlohmann;
}

nlohmann::json cMeasurementStep_HySpex_Command::save()
{
	nlohmann::json entry;

	entry["type"] = mModel;

	entry["command"] = mCommand;

	mDirty = false;

	return entry;
}


QString cMeasurementStep_HySpex_Command::generateDescription() const
{
	if (mCommand == "open shutter")
		return "Wait for the shutter to open.";

	if (mCommand == "close shutter")
		return "Wait for the shutter to close.";

	if (mCommand == "background")
		return "Collect a background image for camera processing.";

	return QString();
}


