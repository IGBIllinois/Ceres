
#pragma once

#include "MeasurementSteps.hpp"

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
	std::shared_ptr<cMeasurementStep> create_step(const std::string& type, const nlohmann::json& info);
}

/// <summary>
/// Measurement Step: Command sent to HySpex Camera
/// 
/// For example: command could be "open shutter", "close shutter", or "background"
/// </summary>
class cMeasurementStep_HySpex_Command : public cMeasurementStep
{
	Q_OBJECT

public:
	cMeasurementStep_HySpex_Command() = delete;
	cMeasurementStep_HySpex_Command(std::string_view camera_model, std::string_view command);

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

