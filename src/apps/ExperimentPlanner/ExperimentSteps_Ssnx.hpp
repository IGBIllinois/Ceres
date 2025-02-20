
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
/// Experiment Step: Command sent to GPS to compute a reference point
/// </summary>
class cExperimentStep_ReferencePoint : public cExperimentStep
{
	Q_OBJECT

public:
	cExperimentStep_ReferencePoint() = default;

	double getMinIntegrationTime_sec() const;
	double getMaxIntegrationTime_sec() const;
	int	   getErrorThreshold_mm() const;

	void setMinIntegrationTime_sec(double sec);
	void setMaxIntegrationTime_sec(double sec);
	void setErrorThreshold_mm(int threshold_mm);

	cBaseStep* graphicsItem(const int id) const override;

signals:
	void onDescriptionChange(const QString& desc);
	void onCommentChange(const QString& comment);

public slots:
	bool onEdit();

protected:
	void load(const nlohmann::json& jdoc) override;
	nlohmann::json save() override;

private:
	QString generateDescription() const;
	QString generateComment() const;

private:
	double mMinIntegrationTime_sec = 5.0;
	double mMaxIntegrationTime_sec = 10.0;

	int mErrorThreshold_mm = 100;
};

