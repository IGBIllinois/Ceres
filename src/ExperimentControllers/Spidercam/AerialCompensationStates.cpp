
#include "AerialCompensationStates.hpp"
#include "AerialCompensationDlg.hpp"
#include "../../ExperimentStateMachine/ExperimentVariableTable.hpp"
#include "../../Utilities/Constants.hpp"
#include "../../Utilities/RappFieldBoundary.hpp"
#include "../../Utilities/RappFieldModel.hpp"

#include <QApplication>



/********************************************************************
 * Above Ground Height Step
 *******************************************************************/
cExperimentState_AGH::cExperimentState_AGH()
	: mpDlg(nullptr)
{
}

cExperimentState_AGH::~cExperimentState_AGH()
{
	mpDlg->deleteLater();
}

bool cExperimentState_AGH::configure(const nlohmann::json& stateDoc)
{
	int32_t sensor_offset_mm = 0;
	int32_t reference_height_mm = nRFM::reference_height_mm();

	auto variables = mVariables.lock();
	if (variables->contains("sensor_offset_mm"))
		variables->get("sensor_offset_mm", sensor_offset_mm);
	else
	{
		if (stateDoc.contains("defaults"))
		{
			auto defaults = stateDoc["defaults"];
			if (defaults.contains("sensor_offset_mm"))
				sensor_offset_mm = defaults["sensor_offset_mm"];
		}
	}

	if (variables->contains("reference_height_mm"))
		variables->get("reference_height_mm", reference_height_mm);


	// We are in the data thread.  Any GUI object must exists in the
	// QApplication thread!
	mpDlg = new cAboveGroundHeightDlg();

	mpDlg->setSensorOffset_mm(sensor_offset_mm);
	mpDlg->setReferenceHeight_mm(reference_height_mm);

	auto result = mpDlg->exec();

	if (result == QDialog::Rejected)
		return false;

	sensor_offset_mm = mpDlg->getSensorOffset_mm();
	reference_height_mm = mpDlg->getReferenceHeight_mm();

	if (!variables->set("sensor_offset_mm", sensor_offset_mm))
		variables->add("sensor_offset_mm", sensor_offset_mm);

	if (!variables->set("reference_height_mm", reference_height_mm))
		variables->set("reference_height_mm", reference_height_mm);

	return true;
}

QString cExperimentState_AGH::getStatusStr()
{
	return QString("Waiting for user...");
}

bool cExperimentState_AGH::recording()
{
	return false;
}

bool cExperimentState_AGH::initialize()
{
	return true;
}

void cExperimentState_AGH::run() {}
void cExperimentState_AGH::pause() {}
void cExperimentState_AGH::stop() {}

cExperimentState::eRESULT cExperimentState_AGH::finished()
{
	return eRESULT::DONE;
}



/********************************************************************
 * Above Canopy Height Step
 *******************************************************************/
cExperimentState_ACH::cExperimentState_ACH()
	: mpDlg(nullptr)
{
}

cExperimentState_ACH::~cExperimentState_ACH()
{
	mpDlg->deleteLater();
}

bool cExperimentState_ACH::configure(const nlohmann::json& stateDoc)
{
	int32_t sensor_offset_mm = 0;
	int32_t canopy_height_mm = 0;
	int32_t reference_height_mm = nRFM::reference_height_mm();

	auto variables = mVariables.lock();

	if (variables->contains("canopy_height_mm"))
		variables->get("canopy_height_mm", canopy_height_mm);
	else
	{
		if (stateDoc.contains("defaults"))
		{
			auto defaults = stateDoc["defaults"];
			if (defaults.contains("canopy_height_mm"))
				canopy_height_mm = defaults["canopy_height_mm"];
		}
	}

	if (variables->contains("sensor_offset_mm"))
		variables->get("sensor_offset_mm", sensor_offset_mm);
	else
	{
		if (stateDoc.contains("defaults"))
		{
			auto defaults = stateDoc["defaults"];
			if (defaults.contains("sensor_offset_mm"))
				sensor_offset_mm = defaults["sensor_offset_mm"];
		}
	}

	if (variables->contains("reference_height_mm"))
		variables->get("reference_height_mm", reference_height_mm);


	// We are in the data thread.  Any GUI object must exists in the
	// QApplication thread!
	mpDlg = new cAboveCanopyHeightDlg();

	mpDlg->setCanopyHeight_mm(canopy_height_mm);
	mpDlg->setSensorOffset_mm(sensor_offset_mm);
	mpDlg->setReferenceHeight_mm(reference_height_mm);

	auto result = mpDlg->exec();

	if (result == QDialog::Rejected)
		return false;

	canopy_height_mm = mpDlg->getCanopyHeight_mm();
	sensor_offset_mm = mpDlg->getSensorOffset_mm();
	reference_height_mm = mpDlg->getReferenceHeight_mm();

	if (!variables->set("canopy_height_mm", canopy_height_mm))
		variables->add("canopy_height_mm", canopy_height_mm);

	if (!variables->set("sensor_offset_mm", sensor_offset_mm))
		variables->add("sensor_offset_mm", sensor_offset_mm);

	if (!variables->set("reference_height_mm", reference_height_mm))
		variables->set("reference_height_mm", reference_height_mm);

	return true;
}

QString cExperimentState_ACH::getStatusStr()
{
	return QString("Waiting for user...");
}

bool cExperimentState_ACH::recording()
{
	return false;
}

bool cExperimentState_ACH::initialize()
{
	return true;
}

void cExperimentState_ACH::run() {}
void cExperimentState_ACH::pause() {}
void cExperimentState_ACH::stop() {}

cExperimentState::eRESULT cExperimentState_ACH::finished()
{
	return eRESULT::DONE;
}

