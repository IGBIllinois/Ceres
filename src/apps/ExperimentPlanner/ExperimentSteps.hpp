
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
class cBaseStep;


class cExperimentStep
{
public:
	cExperimentStep() = default;
	virtual ~cExperimentStep();

	bool isDirty() const;

	virtual cBaseStep* graphicsItem(const int id) const = 0;
//	virtual cConnectedItem* graphicsItem(const int id) const = 0;

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
class cExperimentStep_Delay : public QObject, public cExperimentStep
{
	Q_OBJECT

public:
	cExperimentStep_Delay() = default;

	double getWaitTime_sec() const;
	const std::optional<int>& getWaitTime_min() const;
	const std::optional<int>& getWaitTime_hr() const;

	bool isRecording() const;

	void setWaitTime_sec(double sec);
	void setWaitTime_min(const std::optional<int>& min);
	void setWaitTime_hr(const std::optional<int>& hr);

	void setRecording(bool recording);

	cBaseStep* graphicsItem(const int id) const override;
//	cConnectedItem* graphicsItem(const int id) const override;

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
	double mWaitTime_sec = 0.0;
	std::optional<int> mWaitTime_min;
	std::optional<int> mWaitTime_hr;
	bool mRecording = false;
};

/// <summary>
/// Experiment Step: Pause for User Input
/// </summary>
class cExperimentStep_Pause : public QObject, public cExperimentStep
{
	Q_OBJECT

public:
	cExperimentStep_Pause() = default;

	cBaseStep* graphicsItem(const int id) const override;
//	cConnectedItem* graphicsItem(const int id) const override;

protected:
	void load(const nlohmann::json& jdoc) override;
	nlohmann::json save() override;
};

/// <summary>
/// Experiment Step: Controls SpiderCam Movement
/// </summary>
class cExperimentStep_Movement : public QObject, public cExperimentStep
{
	Q_OBJECT

public:
	cExperimentStep_Movement() = default;

	const std::optional<int>& getX_mm() const;
	const std::optional<int>& getY_mm() const;
	const std::optional<int>& getZ_mm() const;

	int getSpeed_mmps() const;

	const std::optional<double>& getPan_deg() const;
	const std::optional<double>& getTilt_deg() const;
	const std::optional<double>& getRoll_deg() const;

	bool isRecording() const;

	void setX_mm(const std::optional<int>& x_mm);
	void setY_mm(const std::optional<int>& y_mm);
	void setZ_mm(const std::optional<int>& z_mm);

	void setSpeed_mmps(int speed_mmps);

	void setPan_deg(const std::optional<double>& pan_deg);
	void setTilt_deg(const std::optional<double>& tilt_deg);
	void setRoll_deg(const std::optional<double>& roll_deg);

	void setRecording(bool recording);

	cBaseStep* graphicsItem(const int id) const override;
//	cConnectedItem* graphicsItem(const int id) const override;

signals:
	void onMovementTextChange(const QString& desc);
	void onOrientationTextChange(const QString& desc);
	void onCommentChange(const QString& comment);

public slots:
	bool onEdit();

protected:
	void load(const nlohmann::json& jdoc) override;
	nlohmann::json save() override;

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

	bool mUsingMeters = false;
	bool mUsingMps = false;
};


