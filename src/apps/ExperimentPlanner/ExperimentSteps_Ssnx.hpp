
#pragma once

#include "ExperimentSteps.hpp"

#include <nlohmann/json.hpp>

#include <QObject>
#include <QDialog>

#include <filesystem>
#include <optional>

// Qt Forward Declaration
QT_BEGIN_NAMESPACE
class QButton;
QT_END_NAMESPACE

// Forward Declarations
class cExperimentFile;
class cConnectedItem;
class cBaseStep;


namespace ssnx
{
	std::shared_ptr<cExperimentStep> create_step(const std::string& type, const nlohmann::json& info);
}

/// <summary>
/// Experiment Step: Command sent to Ssnx GPS Unit
/// 
/// For example: command could be "reference"
/// </summary>
class cExperimentStep_Ssnx_Command : public cExperimentStep
{
	Q_OBJECT

public:
	cExperimentStep_Ssnx_Command() = delete;
	cExperimentStep_Ssnx_Command(std::string_view command);

	cBaseStep* graphicsItem(const int id) const override;

protected:
	void load(const nlohmann::json& jdoc) override;
	nlohmann::json save() override;

private:
	QString generateDescription() const;

private:
	std::string mCommand;
};

