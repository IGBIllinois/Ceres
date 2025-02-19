
#include "ExperimentSteps_Ssnx.hpp"
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

std::shared_ptr<cExperimentStep> ssnx::create_step(const std::string& type, const nlohmann::json& info)
{
	if (type == "ssnx")
	{
		if (info.contains("command"))
		{
			std::string command = info["command"];

			if (command == "reference")
				return std::make_shared<cExperimentStep_Ssnx_Command>("reference");
		}
	}

	return std::shared_ptr<cExperimentStep>();
}


/********************************************************************
 *
 * Experiment Step: Ssnx GPS Command
 *
 ********************************************************************/

cExperimentStep_Ssnx_Command::cExperimentStep_Ssnx_Command(std::string_view command)
{
	mCommand = command;
}

cBaseStep* cExperimentStep_Ssnx_Command::graphicsItem(const int id) const
{
	auto step = new cProcessStep(id);

	step->setTitle("ssnx");

	auto description = generateDescription();
	step->setSubHeading1(description);

	return step;
}

void cExperimentStep_Ssnx_Command::load(const nlohmann::json& jdoc)
{
	using namespace nlohmann;
}

nlohmann::json cExperimentStep_Ssnx_Command::save()
{
	nlohmann::json entry;

	entry["type"] = "ssnx";

	entry["command"] = mCommand;

	mDirty = false;

	return entry;
}


QString cExperimentStep_Ssnx_Command::generateDescription() const
{
	if (mCommand == "reference")
		return "Collect a GPS reference point for later processing.";

	return QString();
}


