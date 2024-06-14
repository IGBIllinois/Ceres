
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


namespace hyspex
{
	std::shared_ptr<cExperimentStep> create_step(const std::string& type, const nlohmann::json& info);
}

/// <summary>
/// Experiment Step: Command sent to HySpex Camera
/// 
/// For example: command could be "open shutter", "close shutter", or "background"
/// </summary>
class cExperimentStep_HySpex_Command : public QObject, public cExperimentStep
{
	Q_OBJECT

public:
	cExperimentStep_HySpex_Command() = delete;
	cExperimentStep_HySpex_Command(std::string_view camera_model, std::string_view command);

	cBaseStep* graphicsItem(const int id) const override;

protected:
	void load(const nlohmann::json& jdoc) override;
	nlohmann::json save() override;

private:
	QString generateDescription() const;

private:
	std::string mModel;
	std::string mCommand;
};

