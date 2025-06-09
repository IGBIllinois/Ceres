
#include "CreateLidarExperimentFromSpiderCamPointDlg.hpp"
#include "Constants.hpp"

#include "MeasurementSteps.hpp"
#include "MeasurementSteps_Ssnx.hpp"

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
	const QString SCAN_DISTANCE_TEXT = "Scan Distance (";
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

cCreateLidarExperimentFromSpiderCamDlg::cCreateLidarExperimentFromSpiderCamDlg(QWidget* parent)
:
	cCreateLidarExperimentDlg(parent)
{
	setMinimumWidth(550);

	initialize();
}

cCreateLidarExperimentFromSpiderCamDlg::~cCreateLidarExperimentFromSpiderCamDlg()
{}

void cCreateLidarExperimentFromSpiderCamDlg::createControls_PointSelection()
{
	mpStartX_mm = new QLineEdit(this);
	mpStartX_mm->setValidator(new QIntValidator(10000, 190000));

	mpStartY_mm = new QLineEdit(this);
	mpStartY_mm->setValidator(new QIntValidator(10000, 190000));

	// default to feet
	mpScanDistanceLabel = new QLabel(SCAN_DISTANCE_TEXT + "ft)", this);
	mpScanDistance = new QLineEdit(this);
	mpScanDistance->setValidator(new QDoubleValidator(0, 10000.0, 3));

	mpScanOrientation = new QComboBox(this);
	mpScanOrientation->setEditable(false);
	mpScanOrientation->addItem(WEST_TO_EAST);
	mpScanOrientation->addItem(EAST_TO_WEST);
	mpScanOrientation->addItem(NORTH_TO_SOUTH);
	mpScanOrientation->addItem(SOUTH_TO_NORTH);

	mpScanUnits = new QComboBox(this);
	mpScanUnits->setEditable(false);
	mpScanUnits->addItem("Meters");
	mpScanUnits->addItem("Millimeters");
	mpScanUnits->addItem("Feet");
	mpScanUnits->addItem("Inches");
	mpScanUnits->setCurrentIndex(2);
	mScanConversionFactor = nConstants::FT_TO_MM;
	connect(mpScanUnits, &QComboBox::currentTextChanged, this, &cCreateLidarExperimentFromSpiderCamDlg::onScanUnitChange);

	mpSampleXY = new QPushButton("Record X, Y", this);
	mpSampleXY->setEnabled(false);
	connect(mpSampleXY, &QPushButton::pressed, this, &cCreateLidarExperimentFromSpiderCamDlg::recordXY);
}

void cCreateLidarExperimentFromSpiderCamDlg::createLayout_PointSelection(QVBoxLayout* pMainLayout)
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
	pGridLayout->addWidget(mpStartX_mm, 0, 1);
	pText = new QLabel("Y position (mm)");
	pGridLayout->addWidget(pText, 0, 3);
	pGridLayout->addWidget(mpStartY_mm, 0, 4);
	pGridLayout->addWidget(mpScanDistanceLabel, 2, 0);
	pGridLayout->addWidget(mpScanDistance, 2, 1);
	pGridLayout->addWidget(mpScanOrientation, 2, 3);
	pGridLayout->addWidget(mpScanUnits, 2, 4);
	pPosLayout->addLayout(pGridLayout);

	pPosLayout->addSpacing(10);
	pPosLayout->addWidget(mpSampleXY);

	pPosLayout->addStretch(1);

	QVBoxLayout* pVSubLayout = new QVBoxLayout();
	pVSubLayout->addWidget(mpClearPath);
	pVSubLayout->addWidget(mpShowPath);
	pPosLayout->addLayout(pVSubLayout);
	pPosLayout->addStretch(1);

	pMainLayout->addLayout(pPosLayout);

	pMainLayout->addSpacing(10);
}

void cCreateLidarExperimentFromSpiderCamDlg::onScanUnitChange(const QString& text)
{
	double distance = mpScanDistance->text().toDouble() * mScanConversionFactor;

	switch (mpScanUnits->currentIndex())
	{
	case 0:
		mpScanDistanceLabel->setText(SCAN_DISTANCE_TEXT + "m)");

		mScanConversionFactor = nConstants::M_TO_MM;
		break;
	case 1:
		mpScanDistanceLabel->setText(SCAN_DISTANCE_TEXT + "mm)");

		mScanConversionFactor = 1.0;
		break;
	case 2:
		mpScanDistanceLabel->setText(SCAN_DISTANCE_TEXT + "ft)");

		mScanConversionFactor = nConstants::FT_TO_MM;
		break;
	case 3:
		mpScanDistanceLabel->setText(SCAN_DISTANCE_TEXT + "in)");

		mScanConversionFactor = nConstants::IN_TO_MM;
		break;
	}

	distance /= mScanConversionFactor;

	mpScanDistance->setText(QString::number(distance));
}

bool cCreateLidarExperimentFromSpiderCamDlg::generate()
{
	std::string str;
	QString text;
	std::vector<std::string> list;

	std::string title = mpTitle->text().toStdString();
	if (title.empty())
	{
		QString msg = "The \"Measurement Title\" can not be blank.";
		QMessageBox msg_box(QMessageBox::Critical, "Invalid Parameter", msg);
		msg_box.exec();
		return false;
	}

	if (mExperimentTitle.empty())
	{
		auto pos = title.find("_Pass");

		if (pos == std::string::npos)
			mExperimentTitle = title;
		else
			mExperimentTitle = title.substr(0, pos);
	}

	if (mpStartX_mm->text().isEmpty() || mpStartY_mm->text().isEmpty()
		|| mpScanDistance->text().isEmpty())
	{
		QString msg = "The SpiderCam position or scan distance can not be blank.";
		QMessageBox msg_box(QMessageBox::Critical, "Invalid Parameter", msg);
		msg_box.exec();
		return false;
	}

	auto* pGroundModel = cRappGroundModel::get();

	int x1_mm = mpStartX_mm->text().toInt();
	int y1_mm = mpStartY_mm->text().toInt();

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

	int distance_mm = static_cast<int>(mpScanDistance->text().toDouble() * mScanConversionFactor);

	int x2_mm = x1_mm;
	int y2_mm = y1_mm;
	int h2_mm = h1_mm;

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

	if (pGroundModel)
	{
		h2_mm = static_cast<int>(pGroundModel->getMeshHeight_mm(x2_mm, y2_mm));

		if (h2_mm == rfm::INVALID_HEIGHT)
			h2_mm = 0;
	}

	int travel_z_mm = static_cast<int>(mpTravelHeight_m->text().toDouble() * nConstants::M_TO_MM);
	int scan_z_mm = static_cast<int>(mpMeasurementHeight_m->text().toDouble() * nConstants::M_TO_MM);
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

	int dx_mm = x2_mm - x1_mm;
	int dy_mm = y2_mm - y1_mm;

	int vertical_speed_mmps = mpTravelVerticalSpeed_mmps->text().toInt();
	int travel_speed_mmps = mpTravelSpeed_mmps->text().toInt();
	int scan_speed_mmps = mpMeasurementSpeed_mmps->text().toInt();
	int safe_vertical_speed_mmps = mpSafeVerticalSpeed_mmps->text().toInt();

	int start_offset_mm = static_cast<int>(mpBeginningOffset_m->text().toDouble() * nConstants::M_TO_MM);
	int end_offset_mm = static_cast<int>(mpEndingOffset_m->text().toDouble() * nConstants::M_TO_MM);

	double minIntegrationTime_sec = mpMinIntegrationTime_sec->text().toDouble();
	double maxIntegrationTime_sec = mpMaxIntegrationTime_sec->text().toDouble();

	if (maxIntegrationTime_sec < minIntegrationTime_sec)
		std::swap(minIntegrationTime_sec, maxIntegrationTime_sec);

	int errorThreshold_mm = mpErrorThreshold_mm->text().toInt();

	/* Grab the info for multiple scans if selected */
	int startNum = 0;
	bool hasNumber = nStringUtils::endsWithInt(title, &startNum);

	bool mFastMode = false;

	int numOfScans = 1;
	int orientation = 0;
	double separation_mm = 0.0;

	if (mpHasSubScans->isChecked())
	{
		numOfScans = mpNumOfScans->text().toInt();
		orientation = mpSubScanOrientation->currentIndex();
		separation_mm = mpSubScanSeparation->text().toDouble() * mSubScanConversionFactor;
		mFastMode = mpFastMode->isChecked();
	}


	for (int scan = 0; scan < numOfScans; ++scan)
	{
		QSharedPointer<cExperimentFile> pInfo = QSharedPointer<cExperimentFile>(new cExperimentFile());

		if (hasNumber)
			nStringUtils::replaceIntAtEnd(title, startNum++);

		
		pInfo->setMeasurementName(title);
		pInfo->setExperimentName(mExperimentTitle);
		pInfo->setExperimentType(cExperimentFile::eExperimentType::LIDAR);
		pInfo->setMetaData(mMetaInfo);
		pInfo->setController(copy(mCtrlInfo));
		pInfo->setSensors(mSensorInfo);

		// Add preamble: moving dolly up to a safe travel height...
		std::unique_ptr<cMeasurementStep_Movement> step = std::make_unique<cMeasurementStep_Movement>();

		step->setZ_mm(travel_z_mm);
		step->setSpeed_mmps(vertical_speed_mmps);
		pInfo->appendStep(std::move(step));

		if ((dx_mm == 0) && (dy_mm == 0))
		{
			// Moving dolly to the beginning of the measurement scan...
			step = std::make_unique<cMeasurementStep_Movement>();
			step->setX_mm(x1_mm);
			step->setY_mm(y1_mm);
			step->setSpeed_mmps(travel_speed_mmps);
			pInfo->appendStep(std::move(step));


			// Move the dolly to measurement height...
			step = std::make_unique<cMeasurementStep_Movement>();

			if (mpHeightReference->currentIndex() == 1)
				step->setZ_mm(scan_z_mm + h1_mm);
			else
				step->setZ_mm(scan_z_mm);

			step->setSpeed_mmps(vertical_speed_mmps);

			step->setTilt_deg(tilt_deg);
			step->setRoll_deg(roll_deg);
			step->setPan_deg(pan_deg);

			pInfo->appendStep(std::move(step));

			float delay_sec = mpStartMeasurementDelay_sec->text().toFloat();

			if (delay_sec > 0.0)
			{
				// Add delay for dolly to stabilize...
				std::unique_ptr<cMeasurementStep_Delay> delay = std::make_unique<cMeasurementStep_Delay>();
				delay->setWaitTime_sec(delay_sec);
				pInfo->appendStep(std::move(delay));
			}

			// Collect a reference point measurement
			std::unique_ptr<cMeasurementStep_ReferencePoint> reference = std::make_unique<cMeasurementStep_ReferencePoint>();

			reference->setMinIntegrationTime_sec(minIntegrationTime_sec);
			reference->setMaxIntegrationTime_sec(maxIntegrationTime_sec);
			reference->setErrorThreshold_mm(errorThreshold_mm);
			pInfo->appendStep(std::move(reference));

			delay_sec = mpEndMeasurementDelay_sec->text().toFloat();

			if (delay_sec > 0.0)
			{
				// Do measurement...
				std::unique_ptr<cMeasurementStep_Delay> delay = std::make_unique<cMeasurementStep_Delay>();
				delay->setWaitTime_sec(delay_sec);
				delay->setRecording(true);
				pInfo->appendStep(std::move(delay));
			}
		}
		else
		{
			int x_mm = 0;
			int y_mm = 0;

			if (std::abs(dx_mm) < 500)
			{
				x_mm = (x2_mm + x1_mm) / 2;

				if (y1_mm > y2_mm)
					y_mm = y1_mm + start_offset_mm;
				else
					y_mm = y1_mm - start_offset_mm;
			}
			else if (std::abs(dy_mm) < 500)
			{
				if (x1_mm > x2_mm)
					x_mm = x1_mm + start_offset_mm;
				else
					x_mm = x1_mm - start_offset_mm;

				y_mm = (y2_mm + y1_mm) / 2;
			}

			// Moving dolly to the beginning of the measurement scan...
			step = std::make_unique<cMeasurementStep_Movement>();
			step->setX_mm(x_mm);
			step->setY_mm(y_mm);
			step->setSpeed_mmps(travel_speed_mmps);
			pInfo->appendStep(std::move(step));

			// Move the dolly to measurement height...
			step = std::make_unique<cMeasurementStep_Movement>();

			if (mpHeightReference->currentIndex() == 1)
				step->setZ_mm(scan_z_mm + h1_mm);
			else
				step->setZ_mm(scan_z_mm);

			step->setSpeed_mmps(vertical_speed_mmps);

			step->setTilt_deg(tilt_deg);
			step->setRoll_deg(roll_deg);
			step->setPan_deg(pan_deg);

			pInfo->appendStep(std::move(step));

			float delay_sec = mpStartMeasurementDelay_sec->text().toFloat();

			if (delay_sec > 0.0)
			{
				// Add delay for dolly to stabilize...
				std::unique_ptr<cMeasurementStep_Delay> delay = std::make_unique<cMeasurementStep_Delay>();
				delay->setWaitTime_sec(delay_sec);
				pInfo->appendStep(std::move(delay));
			}

			// Collect a reference point measurement
			std::unique_ptr<cMeasurementStep_ReferencePoint> reference = std::make_unique<cMeasurementStep_ReferencePoint>();

			reference->setMinIntegrationTime_sec(minIntegrationTime_sec);
			reference->setMaxIntegrationTime_sec(maxIntegrationTime_sec);
			reference->setErrorThreshold_mm(errorThreshold_mm);
			pInfo->appendStep(std::move(reference));

			if (std::abs(dx_mm) < 500)
			{
				if (y1_mm > y2_mm)
					y_mm = y2_mm - end_offset_mm;
				else
					y_mm = y2_mm + end_offset_mm;
			}
			else if (std::abs(dy_mm) < 500)
			{
				if (x1_mm > x2_mm)
					x_mm = x2_mm - end_offset_mm;
				else
					x_mm = x2_mm + end_offset_mm;
			}

			// Do measurement...
			step = std::make_unique<cMeasurementStep_Movement>();
			step->setX_mm(x_mm);
			step->setY_mm(y_mm);

			if ((mpHeightReference->currentIndex() == 1) &&
				((scan_z_mm + h1_mm) != (scan_z_mm + h2_mm)))
			{
				step->setZ_mm(scan_z_mm + h2_mm);
			}

			step->setSpeed_mmps(scan_speed_mmps);
			step->setRecording(true);
			pInfo->appendStep(std::move(step));

			delay_sec = mpEndMeasurementDelay_sec->text().toFloat();

			if (delay_sec > 0.0)
			{
				// Add delay for dolly to stabilize...
				std::unique_ptr<cMeasurementStep_Delay> delay = std::make_unique<cMeasurementStep_Delay>();
				delay->setWaitTime_sec(delay_sec);
				pInfo->appendStep(std::move(delay));
			}
		}

		// Move dolly to a safe height to park it
		step = std::make_unique<cMeasurementStep_Movement>();
		step->setZ_mm(safe_z_mm);
		step->setSpeed_mmps(safe_vertical_speed_mmps);
		step->setTilt_deg(safe_tilt_deg);
		step->setRoll_deg(safe_roll_deg);
		pInfo->appendStep(std::move(step));

		emit experimentChanged(pInfo);

		switch (orientation)
		{
		case 0:
			x1_mm += separation_mm;
			x2_mm += separation_mm;
			break;
		case 1:
			x1_mm -= separation_mm;
			x2_mm -= separation_mm;
			break;
		case 2:
			y1_mm -= separation_mm;
			y2_mm -= separation_mm;
			break;
		case 3:
			y1_mm += separation_mm;
			y2_mm += separation_mm;
			break;
		}

		if (mFastMode)
		{
			std::swap(x1_mm, x2_mm);
			std::swap(y1_mm, y2_mm);
		}
	}

	return true;
}

void cCreateLidarExperimentFromSpiderCamDlg::onShowPath()
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


	if (mpHasSubScans->isChecked())
	{
		int separation_mm = static_cast<int>(mpSubScanSeparation->text().toDouble() * mSubScanConversionFactor);

		int numOfScans = mpNumOfScans->text().toInt();

		int orientation = mpSubScanOrientation->currentIndex();

		for (int i = 1; i < numOfScans; ++i)
		{
			switch (orientation)
			{
			case 0:
				x1_mm += separation_mm;
				x2_mm += separation_mm;
				break;
			case 1:
				x1_mm -= separation_mm;
				x2_mm -= separation_mm;
				break;
			case 2:
				y1_mm -= separation_mm;
				y2_mm -= separation_mm;
				break;
			case 3:
				y1_mm += separation_mm;
				y2_mm += separation_mm;
				break;
			}

			emit drawPath(x1_mm, y1_mm, x2_mm, y2_mm);
		}
	}
}

void cCreateLidarExperimentFromSpiderCamDlg::recordXY()
{
	if ((mSpidercamX_mm > 0) && (mSpidercamY_mm > 0))
	{
		mpStartX_mm->setText(QString::number(mSpidercamX_mm));
		mpStartY_mm->setText(QString::number(mSpidercamY_mm));
	}
}

void cCreateLidarExperimentFromSpiderCamDlg::positionUpdated(spidercam::sPosition_1_t pos)
{
	mSpidercamX_mm = pos.X_mm;
	mSpidercamY_mm = pos.Y_mm;

	mpSampleXY->setEnabled(true);
}
