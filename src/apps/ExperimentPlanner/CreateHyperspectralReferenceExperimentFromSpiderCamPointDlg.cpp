
#include "CreateHyperspectralReferenceExperimentFromSpiderCamPointDlg.hpp"
#include "Constants.hpp"

#include "ExperimentSteps.hpp"

#include "ExperimentMetaInfoDlg.hpp"
#include "ExperimentCtrlInfoDlg.hpp"
#include "ExperimentSensorInfoDlg.hpp"

#include "RappGroundModel.hpp"
#include "RappFieldBoundary.hpp"

#include "StringUtils.hpp"

#include <QLabel>
#include <QLayout>
#include <QCheckBox>
#include <QComboBox>
#include <QPushButton>
#include <QGridLayout>
#include <QGroupBox>
#include <QDialogButtonBox>
#include <QLineEdit>
#include <QIntValidator>
#include <QPlainTextEdit>
#include <QTableView>
#include <QStringList>
#include <QStandardItemModel>
#include <QHeaderView>
#include <QMessageBox>


#include <algorithm>
#include <memory>

namespace
{
	const QString SCAN_DISTANCE_TEXT = "Distance (";
	const QString SCAN_SEPARATION_TEXT = "Separation (";

	const QString WEST_TO_EAST = "West to East";
	const QString EAST_TO_WEST = "East to West";
	const QString NORTH_TO_SOUTH = "North to South";
	const QString SOUTH_TO_NORTH = "South to North";

	constexpr int SCAN_WEST_TO_EAST = 0;
	constexpr int SCAN_EAST_TO_WEST = 1;
	constexpr int SCAN_NORTH_TO_SOUTH = 2;
	constexpr int SCAN_SOUTH_TO_NORTH = 3;
}

cCreateHyperspectralReferenceExperimentFromSpiderCamDlg::cCreateHyperspectralReferenceExperimentFromSpiderCamDlg(QWidget* parent)
:
	cCreateHyperspectralReferenceExperimentDlg(parent)
{
	setMinimumWidth(550);

	initialize();
}

cCreateHyperspectralReferenceExperimentFromSpiderCamDlg::~cCreateHyperspectralReferenceExperimentFromSpiderCamDlg()
{}

void cCreateHyperspectralReferenceExperimentFromSpiderCamDlg::createControls_PointSelection()
{
	mpRefPosX_mm = new QLineEdit(this);
	mpRefPosX_mm->setValidator(new QIntValidator(10000, 190000));

	mpRefPosY_mm = new QLineEdit(this);
	mpRefPosY_mm->setValidator(new QIntValidator(10000, 190000));
}

void cCreateHyperspectralReferenceExperimentFromSpiderCamDlg::createLayout_PointSelection(QVBoxLayout* pMainLayout)
{
	QLabel* pText = nullptr;
	QGroupBox* pGroupBox = nullptr;
	QGridLayout* pGridLayout = nullptr;

	QHBoxLayout* pPosLayout = new QHBoxLayout();

	pPosLayout->addStretch(1);

	pGridLayout = new QGridLayout();
	pGridLayout->setColumnMinimumWidth(2, 10);

	pText = new QLabel("X position (mm)");
	pGridLayout->addWidget(pText, 0, 0);
	pGridLayout->addWidget(mpRefPosX_mm, 0, 1);
	pText = new QLabel("Y position (mm)");
	pGridLayout->addWidget(pText, 0, 3);
	pGridLayout->addWidget(mpRefPosY_mm, 0, 4);
	pPosLayout->addLayout(pGridLayout);

	pPosLayout->addStretch(1);

//	QVBoxLayout* pVSubLayout = new QVBoxLayout();
//	pVSubLayout->addWidget(mpClearPath);
//	pVSubLayout->addWidget(mpShowPath);
//	pPosLayout->addLayout(pVSubLayout);
//	pPosLayout->addStretch(1);

	pMainLayout->addLayout(pPosLayout);

	pMainLayout->addSpacing(10);
}

bool cCreateHyperspectralReferenceExperimentFromSpiderCamDlg::generate()
{
	std::string str;
	QString text;
	std::vector<std::string> list;

	std::string title = mpTitle->text().toStdString();
	if (title.empty())
	{
		QString msg = "The \"Experiment Title\" can not be blank.";
		QMessageBox msg_box(QMessageBox::Critical, "Invalid Parameter", msg);
		msg_box.exec();
		return false;
	}

	if (mpRefPosX_mm->text().isEmpty() || mpRefPosY_mm->text().isEmpty())
	{
		QString msg = "The SpiderCam position can not be blank.";
		QMessageBox msg_box(QMessageBox::Critical, "Invalid Parameter", msg);
		msg_box.exec();
		return false;
	}

	auto* pGroundModel = cRappGroundModel::get();

	int x1_mm = mpRefPosX_mm->text().toInt();
	int y1_mm = mpRefPosY_mm->text().toInt();

	if (!rfb::withinBoundary(x1_mm, y1_mm))
	{
		QString msg = "The SpiderCam position must be a valid field position (10,000mm to 190,000mm).";
		QMessageBox msg_box(QMessageBox::Critical, "Invalid Parameter", msg);
		msg_box.exec();
		return false;
	}

	int h1_mm = 0;
	
	if (pGroundModel)
	{
		h1_mm = static_cast<int>(pGroundModel->getMeshHeight_mm(x1_mm, y1_mm));

		if (h1_mm == rfm::INVALID_HEIGHT)
			h1_mm = 0;
	}

	int travel_z_mm = static_cast<int>(mpTravelHeight_m->text().toDouble() * nConstants::M_TO_MM);
	int scan_z_mm = static_cast<int>(mpReferenceHeight_m->text().toDouble() * nConstants::M_TO_MM);
	scan_z_mm += mpLensFocalDistance->currentData().toInt();

	int safe_z_mm = static_cast<int>(mpSafeHeight_m->text().toDouble() * nConstants::M_TO_MM);

	std::optional<double> tilt_deg;
	std::optional<double> safe_tilt_deg;
	if (!mpGimbleTilt_deg->text().isEmpty())
	{
		double value = mpGimbleTilt_deg->text().toDouble();
		if (value != 0.0)
		{
			tilt_deg = value;
			safe_tilt_deg = 0.0;
		}
	}

	std::optional<double> pan_deg;
	if (!mpGimblePan_deg->text().isEmpty())
	{
		double value = mpGimblePan_deg->text().toDouble();
		if (value != 0.0)
			pan_deg = value;
	}

	std::optional<double> roll_deg;
	std::optional<double> safe_roll_deg;
	if (!mpGimbleRoll_deg->text().isEmpty())
	{
		double value = mpGimbleRoll_deg->text().toDouble();
		if (value != 0.0)
		{
			roll_deg = value;
			safe_roll_deg = 0.0;
		}
	}

	if (!mpSensorOffset_mm->text().isEmpty())
	{
		int sensor_offset_mm = mpSensorOffset_mm->text().toInt();
		scan_z_mm += sensor_offset_mm;
	}

	int vertical_speed_mmps = mpTravelVerticalSpeed_mmps->text().toInt();
	int travel_speed_mmps = mpTravelSpeed_mmps->text().toInt();
	int safe_vertical_speed_mmps = mpSafeVerticalSpeed_mmps->text().toInt();

	QSharedPointer<cExperimentFile> pInfo = QSharedPointer<cExperimentFile>(new cExperimentFile());

	pInfo->setExperimentName(title);
	pInfo->setController(copy(mCtrlInfo));
	pInfo->setSensors(mSensorInfo);

	// Add preamble: moving dolly up to a safe travel height...
	std::unique_ptr<cExperimentStep_Movement> step = std::make_unique<cExperimentStep_Movement>();

	step->setZ_mm(travel_z_mm);
	step->setSpeed_mmps(vertical_speed_mmps);
	pInfo->appendStep(std::move(step));

	// Moving dolly to the beginning of the measurement scan...
	step = std::make_unique<cExperimentStep_Movement>();
	step->setX_mm(x1_mm);
	step->setY_mm(y1_mm);
	step->setSpeed_mmps(travel_speed_mmps);
	pInfo->appendStep(std::move(step));

	// Move the dolly to measurement height...
	step = std::make_unique<cExperimentStep_Movement>();

	step->setZ_mm(scan_z_mm + h1_mm);

	step->setSpeed_mmps(vertical_speed_mmps);

	step->setTilt_deg(tilt_deg);
	step->setRoll_deg(roll_deg);
	step->setPan_deg(pan_deg);

	pInfo->appendStep(std::move(step));

	float delay_sec = mpStartMeasurementDelay_sec->text().toFloat();

	if (delay_sec > 0.0)
	{
		// Add delay for dolly to stabilize...
		std::unique_ptr<cExperimentStep_Delay> delay = std::make_unique<cExperimentStep_Delay>();
		delay->setWaitTime_sec(delay_sec);
		pInfo->appendStep(std::move(delay));
	}

	float measurement_time_sec = mpMeasurementTime_sec->text().toFloat();

	if (measurement_time_sec > 0.0)
	{
		// Do measurement...
		std::unique_ptr<cExperimentStep_Delay> delay = std::make_unique<cExperimentStep_Delay>();
		delay->setWaitTime_sec(measurement_time_sec);
		delay->setRecording(true);
		pInfo->appendStep(std::move(delay));
	}

	// Move dolly to a safe height to park it
	step = std::make_unique<cExperimentStep_Movement>();
	step->setZ_mm(safe_z_mm);
	step->setSpeed_mmps(safe_vertical_speed_mmps);
	step->setTilt_deg(safe_tilt_deg);
	step->setRoll_deg(safe_roll_deg);
	pInfo->appendStep(std::move(step));

	emit experimentChanged(pInfo);

	if (mpGeneratePlacementExperiment->isChecked())
	{
		title += " Placement";

		QSharedPointer<cExperimentFile> pInfo = QSharedPointer<cExperimentFile>(new cExperimentFile());

		pInfo->setExperimentName(title);
		pInfo->setController(copy(mCtrlInfo));
		pInfo->setSensors(mSensorInfo);

		// Add preamble: moving dolly up to a safe travel height...
		std::unique_ptr<cExperimentStep_Movement> step = std::make_unique<cExperimentStep_Movement>();

		step->setZ_mm(travel_z_mm);
		step->setSpeed_mmps(vertical_speed_mmps);
		pInfo->appendStep(std::move(step));

		// Moving dolly to the beginning of the measurement scan...
		step = std::make_unique<cExperimentStep_Movement>();
		step->setX_mm(x1_mm);
		step->setY_mm(y1_mm);
		step->setSpeed_mmps(travel_speed_mmps);
		pInfo->appendStep(std::move(step));

		// Move the dolly to measurement height...
		step = std::make_unique<cExperimentStep_Movement>();

		step->setZ_mm(scan_z_mm + h1_mm);

		step->setSpeed_mmps(vertical_speed_mmps);

		step->setTilt_deg(tilt_deg);
		step->setRoll_deg(roll_deg);
		step->setPan_deg(pan_deg);

		pInfo->appendStep(std::move(step));

		emit experimentChanged(pInfo);
	}

	return true;
}

/*
void cCreateHyperspectralReferenceExperimentFromSpiderCamDlg::onShowPath()
{
	int x1_mm = mpStartX_mm->text().toInt();
	int y1_mm = mpStartY_mm->text().toInt();

	int distance_mm = static_cast<int>(mpScanDistance->text().toDouble() * mScanConversionFactor);

	int x2_mm = x1_mm;
	int y2_mm = y1_mm;

	switch (mpScanOrientation->currentIndex())
	{
	case SCAN_WEST_TO_EAST:
		y2_mm += distance_mm;
		break;
	case SCAN_EAST_TO_WEST:
		y2_mm -= distance_mm;
		break;
	case SCAN_NORTH_TO_SOUTH:
		x2_mm += distance_mm;
		break;
	case SCAN_SOUTH_TO_NORTH:
		x2_mm -= distance_mm;
		break;
	}


	emit drawPath(x1_mm, y1_mm, x2_mm, y2_mm);
}
*/
