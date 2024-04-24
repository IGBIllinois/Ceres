
#include "ExperimentSteps.hpp"
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


/********************************************************************
 *
 * Experiment Step: Base Class
 *
 ********************************************************************/

cExperimentStep::~cExperimentStep()
{}

bool cExperimentStep::isDirty() const
{
	return mDirty;
}



/********************************************************************
 *
 * Experiment Step: Simple Delay
 *
 ********************************************************************/

cConnectedItem* cExperimentStep_Delay::graphicsItem() const
{
	auto step = new cProcessStep();
	connect(step, &cProcessStep::editStep, this, &cExperimentStep_Delay::onEdit);
	connect(this, &cExperimentStep_Delay::onDescriptionChange, step, &cProcessStep::setSubHeading1);
	connect(this, &cExperimentStep_Delay::onCommentChange, step, &cProcessStep::setSubHeading2);

	step->setTitle("Delay");

	auto description = generateDescription();
	step->setSubHeading1(description);

	auto comment = generateComment();
	step->setSubHeading2(comment);

	return step;
}

void cExperimentStep_Delay::load(const nlohmann::json& jdoc)
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

nlohmann::json cExperimentStep_Delay::save()
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

void cExperimentStep_Delay::onEdit()
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
		return;

	std::optional<int> hours;
	if (dlg.hasHours())
	{
		if (dlg.hours() > 0)
			hours = dlg.hours();
	}
	mDirty = mWaitTime_hr != hours;
	mWaitTime_hr = hours;

	std::optional<int> minutes;
	if (dlg.hasMinutes())
	{
		if (dlg.minutes() > 0)
			minutes = dlg.minutes();
	}
	mDirty = mWaitTime_min != minutes;
	mWaitTime_min = minutes;

	double seconds = dlg.seconds();
	mDirty = mWaitTime_sec != seconds;
	mWaitTime_sec = seconds;

	bool record = dlg.recording();
	mDirty = mRecording != record;
	mRecording = record;

	auto description = generateDescription();
	emit onDescriptionChange(description);

	auto comment = generateComment();
	emit onCommentChange(comment);
}

QString cExperimentStep_Delay::generateDescription() const
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

QString cExperimentStep_Delay::generateComment() const
{
	if (mRecording)
		return QString("Recording: On");

	return QString();
}


/********************************************************************
 *
 * Experiment Step: Pause
 *
 ********************************************************************/

cConnectedItem* cExperimentStep_Pause::graphicsItem() const
{
	auto step = new cIoStep();
	step->setReadOnly(true);
	step->setTitle("Wait for User OK to Advance...");

	return step;
}

void cExperimentStep_Pause::load(const nlohmann::json& jdoc)
{
	using namespace nlohmann;
}

nlohmann::json cExperimentStep_Pause::save()
{
	nlohmann::json entry;

	entry["type"] = "pause";

	return entry;
}


/********************************************************************
 *
 * Experiment Step: SpiderCam Movement
 *
 ********************************************************************/

const std::optional<int>& cExperimentStep_Movement::getX_mm() const { return mX_mm; }
const std::optional<int>& cExperimentStep_Movement::getY_mm() const { return mY_mm; }
const std::optional<int>& cExperimentStep_Movement::getZ_mm() const { return mZ_mm; }

int cExperimentStep_Movement::getSpeed_mmps() const { return mSpeed_mmps; }

const std::optional<double>& cExperimentStep_Movement::getPan_deg() const { return mPan_deg; }
const std::optional<double>& cExperimentStep_Movement::getTilt_deg() const { return mTilt_deg; }
const std::optional<double>& cExperimentStep_Movement::getRoll_deg() const { return mRoll_deg; }

bool cExperimentStep_Movement::isRecording() const { return mRecording; }

cConnectedItem* cExperimentStep_Movement::graphicsItem() const
{
	auto step = new cProcessStep();
	connect(step, &cProcessStep::editStep, this, &cExperimentStep_Movement::onEdit);
	connect(this, &cExperimentStep_Movement::onMovementTextChange, step, &cProcessStep::setSubHeading1);
	connect(this, &cExperimentStep_Movement::onOrientationTextChange, step, &cProcessStep::setSubHeading2);
	connect(this, &cExperimentStep_Movement::onCommentChange, step, &cProcessStep::setSubHeading3);

	step->setTitle("Movement");

	auto msg = generateMovementDescription();
	step->setSubHeading1(msg);

	msg = generateOrientationDescription();
	step->setSubHeading2(msg);

	msg = generateComment();
	step->setSubHeading3(msg);

	return step;
}

void cExperimentStep_Movement::load(const nlohmann::json& jdoc)
{
	auto pos = jdoc["position"];

	if (pos.contains("x (mm)"))
	{
		mX_mm = static_cast<uint32_t>(pos["x (mm)"].get<int>());
	}
	else if (pos.contains("x (m)"))
	{
		mX_mm = static_cast<uint32_t>(pos["x (m)"].get<double>() * nConstants::M_TO_MM);
	}

	if (pos.contains("y (mm)"))
	{
		mY_mm = static_cast<uint32_t>(pos["y (mm)"].get<int>());
	}
	else if (pos.contains("y (m)"))
	{
		mY_mm = static_cast<uint32_t>(pos["y (m)"].get<double>() * nConstants::M_TO_MM);
	}

	if (pos.contains("z (mm)"))
	{
		mZ_mm = static_cast<uint32_t>(pos["z (mm)"].get<int>());
	}
	else if (pos.contains("z (m)"))
	{
		mZ_mm = static_cast<uint32_t>(pos["z (m)"].get<double>() * nConstants::M_TO_MM);
	}

	if (pos.contains("speed (mm/s)"))
	{
		mSpeed_mmps = jdoc["speed (mm/s)"].get<int>();
	}
	else
	{
		mSpeed_mmps = static_cast<uint32_t>(jdoc["speed (m/s)"].get<double>() * nConstants::M_TO_MM);
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

	mRecording = jdoc["record"];
}

nlohmann::json cExperimentStep_Movement::save()
{
	nlohmann::json entry;

	entry["type"] = "movement";

	entry["record"] = mRecording;

	return entry;
}


void cExperimentStep_Movement::onEdit()
{
	cMovementStepInfoDlg dlg;

	if (mX_mm.has_value())
		dlg.setX_mm(mX_mm.value());

	if (mY_mm.has_value())
		dlg.setY_mm(mY_mm.value());

	if (mZ_mm.has_value())
		dlg.setZ_mm(mZ_mm.value());

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
		return;

	std::optional<int> x_mm;
	if (dlg.hasX())
	{
		if (dlg.x_mm() > 0)
			x_mm = dlg.x_mm();
	}
	mDirty = mX_mm != x_mm;
	mX_mm = x_mm;

	std::optional<int> y_mm;
	if (dlg.hasY())
	{
		if (dlg.y_mm() > 0)
			y_mm = dlg.y_mm();
	}
	mDirty = mY_mm != y_mm;
	mY_mm = y_mm;

	std::optional<int> z_mm;
	if (dlg.hasZ())
	{
		if (dlg.z_mm() > 0)
			z_mm = dlg.z_mm();
	}
	mDirty = mZ_mm != z_mm;
	mZ_mm = z_mm;

	int speed_mmps = dlg.speed_mmps();
	mDirty = mSpeed_mmps != speed_mmps;
	mSpeed_mmps = speed_mmps;

	std::optional<int> pan_deg;
	if (dlg.hasPan())
	{
		pan_deg = dlg.pan_deg();
	}
	mDirty = mPan_deg != pan_deg;
	mPan_deg = pan_deg;

	std::optional<int> tilt_deg;
	if (dlg.hasTilt())
	{
		tilt_deg = dlg.tilt_deg();
	}
	mDirty = mTilt_deg != tilt_deg;
	mTilt_deg = tilt_deg;

	std::optional<int> roll_deg;
	if (dlg.hasRoll())
	{
		roll_deg = dlg.roll_deg();
	}
	mDirty = mRoll_deg != roll_deg;
	mRoll_deg = roll_deg;

	bool recording = dlg.recording();
	mDirty = mRecording != recording;
	mRecording = recording;
}

QString cExperimentStep_Movement::generateMovementDescription() const
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

	return description;
}

QString cExperimentStep_Movement::generateOrientationDescription() const
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

QString cExperimentStep_Movement::generateComment() const
{
	if (mRecording)
		return QString("Recording: On");

	return QString();
}



