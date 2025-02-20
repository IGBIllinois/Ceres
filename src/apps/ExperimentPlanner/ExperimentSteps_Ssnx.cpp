
#include "ExperimentSteps_Ssnx.hpp"
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

std::shared_ptr<cExperimentStep> ssnx::create_step(const std::string& type, const nlohmann::json& info)
{
	if (type == "ssnx")
	{
		if (info.contains("command"))
		{
			std::string command = info["command"];

			if (command == "reference")
				return std::make_shared<cExperimentStep_ReferencePoint>();
		}
	}

	return std::shared_ptr<cExperimentStep>();
}


/********************************************************************
 *
 * Experiment Step: Ssnx GPS Reference Point
 *
 ********************************************************************/

double cExperimentStep_ReferencePoint::getMinIntegrationTime_sec() const { return mMinIntegrationTime_sec; }
double cExperimentStep_ReferencePoint::getMaxIntegrationTime_sec() const { return mMaxIntegrationTime_sec; }
int cExperimentStep_ReferencePoint::getErrorThreshold_mm() const { return mErrorThreshold_mm; }

void cExperimentStep_ReferencePoint::setMinIntegrationTime_sec(double sec)
{
	mDirty |= mMinIntegrationTime_sec != sec;
	mMinIntegrationTime_sec = sec;
}

void cExperimentStep_ReferencePoint::setMaxIntegrationTime_sec(double sec)
{
	mDirty |= mMaxIntegrationTime_sec != sec;
	mMaxIntegrationTime_sec = sec;
}

void cExperimentStep_ReferencePoint::setErrorThreshold_mm(int threshold_mm)
{
	mDirty |= mErrorThreshold_mm != threshold_mm;
	mErrorThreshold_mm = threshold_mm;
}

cBaseStep* cExperimentStep_ReferencePoint::graphicsItem(const int id) const
{
	auto step = new cProcessStep(id);
	connect(step, &cProcessStep::editStep, this, &cExperimentStep_ReferencePoint::onEdit);
	connect(this, &cExperimentStep_ReferencePoint::onDescriptionChange, step, &cProcessStep::setSubHeading1);
	connect(this, &cExperimentStep_ReferencePoint::onCommentChange, step, &cProcessStep::setSubHeading2);

	step->setTitle("ssnx");

	auto description = generateDescription();
	step->setSubHeading1(description);

	auto comment = generateComment();
	step->setSubHeading2(comment);

	return step;
}

void cExperimentStep_ReferencePoint::load(const nlohmann::json& jdoc)
{
	using namespace nlohmann;
}

nlohmann::json cExperimentStep_ReferencePoint::save()
{
	nlohmann::json entry;

	entry["type"] = "ssnx";

	entry["command"] = "reference";

	mDirty = false;

	return entry;
}

bool cExperimentStep_ReferencePoint::onEdit()
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

QString cExperimentStep_ReferencePoint::generateDescription() const
{
	return "Collect a GPS reference point for later processing.";
}

QString cExperimentStep_ReferencePoint::generateComment() const
{
		return QString("Recording: On");

	return QString();
}


