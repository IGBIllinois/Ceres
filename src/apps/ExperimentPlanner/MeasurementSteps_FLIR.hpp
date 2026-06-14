
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


namespace flir
{
	std::shared_ptr<cMeasurementStep> create_step(const std::string& type, const nlohmann::json& info);
}

/// <summary>
/// Measurement Step: Configures the FLIR Camera
/// 
/// For example: command could be "open shutter", "close shutter", or "background"
/// </summary>
class cMeasurementStep_FLIR_Configure : public cMeasurementStep
{
	Q_OBJECT

public:
	cMeasurementStep_FLIR_Configure() = delete;
	cMeasurementStep_FLIR_Configure(std::string_view camera_model, std::string_view command);

	cBaseStep* graphicsItem(const int id, eExperimentType exp_type) const override;

protected:
	void load(const nlohmann::json& jdoc) override;
	nlohmann::json save() override;

private:
	QString generateDescription() const;

private:
	std::string mModel;
	std::string mCommand;
};


/// <summary>
/// Measurement Step: Trigger the FLIR Camera to take a photo
/// 
/// </summary>
class cMeasurementStep_FLIR_TakePhoto : public cMeasurementStep
{
	Q_OBJECT

public:
	cMeasurementStep_FLIR_TakePhoto() = delete;
	cMeasurementStep_FLIR_TakePhoto(std::string_view camera_model);

	cBaseStep* graphicsItem(const int id, eExperimentType exp_type) const override;

protected:
	void load(const nlohmann::json& jdoc) override;
	nlohmann::json save() override;

private:
	QString generateDescription() const;

private:
	std::string mModel;
};

