
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
class cMeasurementStep;
enum class eExperimentType;

namespace basic
{
	std::shared_ptr<cMeasurementStep> create_step(const std::string& type);
}


class cMeasurementStep : public QObject
{
	Q_OBJECT

public:
	cMeasurementStep() = default;
	virtual ~cMeasurementStep();

	bool isDirty() const;

	virtual cBaseStep* graphicsItem(const int id, eExperimentType exp_type) const = 0;
//	virtual cConnectedItem* graphicsItem(const int id) const = 0;

signals:
	void redraw();

protected:
	virtual void load(const nlohmann::json& jdoc) = 0;
	virtual nlohmann::json save() = 0;

protected:
	bool mDirty = false;

	friend class cExperimentFile;
};

/// <summary>
/// Measurement Include: Include addition measure steps
/// </summary>
class cMeasurementStep_Include : public cMeasurementStep
{
	Q_OBJECT

public:
	cMeasurementStep_Include() = default;

	std::string getIncludeFilename() const;

	bool isRecording() const { return false; }

	void setIncludeFilename(const std::string& filename);

	void setDefaultPath(const std::string& path);

	cBaseStep* graphicsItem(const int id, eExperimentType exp_type) const override;

signals:
	void onDescriptionChange(const QString& desc);

public slots:
	bool onEdit();

protected:
	void load(const nlohmann::json& jdoc) override;
	nlohmann::json save() override;

private:
	QString generateDescription() const;

private:
	std::string mDefaultPath;
	std::string mFilename;
};

/// <summary>
/// Measurement Step: Simple Time Delay
/// </summary>
class cMeasurementStep_Delay : public cMeasurementStep
{
	Q_OBJECT

public:
	cMeasurementStep_Delay() = default;

	double getWaitTime_sec() const;
	const std::optional<int>& getWaitTime_min() const;
	const std::optional<int>& getWaitTime_hr() const;

	bool isRecording() const;

	void setWaitTime_sec(double sec);
	void setWaitTime_min(const std::optional<int>& min);
	void setWaitTime_hr(const std::optional<int>& hr);

	void setRecording(bool recording);

	cBaseStep* graphicsItem(const int id, eExperimentType exp_type) const override;

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
/// Measurement Step: Pause for User Input
/// </summary>
class cMeasurementStep_Pause : public cMeasurementStep
{
	Q_OBJECT

public:
	cMeasurementStep_Pause() = default;

	cBaseStep* graphicsItem(const int id, eExperimentType exp_type) const override;

protected:
	void load(const nlohmann::json& jdoc) override;
	nlohmann::json save() override;
};

/// <summary>
/// Measurement Step: Controls SpiderCam Movement
/// </summary>
class cMeasurementStep_Movement : public cMeasurementStep
{
	Q_OBJECT

public:
	cMeasurementStep_Movement() = default;

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

	cBaseStep* graphicsItem(const int id, eExperimentType exp_type) const override;

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

/// <summary>
/// Measurement Step: Set Marker
/// </summary>
class cMeasurementStep_Marker : public cMeasurementStep
{
	Q_OBJECT

public:
	enum class eMarkerType {CUSTOM, START_OF_MEASUREMENT, END_OF_MEASUREMENT};

public:
	cMeasurementStep_Marker() = default;

	const eMarkerType getMarkerType() const;
	const std::string getMarkerLabel() const;

	void setMarkerType(eMarkerType marker_type);
	void setMarkerLabel(const std::string& marker_label);

	cBaseStep* graphicsItem(const int id, eExperimentType exp_type) const override;

signals:
	void onMarkerTypeTextChange(const QString& desc);

public slots:
	bool onEdit();

private:
	QString generateDescription() const;

protected:
	void load(const nlohmann::json& jdoc) override;
	nlohmann::json save() override;

private:
	eMarkerType mMarkerType = eMarkerType::CUSTOM;
	std::string mMarkerLabel;
};



