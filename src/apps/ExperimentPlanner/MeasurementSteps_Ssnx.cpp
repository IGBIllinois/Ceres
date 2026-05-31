
#include "MeasurementSteps_Ssnx.hpp"
#include "ExperimentDesignItems.hpp"

#include "ReferencePointDlg.hpp"

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

std::shared_ptr<cMeasurementStep> ssnx::create_step(const std::string& type, const nlohmann::json& info)
{
	if (type == "ssnx")
	{
		if (info.contains("command"))
		{
			std::string command = info["command"];

			if (command == "reference")
				return std::make_shared<cMeasurementStep_ReferencePoint>();
		}
	}

	return std::shared_ptr<cMeasurementStep>();
}


/********************************************************************
 *
 * Measurement Step: Ssnx GPS Reference Point
 *
 ********************************************************************/

double cMeasurementStep_ReferencePoint::getMinIntegrationTime_sec() const { return mMinIntegrationTime_sec; }
double cMeasurementStep_ReferencePoint::getMaxIntegrationTime_sec() const { return mMaxIntegrationTime_sec; }
int cMeasurementStep_ReferencePoint::getErrorThreshold_mm() const { return mErrorThreshold_mm; }

void cMeasurementStep_ReferencePoint::setMinIntegrationTime_sec(double sec)
{
	mDirty |= mMinIntegrationTime_sec != sec;
	mMinIntegrationTime_sec = sec;
}

void cMeasurementStep_ReferencePoint::setMaxIntegrationTime_sec(double sec)
{
	mDirty |= mMaxIntegrationTime_sec != sec;
	mMaxIntegrationTime_sec = sec;
}

void cMeasurementStep_ReferencePoint::setErrorThreshold_mm(int threshold_mm)
{
	mDirty |= mErrorThreshold_mm != threshold_mm;
	mErrorThreshold_mm = threshold_mm;
}

cBaseStep* cMeasurementStep_ReferencePoint::graphicsItem(const int id, eExperimentType exp_type) const
{
	auto step = new cProcessStep(id, exp_type);
	connect(step, &cProcessStep::editStep, this, &cMeasurementStep_ReferencePoint::onEdit);
	connect(this, &cMeasurementStep_ReferencePoint::onDescriptionChange, step, &cProcessStep::setSubHeading1);
	connect(this, &cMeasurementStep_ReferencePoint::onCommentChange, step, &cProcessStep::setSubHeading2);

	step->setTitle("SSNX: Reference Point");

	auto description = generateDescription();
	step->setSubHeading1(description);

	auto comment = generateComment();
	step->setSubHeading2(comment);

	return step;
}

void cMeasurementStep_ReferencePoint::load(const nlohmann::json& jdoc)
{
	using namespace nlohmann;

	if (jdoc.contains("min integration time (sec)"))
	{
		mMinIntegrationTime_sec = jdoc["min integration time (sec)"].get<double>();
	}

	if (jdoc.contains("max integration time (sec)"))
	{
		mMaxIntegrationTime_sec = jdoc["max integration time (sec)"].get<double>();
	}

	if (jdoc.contains("error threshold (mm)"))
	{
		mErrorThreshold_mm = jdoc["error threshold (mm)"].get<int>();
	}
}

nlohmann::json cMeasurementStep_ReferencePoint::save()
{
	nlohmann::json entry;

	entry["type"] = "ssnx";

	entry["command"] = "reference";

	entry["min integration time (sec)"] = mMinIntegrationTime_sec;
	entry["max integration time (sec)"] = mMaxIntegrationTime_sec;
	entry["error threshold (mm)"] = mErrorThreshold_mm;

	mDirty = false;

	return entry;
}

bool cMeasurementStep_ReferencePoint::onEdit()
{
	cReferencePointDlg dlg;

	dlg.setMinIntegrationTime_sec(mMinIntegrationTime_sec);
	dlg.setMaxIntegrationTime_sec(mMaxIntegrationTime_sec);
	dlg.setErrorThreshold_mm(mErrorThreshold_mm);

	auto result = dlg.exec();

	if (result == QDialog::Rejected)
		return false;

	double minTime_sec = dlg.minIntegrationTime_sec();
	double maxTime_sec = dlg.maxIntegrationTime_sec();

	setMinIntegrationTime_sec(minTime_sec);
	setMaxIntegrationTime_sec(maxTime_sec);

	int threshold_mm = dlg.errorThreshold_mm();
	setErrorThreshold_mm(threshold_mm);

	auto description = generateDescription();
	emit onDescriptionChange(description);

	auto comment = generateComment();
	emit onCommentChange(comment);

	emit redraw();

	return true;
}

QString cMeasurementStep_ReferencePoint::generateDescription() const
{
	QString description;

	description = "Integration time: minimum ";
	description += QString::number(mMinIntegrationTime_sec);
	description += " sec, maximum ";
	description += QString::number(mMaxIntegrationTime_sec);
	description += " sec";

	return description;
}

QString cMeasurementStep_ReferencePoint::generateComment() const
{
	QString comment;

	comment = "Error Threshold: ";
	comment += QString::number(mErrorThreshold_mm);
	comment += " mm";

	return comment;
}


