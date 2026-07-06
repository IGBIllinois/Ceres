
#include "MeasurementSteps.hpp"
#include "ExperimentDesignItems.hpp"

#include "DelayStepInfoDlg.hpp"
#include "MovementStepInfoDlg.hpp"
#include "MarkerInfoDlg.hpp"

#include "Constants.hpp"

#include <QLayout>
#include <QPushButton>
#include <QGridLayout>
#include <QComboBox>
#include <QLabel>
#include <QDialogButtonBox>
#include <QMessageBox>
#include <QMenu>
#include <QFileDialog>

#include <string>


namespace fs = std::filesystem;

std::shared_ptr<cMeasurementStep> basic::create_step(const std::string& type)
{
	if (type == "include")
		return std::make_shared<cMeasurementStep_Include>();

	if (type == "delay")
		return std::make_shared<cMeasurementStep_Delay>();

	if (type == "pause")
		return std::make_shared<cMeasurementStep_Pause>();

	if (type == "movement")
		return std::make_shared<cMeasurementStep_Movement>();

	if (type == "marker")
		return std::make_shared<cMeasurementStep_Marker>();

	return std::shared_ptr<cMeasurementStep>();
}


/********************************************************************
 *
 * Measurement Step: Base Class
 *
 ********************************************************************/

cMeasurementStep::~cMeasurementStep()
{}

bool cMeasurementStep::isDirty() const
{
	return mDirty;
}



/********************************************************************
 *
 * Measurement Step: Include
 *
 ********************************************************************/

std::string cMeasurementStep_Include::getIncludeFilename() const { return mFilename; };

void cMeasurementStep_Include::setIncludeFilename(const std::string& filename)
{
	mFilename = filename;
}

void cMeasurementStep_Include::setDefaultPath(const std::string& path)
{
	mDefaultPath = path;
}

cBaseStep* cMeasurementStep_Include::graphicsItem(const int id, eExperimentType exp_type) const
{
	auto step = new cProcessStep(id, exp_type);
	connect(step, &cProcessStep::editStep, this, &cMeasurementStep_Include::onEdit);
	connect(this, &cMeasurementStep_Include::onDescriptionChange, step, &cProcessStep::setSubHeading1);

	step->setTitle("Include");

	auto description = generateDescription();
	step->setSubHeading1(description);

	return step;
}

bool cMeasurementStep_Include::onEdit()
{
	QString fileName = QFileDialog::getOpenFileName(nullptr, tr("Measurement Include File"), 
		QString::fromStdString(mFilename), "Measurement Files (*.json *.inc)");

	if (fileName.isEmpty())
		return false;

	mFilename = fileName.toStdString();

	emit redraw();

	return true;
}

void cMeasurementStep_Include::load(const nlohmann::json& jdoc)
{
	using namespace nlohmann;
	mFilename = jdoc["include"];
}

nlohmann::json cMeasurementStep_Include::save()
{
	nlohmann::json entry;

	entry["include"] = mFilename;

	mDirty = false;

	return entry;
}

QString cMeasurementStep_Include::generateDescription() const
{
	QString description = "Filename: ";

	description += QString::fromStdString(mFilename);

	return description;
}


/********************************************************************
 *
 * Measurement Step: Simple Delay
 *
 ********************************************************************/

double cMeasurementStep_Delay::getWaitTime_sec() const { return mWaitTime_sec; }
const std::optional<int>& cMeasurementStep_Delay::getWaitTime_min() const { return mWaitTime_min; }
const std::optional<int>& cMeasurementStep_Delay::getWaitTime_hr() const { return mWaitTime_hr; }

bool cMeasurementStep_Delay::isRecording() const { return mRecording; }

void cMeasurementStep_Delay::setWaitTime_sec(double sec)
{
	mDirty |= mWaitTime_sec != sec;
	mWaitTime_sec = sec;
}

void cMeasurementStep_Delay::setWaitTime_min(const std::optional<int>& min)
{
	mDirty |= mWaitTime_min != min;
	mWaitTime_min = min;
}

void cMeasurementStep_Delay::setWaitTime_hr(const std::optional<int>& hr)
{

	mDirty |= mWaitTime_hr != hr;
	mWaitTime_hr = hr;
}

void cMeasurementStep_Delay::setRecording(bool recording)
{
	mDirty |= mRecording != recording;
	mRecording = recording;
}

//cConnectedItem* cExperimentStep_Delay::graphicsItem(const int id) const
cBaseStep* cMeasurementStep_Delay::graphicsItem(const int id, eExperimentType exp_type) const
{
	auto step = new cProcessStep(id, exp_type);
	connect(step, &cProcessStep::editStep, this, &cMeasurementStep_Delay::onEdit);
	connect(this, &cMeasurementStep_Delay::onDescriptionChange, step, &cProcessStep::setSubHeading1);
	connect(this, &cMeasurementStep_Delay::onCommentChange, step, &cProcessStep::setSubHeading2);

	step->setTitle("Delay");

	auto description = generateDescription();
	step->setSubHeading1(description);

	auto comment = generateComment();
	step->setSubHeading2(comment);

	return step;
}

void cMeasurementStep_Delay::load(const nlohmann::json& jdoc)
{
	using namespace nlohmann;

	mWaitTime_sec = 0;

	if (jdoc.contains("wait (sec)"))
		mWaitTime_sec = jdoc["wait (sec)"];

	if (jdoc.contains("wait (min)"))
		mWaitTime_min = jdoc["wait (min)"];

	if (jdoc.contains("wait (hr)"))
		mWaitTime_hr = jdoc["wait (hr)"];

	if (jdoc.contains("record"))
		mRecording = jdoc["record"];
}

nlohmann::json cMeasurementStep_Delay::save()
{
	nlohmann::json entry;

	entry["type"] = "delay";

	if (mWaitTime_hr.has_value())
	{
		int hours = mWaitTime_hr.value();
		entry["wait (hr)"] = hours;
	}

	if (mWaitTime_min.has_value())
	{
		int minutes = mWaitTime_min.value();
		entry["wait (min)"] = minutes;
	}

	entry["wait (sec)"] = mWaitTime_sec;

	entry["record"] = mRecording;

	mDirty = false;

	return entry;
}

bool cMeasurementStep_Delay::onEdit()
{
	cDelayStepInfoDlg dlg;

	if (mWaitTime_hr.has_value())
		dlg.setHours(mWaitTime_hr.value());

	if (mWaitTime_min.has_value())
		dlg.setMinutes(mWaitTime_min.value());

	dlg.setSeconds(mWaitTime_sec);
	dlg.setRecording(mRecording);

	auto result = dlg.exec();

	if (result == QDialog::Rejected)
		return false;

	std::optional<int> hours;
	if (dlg.hasHours())
	{
		if (dlg.hours() > 0)
			hours = dlg.hours();
	}
	setWaitTime_hr(hours);

	std::optional<int> minutes;
	if (dlg.hasMinutes())
	{
		if (dlg.minutes() > 0)
			minutes = dlg.minutes();
	}
	setWaitTime_min(minutes);

	double seconds = dlg.seconds();
	setWaitTime_sec(seconds);

	bool record = dlg.recording();
	setRecording(record);

	auto description = generateDescription();
	emit onDescriptionChange(description);

	auto comment = generateComment();
	emit onCommentChange(comment);

	emit redraw();

	return true;
}

QString cMeasurementStep_Delay::generateDescription() const
{
	QString description = "Delay for ";

	if (mWaitTime_hr.has_value())
	{
		int hour = mWaitTime_hr.value();
		description += QString::number(hour);
		description += ":";

		if (mWaitTime_min.has_value())
		{
			int minutes = mWaitTime_min.value();
			if (minutes < 10)
			{
				description += "0";
			}
			description += QString::number(minutes);
			description += ":";
		}
		else
		{
			description += "00:";
		}

		description += QString::number(mWaitTime_sec);
	}
	else if (mWaitTime_min.has_value())
	{
		int minutes = mWaitTime_min.value();
		if (minutes < 10)
		{
			double time = minutes * 60.0 + mWaitTime_sec;
			description += QString::number(time);
			description += " sec";
		}
		else
		{
			description += QString::number(minutes);
			description += ":";
			description += QString::number(mWaitTime_sec);
		}
	}
	else
	{
		description += QString::number(mWaitTime_sec);
		description += " sec";
	}

	return description;
}

QString cMeasurementStep_Delay::generateComment() const
{
	if (mRecording)
		return QString("Recording: On");

	return QString();
}


/********************************************************************
 *
 * Measurement Step: Pause
 *
 ********************************************************************/

//cConnectedItem* cMeasurementStep_Pause::graphicsItem(const int id) const
cBaseStep* cMeasurementStep_Pause::graphicsItem(const int id, eExperimentType exp_type) const
{
	auto step = new cIoStep(id, exp_type);
	step->setReadOnly(true);
	step->setTitle("Wait for User OK to Advance...");

	return step;
}

void cMeasurementStep_Pause::load(const nlohmann::json& jdoc)
{
	using namespace nlohmann;
}

nlohmann::json cMeasurementStep_Pause::save()
{
	nlohmann::json entry;

	entry["type"] = "pause";

	mDirty = false;

	return entry;
}


/********************************************************************
 *
 * Measurement Step: SpiderCam Movement
 *
 ********************************************************************/

const std::optional<int>& cMeasurementStep_Movement::getX_mm() const { return mX_mm; }
const std::optional<int>& cMeasurementStep_Movement::getY_mm() const { return mY_mm; }
const std::optional<int>& cMeasurementStep_Movement::getZ_mm() const { return mZ_mm; }

const std::optional<int>& cMeasurementStep_Movement::getHeightAGL_mm() const { return mHeightAGL_mm; }
const std::optional<int>& cMeasurementStep_Movement::getHeightACL_mm() const { return mHeightACL_mm; }

int cMeasurementStep_Movement::getSpeed_mmps() const { return mSpeed_mmps; }

const std::optional<double>& cMeasurementStep_Movement::getPan_deg() const { return mPan_deg; }
const std::optional<double>& cMeasurementStep_Movement::getTilt_deg() const { return mTilt_deg; }
const std::optional<double>& cMeasurementStep_Movement::getRoll_deg() const { return mRoll_deg; }

bool cMeasurementStep_Movement::isRecording() const { return mRecording; }

void cMeasurementStep_Movement::setX_mm(const std::optional<int>& x_mm)
{
	mDirty |= mX_mm != x_mm;
	mX_mm = x_mm;
}

void cMeasurementStep_Movement::setY_mm(const std::optional<int>& y_mm)
{
	mDirty |= mY_mm != y_mm;
	mY_mm = y_mm;
}

void cMeasurementStep_Movement::setZ_mm(const std::optional<int>& z_mm)
{
	mDirty |= mZ_mm != z_mm;
	mZ_mm = z_mm;

	mHeightAGL_mm.reset();
	mHeightACL_mm.reset();
}

void cMeasurementStep_Movement::setHeightAGL_mm(const std::optional<int>& height_agl_mm)
{
	mDirty |= mHeightAGL_mm != height_agl_mm;
	mHeightAGL_mm = height_agl_mm;

	mZ_mm.reset();
	mHeightACL_mm.reset();
}

void cMeasurementStep_Movement::setHeightACL_mm(const std::optional<int>& height_acl_mm)
{
	mDirty |= mHeightACL_mm != height_acl_mm;
	mHeightACL_mm = height_acl_mm;

	mZ_mm.reset();
	mHeightAGL_mm.reset();
}

void cMeasurementStep_Movement::setSpeed_mmps(int speed_mmps)
{
	mDirty |= mSpeed_mmps != speed_mmps;
	mSpeed_mmps = speed_mmps;
}

void cMeasurementStep_Movement::setPan_deg(const std::optional<double>& pan_deg)
{
	mDirty |= mPan_deg != pan_deg;
	mPan_deg = pan_deg;
}

void cMeasurementStep_Movement::setTilt_deg(const std::optional<double>& tilt_deg)
{
	mDirty |= mTilt_deg != tilt_deg;
	mTilt_deg = tilt_deg;
}

void cMeasurementStep_Movement::setRoll_deg(const std::optional<double>& roll_deg)
{
	mDirty |= mRoll_deg != roll_deg;
	mRoll_deg = roll_deg;
}

void cMeasurementStep_Movement::setRecording(bool recording)
{
	mDirty |= mRecording != recording;
	mRecording = recording;
}

//cConnectedItem* cExperimentStep_Movement::graphicsItem(const int id) const
cBaseStep* cMeasurementStep_Movement::graphicsItem(const int id, eExperimentType exp_type) const
{
	auto step = new cProcessStep(id, exp_type);
	connect(step, &cProcessStep::editStep, this, &cMeasurementStep_Movement::onEdit);
	connect(this, &cMeasurementStep_Movement::onMovementTextChange, step, &cProcessStep::setSubHeading1);
	connect(this, &cMeasurementStep_Movement::onOrientationTextChange, step, &cProcessStep::setSubHeading2);
	connect(this, &cMeasurementStep_Movement::onCommentChange, step, &cProcessStep::setSubHeading3);

	step->setTitle("Movement");

	auto msg = generateMovementDescription();
	step->setSubHeading1(msg);

	msg = generateOrientationDescription();
	step->setSubHeading2(msg);

	msg = generateComment();
	step->setSubHeading3(msg);

	return step;
}

void cMeasurementStep_Movement::load(const nlohmann::json& jdoc)
{
	auto pos = jdoc["position"];

	if (pos.contains("x (mm)"))
	{
		mX_mm = static_cast<uint32_t>(pos["x (mm)"].get<int>());
	}
	else if (pos.contains("x (m)"))
	{
		mX_mm = static_cast<uint32_t>(pos["x (m)"].get<double>() * nConstants::M_TO_MM);
		mUsingMeters = true;
	}

	if (pos.contains("y (mm)"))
	{
		mY_mm = static_cast<uint32_t>(pos["y (mm)"].get<int>());
	}
	else if (pos.contains("y (m)"))
	{
		mY_mm = static_cast<uint32_t>(pos["y (m)"].get<double>() * nConstants::M_TO_MM);
		mUsingMeters = true;
	}

	if (pos.contains("z (mm)"))
	{
		mZ_mm = static_cast<uint32_t>(pos["z (mm)"].get<int>());
	}
	else if (pos.contains("z (m)"))
	{
		mZ_mm = static_cast<uint32_t>(pos["z (m)"].get<double>() * nConstants::M_TO_MM);
		mUsingMeters = true;
	}
	else if (pos.contains("height agl (mm)"))
	{
		mHeightAGL_mm = static_cast<uint32_t>(pos["height agl (mm)"].get<int>());
	}
	else if (pos.contains("height acl (mm)"))
	{
		mHeightACL_mm = static_cast<uint32_t>(pos["height acl (mm)"].get<int>());
	}


	if (jdoc.contains("speed (mm/s)"))
	{
		mSpeed_mmps = jdoc["speed (mm/s)"].get<int>();
	}
	else
	{
		mSpeed_mmps = static_cast<uint32_t>(jdoc["speed (m/s)"].get<double>() * nConstants::M_TO_MM);
		mUsingMps = true;
	}

	if (jdoc.contains("pan"))
	{
		mPan_deg = jdoc["pan"];
	}
	else if (jdoc.contains("pan (deg)"))
	{
		mPan_deg = jdoc["pan (deg)"];
	}

	if (jdoc.contains("tilt"))
	{
		mTilt_deg = jdoc["tilt"];
	}
	else if (jdoc.contains("tilt (deg)"))
	{
		mTilt_deg = jdoc["tilt (deg)"];
	}

	if (jdoc.contains("roll"))
	{
		mRoll_deg = jdoc["roll"];
	}
	else if (jdoc.contains("roll (deg)"))
	{
		mRoll_deg = jdoc["roll (deg)"];
	}

	mRecording = jdoc["record"];
}

nlohmann::json cMeasurementStep_Movement::save()
{
	nlohmann::json entry;

	entry["type"] = "movement";

	nlohmann::json pos;

	if (mX_mm.has_value())
	{
		if (mUsingMeters)
			pos["x (m)"] = mX_mm.value() * nConstants::MM_TO_M;
		else
			pos["x (mm)"] = mX_mm.value();
	}

	if (mY_mm.has_value())
	{
		if (mUsingMeters)
			pos["y (m)"] = mY_mm.value() * nConstants::MM_TO_M;
		else
			pos["y (mm)"] = mY_mm.value();
	}

	if (mZ_mm.has_value())
	{
		if (mUsingMeters)
			pos["z (m)"] = mZ_mm.value() * nConstants::MM_TO_M;
		else
			pos["z (mm)"] = mZ_mm.value();
	}
	else if (mHeightAGL_mm.has_value())
	{
		pos["height agl (mm)"] = mHeightAGL_mm.value();
	}
	else if (mHeightACL_mm.has_value())
	{
		 pos["height acl (mm)"] = mHeightACL_mm.value();
	}

	entry["position"] = pos;

	if (mUsingMps)
		entry["speed (m/s)"] = mSpeed_mmps * nConstants::MM_TO_M;
	else
		entry["speed (mm/s)"] = mSpeed_mmps;

	if (mPan_deg.has_value())
		entry["pan (deg)"] = mPan_deg.value();

	if (mTilt_deg.has_value())
		entry["tilt (deg)"] = mTilt_deg.value();

	if (mRoll_deg.has_value())
		entry["roll (deg)"] = mRoll_deg.value();

	entry["record"] = mRecording;

	mDirty = false;

	return entry;
}


bool cMeasurementStep_Movement::onEdit()
{
	cMovementStepInfoDlg dlg;

	if (mX_mm.has_value())
		dlg.setX_mm(mX_mm.value());

	if (mY_mm.has_value())
		dlg.setY_mm(mY_mm.value());

	if (mZ_mm.has_value())
		dlg.setZ_mm(mZ_mm.value());
	else if (mHeightAGL_mm.has_value())
		dlg.setHeightAGL_mm(mHeightAGL_mm.value());
	else if (mHeightACL_mm.has_value())
		dlg.setHeightACL_mm(mHeightACL_mm.value());

	dlg.setSpeed_mmps(mSpeed_mmps);

	if (mPan_deg.has_value())
		dlg.setPan_deg(mPan_deg.value());

	if (mTilt_deg.has_value())
		dlg.setTilt_deg(mTilt_deg.value());

	if (mRoll_deg.has_value())
		dlg.setRoll_deg(mRoll_deg.value());

	dlg.setSpeed_mmps(mSpeed_mmps);

	dlg.setRecording(mRecording);

	auto result = dlg.exec();

	if (result == QDialog::Rejected)
		return false;

	std::optional<int> x_mm;
	if (dlg.hasX())
	{
		if (dlg.x_mm() > 0)
			x_mm = dlg.x_mm();
	}
	setX_mm(x_mm);

	std::optional<int> y_mm;
	if (dlg.hasY())
	{
		if (dlg.y_mm() > 0)
			y_mm = dlg.y_mm();
	}
	setY_mm(y_mm);

	std::optional<int> z_mm;
	if (dlg.hasZ())
	{
		if (dlg.z_mm() > 0)
			z_mm = dlg.z_mm();
	}
	setZ_mm(z_mm);

	std::optional<int> height_agl_mm;
	if (dlg.hasHeightAGL())
	{
		if (dlg.height_agl_mm() > 0)
			height_agl_mm = dlg.height_agl_mm();
	}
	setHeightAGL_mm(height_agl_mm);

	std::optional<int> height_acl_mm;
	if (dlg.hasHeightACL())
	{
		if (dlg.height_acl_mm() > 0)
			height_acl_mm = dlg.height_acl_mm();
	}
	setHeightACL_mm(height_acl_mm);

	int speed_mmps = dlg.speed_mmps();
	setSpeed_mmps(speed_mmps);

	std::optional<int> pan_deg;
	if (dlg.hasPan())
	{
		pan_deg = dlg.pan_deg();
	}
	setPan_deg(pan_deg);

	std::optional<int> tilt_deg;
	if (dlg.hasTilt())
	{
		tilt_deg = dlg.tilt_deg();
	}
	setTilt_deg(tilt_deg);

	std::optional<int> roll_deg;
	if (dlg.hasRoll())
	{
		roll_deg = dlg.roll_deg();
	}
	setRoll_deg(roll_deg);

	bool recording = dlg.recording();
	setRecording(recording);

	if (isDirty())
	{
		auto description = generateMovementDescription();
		emit onMovementTextChange(description);

		description = generateOrientationDescription();
		emit onOrientationTextChange(description);

		auto comment = generateComment();
		emit onCommentChange(comment);

		emit redraw();
	}

	return true;
}

QString cMeasurementStep_Movement::generateMovementDescription() const
{
	QString description;

	if (mX_mm.has_value() && mY_mm.has_value() && mZ_mm.has_value())
	{
		description = "Moving to ";
		description += QString::number(mX_mm.value());
		description += " mm, ";
		description += QString::number(mY_mm.value());
		description += " mm, ";
		description += QString::number(mZ_mm.value());
		description += " mm @ ";
		description += QString::number(mSpeed_mmps);
		description += " mm/sec";
	}
	else if (mX_mm.has_value() && mY_mm.has_value())
	{
		description = "Moving horizontally to ";
		description += QString::number(mX_mm.value());
		description += " mm, ";
		description += QString::number(mY_mm.value());
		description += " mm @ ";
		description += QString::number(mSpeed_mmps);
		description += " mm/sec";
	}
	else if (mZ_mm.has_value())
	{
		description = "Moving vertically to ";
		description += QString::number(mZ_mm.value());
		description += " mm @ ";
		description += QString::number(mSpeed_mmps);
		description += " mm/sec";
	}
	else if (mHeightAGL_mm.has_value())
	{
		description = "Moving vertically to ";
		description += QString::number(mHeightAGL_mm.value());
		description += " AGL mm @ ";
		description += QString::number(mSpeed_mmps);
		description += " mm/sec";
	}
	else if (mHeightACL_mm.has_value())
	{
		description = "Moving vertically to ";
		description += QString::number(mHeightACL_mm.value());
		description += " ACL mm @ ";
		description += QString::number(mSpeed_mmps);
		description += " mm/sec";
	}

	return description;
}

QString cMeasurementStep_Movement::generateOrientationDescription() const
{
	QString description;

	if (mPan_deg.has_value() || mTilt_deg.has_value() || mRoll_deg.has_value())
	{
		description = "Setting ";

		if (mPan_deg.has_value())
		{
			double pan_deg = mPan_deg.value();

			description += "pan to ";
			description += QString::number(pan_deg, 'f', 1);
			description += " deg";
		}

		if (mTilt_deg.has_value())
		{
			if (mPan_deg.has_value())
				description += ", ";

			double tilt_deg = mTilt_deg.value();

			description += "tilt to ";
			description += QString::number(tilt_deg, 'f', 1);
			description += " deg";
		}

		if (mRoll_deg.has_value())
		{
			if (mPan_deg.has_value() || mTilt_deg.has_value())
				description += ", ";

			double roll_deg = mRoll_deg.value();

			description += "roll to ";
			description += QString::number(roll_deg, 'f', 1);
			description += " deg";
		}
	}

	return description;
}

QString cMeasurementStep_Movement::generateComment() const
{
	if (mRecording)
		return QString("Recording: On");

	return QString();
}


/********************************************************************
 *
 * Measurement Step: Marker
 *
 ********************************************************************/

const cMeasurementStep_Marker::eMarkerType cMeasurementStep_Marker::getMarkerType() const
{
	return mMarkerType;
}

const std::string cMeasurementStep_Marker::getMarkerLabel() const
{
	if (mMarkerType == eMarkerType::CUSTOM)
		return mMarkerLabel;

	return std::string();
}

void cMeasurementStep_Marker::setMarkerType(eMarkerType marker_type)
{
	mDirty |= mMarkerType != marker_type;
	mMarkerType = marker_type;
}

void cMeasurementStep_Marker::setMarkerLabel(const std::string& marker_label)
{
	mDirty |= mMarkerLabel != marker_label;
	mMarkerLabel = marker_label;
}

cBaseStep* cMeasurementStep_Marker::graphicsItem(const int id, eExperimentType exp_type) const
{
	auto step = new cProcessStep(id, exp_type);
	connect(step, &cProcessStep::editStep, this, &cMeasurementStep_Marker::onEdit);
	connect(this, &cMeasurementStep_Marker::onMarkerTypeTextChange, step, &cProcessStep::setSubHeading1);

	step->setTitle("Marker");

	auto description = generateDescription();
	step->setSubHeading1(description);

	return step;
}

void cMeasurementStep_Marker::load(const nlohmann::json& jdoc)
{
	using namespace nlohmann;

	if (jdoc.contains("marker type"))
	{
		std::string marker_type = jdoc["marker type"];
		if (marker_type == "start position")
			mMarkerType = eMarkerType::START_OF_MEASUREMENT;
		else if (marker_type == "end position")
			mMarkerType = eMarkerType::END_OF_MEASUREMENT;
		else
		{
			mMarkerType = eMarkerType::CUSTOM;
			mMarkerLabel = marker_type;
		}
	}
}

nlohmann::json cMeasurementStep_Marker::save()
{
	nlohmann::json entry;

	if ((mMarkerType == eMarkerType::CUSTOM) && mMarkerLabel.empty())
		return entry;

	entry["type"] = "marker";

	switch (mMarkerType)
	{
	case eMarkerType::START_OF_MEASUREMENT:
		entry["marker type"] = "start position";
		break;
	case eMarkerType::END_OF_MEASUREMENT:
		entry["marker type"] = "end position";
		break;
	case eMarkerType::CUSTOM:
	default:
		entry["marker type"] = mMarkerLabel;
		break;
	}

	mDirty = false;

	return entry;
}

bool cMeasurementStep_Marker::onEdit()
{
	cMarkerInfoDlg dlg;

	switch (mMarkerType)
	{
	case eMarkerType::START_OF_MEASUREMENT:
		dlg.setType(cMarkerInfoDlg::START_OF_MEASUREMENT);
		break;
	case eMarkerType::END_OF_MEASUREMENT:
		dlg.setType(cMarkerInfoDlg::END_OF_MEASUREMENT);
		break;
	case eMarkerType::CUSTOM:
	default:
		dlg.setType(cMarkerInfoDlg::CUSTOM);
		break;
	}

	dlg.setLabel(mMarkerLabel);

	auto result = dlg.exec();

	if (result == QDialog::Rejected)
		return false;

	switch (dlg.type())
	{
	case cMarkerInfoDlg::START_OF_MEASUREMENT:
		setMarkerType(eMarkerType::START_OF_MEASUREMENT);
		break;
	case cMarkerInfoDlg::END_OF_MEASUREMENT:
		setMarkerType(eMarkerType::END_OF_MEASUREMENT);
		break;
	case cMarkerInfoDlg::CUSTOM:
	default:
		setMarkerType(eMarkerType::CUSTOM);
		setMarkerLabel(dlg.label());
		break;
	}

	if (isDirty())
	{
		auto description = generateDescription();
		emit onMarkerTypeTextChange(description);

		emit redraw();
	}

	return true;
}

QString cMeasurementStep_Marker::generateDescription() const
{
	switch (mMarkerType)
	{
	case eMarkerType::START_OF_MEASUREMENT:
		return "Start Position";
		break;
	case eMarkerType::END_OF_MEASUREMENT:
		return "End Position";
		break;
	case eMarkerType::CUSTOM:
	default:
		return QString::fromStdString(mMarkerLabel);
		break;
	}

	return QString();
}


