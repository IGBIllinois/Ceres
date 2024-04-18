
#pragma once

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


class cExerimentStep
{
public:
	cExerimentStep() = default;
	virtual ~cExerimentStep();

	bool isDirty() const;

	virtual cConnectedItem* graphicsItem() const = 0;

protected:
	virtual void load(const nlohmann::json& jdoc) = 0;
	virtual nlohmann::json save() = 0;

protected:
	bool mDirty = false;

	friend class cExperimentFile;
};

/// <summary>
/// Experiment Step: Simple Time Delay
/// </summary>
class cExerimentStep_Delay : public QObject, public cExerimentStep
{
	Q_OBJECT

public:
	cExerimentStep_Delay() = default;

	cConnectedItem* graphicsItem() const override;

signals:
	void onDescriptionChange(const QString& desc);
	void onCommentChange(const QString& comment);

protected:
	void load(const nlohmann::json& jdoc) override;
	nlohmann::json save() override;

private slots:
	void onEdit();

private:
	QString generateDescription() const;
	QString generateComment() const;

private:
	double mWaitTime_sec = 0.0;
	std::optional<int> mWaitTime_min;
	std::optional<int> mWaitTime_hr;
	bool mRecording = false;
};

/// <summary>
/// Experiment Step: Pause for User Input
/// </summary>
class cExerimentStep_Pause : public QObject, public cExerimentStep
{
	Q_OBJECT

public:
	cExerimentStep_Pause() = default;

	cConnectedItem* graphicsItem() const override;

protected:
	void load(const nlohmann::json& jdoc) override;
	nlohmann::json save() override;
};

/// <summary>
/// Experiment Step: Controls SpiderCam Movement
/// </summary>
class cExerimentStep_Movement : public QObject, public cExerimentStep
{
	Q_OBJECT

public:
	cExerimentStep_Movement() = default;

	cConnectedItem* graphicsItem() const override;

signals:
	void onMovementTextChange(const QString& desc);
	void onOrientationTextChange(const QString& desc);
	void onCommentChange(const QString& comment);

protected:
	void load(const nlohmann::json& jdoc) override;
	nlohmann::json save() override;

private slots:
	void onEdit();

private:
	QString generateMovementDescription() const;
	QString generateOrientationDescription() const;
	QString generateComment() const;

private:
	bool mRecording = false;

	std::optional<int> mX_mm;
	std::optional<int> mY_mm;
	std::optional<int> mZ_mm;

	int mSpeed_mmps = 0;

	std::optional<double> mPan_deg;
	std::optional<double> mTilt_deg;
	std::optional<double> mRoll_deg;
};


