
#include "CreateHyperspectralExperimentFromSpiderCamPointDlg.hpp"
#include "Hyperspectral/HySpex/HySpexIDs.hpp"
#include "Constants.hpp"

#include "ExperimentSteps.hpp"
#include "ExperimentSteps_HySpex.hpp"
#include "ExperimentSteps_Ssnx.hpp"

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
#include <QRadioButton>


#include <algorithm>
#include <memory>

namespace
{
	const QString PLOT_LENGTH_TEXT = "Plot Length (";
	const QString ALLEY_LENGTH_TEXT = "Alley Length (";
	const QString SCAN_SEPARATION_TEXT = "Separation (";

	const QString WEST_TO_EAST = "West to East";
	const QString EAST_TO_WEST = "East to West";
	const QString NORTH_TO_SOUTH = "North to South";
	const QString SOUTH_TO_NORTH = "South to North";

	constexpr int PLOT_WEST_TO_EAST = 0;
	constexpr int PLOT_EAST_TO_WEST = 1;
	constexpr int PLOT_NORTH_TO_SOUTH = 2;
	constexpr int PLOT_SOUTH_TO_NORTH = 3;
}

cCreateHyperspectralExperimentFromSpiderCamDlg::cCreateHyperspectralExperimentFromSpiderCamDlg(QWidget* parent)
:
	cCreateHyperspectralExperimentDlg(parent)
{
	setMinimumWidth(550);

	initialize();
}

cCreateHyperspectralExperimentFromSpiderCamDlg::~cCreateHyperspectralExperimentFromSpiderCamDlg()
{}

void cCreateHyperspectralExperimentFromSpiderCamDlg::createControls_PointSelection()
{
	mpStartX_mm = new QLineEdit(this);
	mpStartX_mm->setValidator(new QIntValidator(10000, 190000));

	mpStartY_mm = new QLineEdit(this);
	mpStartY_mm->setValidator(new QIntValidator(10000, 190000));

	mpSampleXY = new QPushButton("Record X, Y", this);
	mpSampleXY->setEnabled(false);
	connect(mpSampleXY, &QPushButton::pressed, this, &cCreateHyperspectralExperimentFromSpiderCamDlg::recordXY);

	mpClearPath = new QPushButton("Clear Path", this);
	connect(mpClearPath, &QPushButton::pressed, this, &cCreateHyperspectralExperimentFromSpiderCamDlg::clearPaths);

	mpShowPath = new QPushButton("Show Path", this);
	connect(mpShowPath, &QPushButton::pressed, this, &cCreateHyperspectralExperimentFromSpiderCamDlg::onShowPath);

	// default to feet
	mpPlotLengthLabel = new QLabel(PLOT_LENGTH_TEXT + "ft)", this);
	mpPlotLength = new QLineEdit(this);
	mpPlotLength->setValidator(new QDoubleValidator(0, 10000.0, 3));
	mpPlotLength->setText("8");

	mpAlleyLengthLabel = new QLabel(ALLEY_LENGTH_TEXT + "ft)", this);
	mpAlleyLength = new QLineEdit(this);
	mpAlleyLength->setValidator(new QDoubleValidator(0, 10000.0, 3));
	mpAlleyLength->setText("0");

	mpPlotOrientation = new QComboBox(this);
	mpPlotOrientation->setEditable(false);
	mpPlotOrientation->addItem(WEST_TO_EAST);
	mpPlotOrientation->addItem(EAST_TO_WEST);
	mpPlotOrientation->addItem(NORTH_TO_SOUTH);
	mpPlotOrientation->addItem(SOUTH_TO_NORTH);

	mpPlotUnits = new QComboBox(this);
	mpPlotUnits->setEditable(false);
	mpPlotUnits->addItem("Meters");
	mpPlotUnits->addItem("Millimeters");
	mpPlotUnits->addItem("Feet");
	mpPlotUnits->addItem("Inches");
	mpPlotUnits->setCurrentIndex(2);
	mPlotConversionFactor = nConstants::FT_TO_MM;
	connect(mpPlotUnits, &QComboBox::currentTextChanged, this, &cCreateHyperspectralExperimentFromSpiderCamDlg::onPlotUnitChange);

	mpNumOfPlots = new QLineEdit(this);
	mpNumOfPlots->setValidator(new QIntValidator(1, 100));
	mpNumOfPlots->setText("1");

	mpStart = new QRadioButton("Start");
	mpCenter = new QRadioButton("Center");
	mpEnd = new QRadioButton("End");

	mpCenter->setChecked(true);
}

void cCreateHyperspectralExperimentFromSpiderCamDlg::createLayout_PointSelection(QVBoxLayout* pMainLayout)
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

	pGroupBox = new QGroupBox(tr("Plot Information"));
	pGroupBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

	mpStart->setParent(pGroupBox);
	mpCenter->setParent(pGroupBox);
	mpEnd->setParent(pGroupBox);

	pVSubLayout = new QVBoxLayout();

	QHBoxLayout* pPlotLayout = new QHBoxLayout();

	pText = new QLabel("Number Of Plots:", this);
	pPlotLayout->addWidget(pText);
	pPlotLayout->addWidget(mpNumOfPlots);
	pPlotLayout->addSpacing(10);
	pPlotLayout->addWidget(mpPlotLengthLabel);
	pPlotLayout->addWidget(mpPlotLength);
	pPlotLayout->addSpacing(10);
	pPlotLayout->addWidget(mpAlleyLengthLabel);
	pPlotLayout->addWidget(mpAlleyLength);
	pPlotLayout->addSpacing(5);
	pText = new QLabel("Orientation:", this);
	pPlotLayout->addWidget(pText);
	pPlotLayout->addWidget(mpPlotOrientation);
	pPlotLayout->addSpacing(10);
	pText = new QLabel("Units:", this);
	pPlotLayout->addWidget(pText);
	pPlotLayout->addWidget(mpPlotUnits);

	pVSubLayout->addLayout(pPlotLayout);

	pPlotLayout = new QHBoxLayout();
	pPlotLayout->addStretch(1);
	pText = new QLabel("Measure:", this);
	pPlotLayout->addWidget(pText);
	pPlotLayout->addSpacing(10);
	pPlotLayout->addWidget(mpStart);
	pPlotLayout->addSpacing(10);
	pPlotLayout->addWidget(mpCenter);
	pPlotLayout->addSpacing(10);
	pPlotLayout->addWidget(mpEnd);
	pPlotLayout->addStretch(1);
	pVSubLayout->addLayout(pPlotLayout);

	pGroupBox->setLayout(pVSubLayout);

	pMainLayout->addWidget(pGroupBox);


	pMainLayout->addSpacing(10);
}

void cCreateHyperspectralExperimentFromSpiderCamDlg::onPlotUnitChange(const QString& text)
{
	double plot_length  = mpPlotLength->text().toDouble() * mPlotConversionFactor;
	double alley_length = mpAlleyLength->text().toDouble() * mPlotConversionFactor;

	switch (mpPlotUnits->currentIndex())
	{
	case 0:
		mpPlotLengthLabel->setText(PLOT_LENGTH_TEXT + "m)");
		mpAlleyLengthLabel->setText(ALLEY_LENGTH_TEXT + "m)");

		mPlotConversionFactor = nConstants::M_TO_MM;
		break;
	case 1:
		mpPlotLengthLabel->setText(PLOT_LENGTH_TEXT + "mm)");
		mpAlleyLengthLabel->setText(ALLEY_LENGTH_TEXT + "mm)");

		mPlotConversionFactor = 1.0;
		break;
	case 2:
		mpPlotLengthLabel->setText(PLOT_LENGTH_TEXT + "ft)");
		mpAlleyLengthLabel->setText(ALLEY_LENGTH_TEXT + "ft)");

		mPlotConversionFactor = nConstants::FT_TO_MM;
		break;
	case 3:
		mpPlotLengthLabel->setText(PLOT_LENGTH_TEXT + "in)");
		mpAlleyLengthLabel->setText(ALLEY_LENGTH_TEXT + "in)");

		mPlotConversionFactor = nConstants::IN_TO_MM;
		break;
	}

	plot_length /= mPlotConversionFactor;
	alley_length /= mPlotConversionFactor;

	mpPlotLength->setText(QString::number(plot_length));
	mpAlleyLength->setText(QString::number(alley_length));
}

bool cCreateHyperspectralExperimentFromSpiderCamDlg::generate()
{
	struct path_t { int x1_mm; int y1_mm; int x2_mm; int y2_mm;};
	std::vector<path_t> path;

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

	if (mpStartX_mm->text().isEmpty() || mpStartY_mm->text().isEmpty()
		|| !isScanDistanceValid())
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

	int numOfPlots = mpNumOfPlots->text().toInt();
	int plot_length_mm = static_cast<int>(mpPlotLength->text().toDouble() * mPlotConversionFactor);
	int alley_length_mm = static_cast<int>(mpAlleyLength->text().toDouble() * mPlotConversionFactor);
	int scan_length_mm = getScanDistance_mm();

	int start_offset_mm = 0;
	int numOfScans = 1;
	int separation_mm = 0;
	bool fastMode = false;
	auto orientation = eSubScanOrientation::NORTH_TO_SOUTH;

	if (mpHasSubScans->isChecked())
	{
		numOfScans = mpNumOfScans->text().toInt();
		int sub_separation_mm = getSubScanSeparation_mm();
		orientation = getSubScanOrientation();
		fastMode = mpFastMode->isChecked();

		if (mpScanEveryRow->isChecked() || (mpScanInsideRows->isChecked() && (numOfScans <= 2)))
		{
			if (numOfScans <= 1)
			{
				numOfScans = 1;
			}
			else
			{
				switch (orientation)
				{
				case eSubScanOrientation::NORTH_TO_SOUTH:
					separation_mm = sub_separation_mm;
					break;
				case eSubScanOrientation::SOUTH_TO_NORTH:
					separation_mm = -sub_separation_mm;
					break;
				case eSubScanOrientation::EAST_TO_WEST:
					separation_mm = -sub_separation_mm;
					break;
				case eSubScanOrientation::WEST_TO_EAST:
					separation_mm = sub_separation_mm;
					break;
				}
			}
		}
		else if (mpScanCenterOnly->isChecked())
		{
			switch (orientation)
			{
			case eSubScanOrientation::NORTH_TO_SOUTH:
				x1_mm += (numOfScans * sub_separation_mm) / 2;
				break;
			case eSubScanOrientation::SOUTH_TO_NORTH:
				x1_mm -= (numOfScans * sub_separation_mm) / 2;
				break;
			case eSubScanOrientation::EAST_TO_WEST:
				y1_mm -= (numOfScans * sub_separation_mm) / 2;
				separation_mm = -sub_separation_mm;
				break;
			case eSubScanOrientation::WEST_TO_EAST:
				y1_mm += (numOfScans * sub_separation_mm) / 2;
				break;
			}

			numOfScans = 1;
		}
		else if (mpScanInsideRows->isChecked())
		{
			numOfScans -= 2;

			switch (orientation)
			{
			case eSubScanOrientation::NORTH_TO_SOUTH:
				x1_mm += sub_separation_mm;
				separation_mm = sub_separation_mm;
				break;
			case eSubScanOrientation::SOUTH_TO_NORTH:
				x1_mm -= sub_separation_mm;
				separation_mm = -sub_separation_mm;
				break;
			case eSubScanOrientation::EAST_TO_WEST:
				y1_mm -= sub_separation_mm;
				separation_mm = -sub_separation_mm;
				break;
			case eSubScanOrientation::WEST_TO_EAST:
				y1_mm += sub_separation_mm;
				separation_mm = sub_separation_mm;
				break;
			}
		}
	}

	if (mpStart->isChecked())
	{
		start_offset_mm = 0;
	}
	else if (mpCenter->isChecked())
	{
		start_offset_mm = (plot_length_mm / 2) - (scan_length_mm / 2);
	}
	else if (mpEnd->isChecked())
	{
		start_offset_mm = plot_length_mm - scan_length_mm;
	}

	int x2_mm = x1_mm;
	int y2_mm = y1_mm;

	switch (mpPlotOrientation->currentIndex())
	{
	case PLOT_WEST_TO_EAST:
		y1_mm += start_offset_mm;
		y2_mm = y1_mm + scan_length_mm;
		break;
	case PLOT_EAST_TO_WEST:
		y1_mm -= start_offset_mm;
		y2_mm = y1_mm - scan_length_mm;
		break;
	case PLOT_NORTH_TO_SOUTH:
		x1_mm += start_offset_mm;
		x2_mm = x1_mm + scan_length_mm;
		break;
	case PLOT_SOUTH_TO_NORTH:
		x1_mm -= start_offset_mm;
		x2_mm = x1_mm - scan_length_mm;
		break;
	}

	int travel_z_mm = static_cast<int>(mpTravelHeight_m->text().toDouble() * nConstants::M_TO_MM);
	int safe_z_mm = static_cast<int>(mpSafeHeight_m->text().toDouble() * nConstants::M_TO_MM);

	int scan_z_offset_mm = static_cast<int>(mpHeightOffset->text().toDouble() * nConstants::M_TO_MM);
	scan_z_offset_mm += mpLensFocalDistance->currentData().toInt();

	double minIntegrationTime_sec = mpMinIntegrationTime_sec->text().toDouble();
	double maxIntegrationTime_sec = mpMaxIntegrationTime_sec->text().toDouble();

	if (maxIntegrationTime_sec < minIntegrationTime_sec)
		std::swap(minIntegrationTime_sec, maxIntegrationTime_sec);

	int errorThreshold_mm = mpErrorThreshold_mm->text().toInt();

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
		scan_z_offset_mm += sensor_offset_mm;
	}


	path.push_back({ x1_mm, y1_mm, x2_mm, y2_mm });

	for (int i = 1; i < numOfPlots; ++i)
	{
		switch (mpPlotOrientation->currentIndex())
		{
		case PLOT_WEST_TO_EAST:
			y1_mm += (plot_length_mm + alley_length_mm);
			y2_mm = y1_mm + scan_length_mm;
			break;
		case PLOT_EAST_TO_WEST:
			y1_mm -= (plot_length_mm + alley_length_mm);
			y2_mm = y1_mm - scan_length_mm;
			break;
		case PLOT_NORTH_TO_SOUTH:
			x1_mm += (plot_length_mm + alley_length_mm);
			x2_mm = x1_mm + scan_length_mm;
			break;
		case PLOT_SOUTH_TO_NORTH:
			x1_mm -= (plot_length_mm + alley_length_mm);
			x2_mm = x1_mm - scan_length_mm;
			break;
		}

		path.push_back({ x1_mm, y1_mm, x2_mm, y2_mm });
	}

	int vertical_speed_mmps = mpTravelVerticalSpeed_mmps->text().toInt();
	int travel_speed_mmps = mpTravelSpeed_mmps->text().toInt();
	int scan_speed_mmps = mpMeasurementSpeed_mmps->text().toInt();
	int safe_vertical_speed_mmps = mpSafeVerticalSpeed_mmps->text().toInt();

	/* Grab the info for multiple scans if selected */
	int startNum = 0;
	bool hasNumber = nStringUtils::endsWithInt(title, &startNum);


	for (int scanNum = 0; scanNum < numOfScans; ++scanNum)
	{
		QSharedPointer<cExperimentFile> pInfo = QSharedPointer<cExperimentFile>(new cExperimentFile());

		if (hasNumber)
			nStringUtils::replaceIntAtEnd(title, startNum++);

		pInfo->setExperimentName(title);
		pInfo->setExperimentType(cExperimentFile::eExperimentType::HYPERSPECTRAL);
		pInfo->setMetaData(mMetaInfo);
		pInfo->setController(copy(mCtrlInfo));
		pInfo->setSensors(mSensorInfo);

		// Add preamble: moving dolly up to a safe travel height...
		std::unique_ptr<cExperimentStep_Movement> step = std::make_unique<cExperimentStep_Movement>();

		step->setZ_mm(travel_z_mm);
		step->setSpeed_mmps(vertical_speed_mmps);
		pInfo->appendStep(std::move(step));

		auto it = path.begin();

		// Moving dolly to the beginning of the measurement scan...
		step = std::make_unique<cExperimentStep_Movement>();
		step->setX_mm(it->x1_mm);
		step->setY_mm(it->y1_mm);
		step->setSpeed_mmps(travel_speed_mmps);
		pInfo->appendStep(std::move(step));

		// Move the dolly to measurement height...
		step = std::make_unique<cExperimentStep_Movement>();

		int h_mm = 0;

		if (pGroundModel)
		{
			h_mm = static_cast<int>(pGroundModel->getMeshHeight_mm(it->x1_mm, it->y1_mm));

			if (h_mm == rfm::INVALID_HEIGHT)
				h_mm = 0;
		}

		h_mm += scan_z_offset_mm;

		step->setZ_mm(h_mm);

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

		// Collect a reference point measurement
		std::unique_ptr<cExperimentStep_ReferencePoint> reference = std::make_unique<cExperimentStep_ReferencePoint>();

		reference->setMinIntegrationTime_sec(minIntegrationTime_sec);
		reference->setMaxIntegrationTime_sec(maxIntegrationTime_sec);
		reference->setErrorThreshold_mm(errorThreshold_mm);
		pInfo->appendStep(std::move(reference));

		for (const auto& sensor : mSensorInfo)
		{
			if (sensor->getType() == vnir_3000N_id)
			{
				// We need to take a background spectra...
				std::unique_ptr<cExperimentStep_HySpex_Command> close_shutter = std::make_unique<cExperimentStep_HySpex_Command>("VNIR-3000N", "close shutter");
				pInfo->appendStep(std::move(close_shutter));

				std::unique_ptr<cExperimentStep_HySpex_Command> background = std::make_unique<cExperimentStep_HySpex_Command>("VNIR-3000N", "background");
				pInfo->appendStep(std::move(background));

				std::unique_ptr<cExperimentStep_HySpex_Command> open_shutter = std::make_unique<cExperimentStep_HySpex_Command>("VNIR-3000N", "open shutter");
				pInfo->appendStep(std::move(open_shutter));
			}

			if (sensor->getType() == swir_384_id)
			{
				// We need to take a background spectra...
				std::unique_ptr<cExperimentStep_HySpex_Command> close_shutter = std::make_unique<cExperimentStep_HySpex_Command>("SWIR-384", "close shutter");
				pInfo->appendStep(std::move(close_shutter));

				std::unique_ptr<cExperimentStep_HySpex_Command> background = std::make_unique<cExperimentStep_HySpex_Command>("SWIR-384", "background");
				pInfo->appendStep(std::move(background));

				std::unique_ptr<cExperimentStep_HySpex_Command> open_shutter = std::make_unique<cExperimentStep_HySpex_Command>("SWIR-384", "open shutter");
				pInfo->appendStep(std::move(open_shutter));
			}
		}

		h_mm = 0;

		if (pGroundModel)
		{
			h_mm = static_cast<int>(pGroundModel->getMeshHeight_mm(it->x2_mm, it->y2_mm));

			if (h_mm == rfm::INVALID_HEIGHT)
				h_mm = 0;
		}

		h_mm += scan_z_offset_mm;

		// Do measurement...
		step = std::make_unique<cExperimentStep_Movement>();
		step->setX_mm(it->x2_mm);
		step->setY_mm(it->y2_mm);

		step->setZ_mm(h_mm);

		step->setSpeed_mmps(scan_speed_mmps);
		step->setRecording(true);
		pInfo->appendStep(std::move(step));

		for (; it != path.end(); ++it)
		{
			// Move to next measurement...
			step = std::make_unique<cExperimentStep_Movement>();
			step->setX_mm(it->x1_mm);
			step->setY_mm(it->y1_mm);

			h_mm = 0;

			if (pGroundModel)
			{
				h_mm = static_cast<int>(pGroundModel->getMeshHeight_mm(it->x1_mm, it->y1_mm));

				if (h_mm == rfm::INVALID_HEIGHT)
					h_mm = 0;
			}

			h_mm += scan_z_offset_mm;

			step->setZ_mm(h_mm);

			step->setSpeed_mmps(scan_speed_mmps);
			step->setRecording(false);
			pInfo->appendStep(std::move(step));

			// Do measurement...
			step = std::make_unique<cExperimentStep_Movement>();
			step->setX_mm(it->x2_mm);
			step->setY_mm(it->y2_mm);

			h_mm = 0;

			if (pGroundModel)
			{
				h_mm = static_cast<int>(pGroundModel->getMeshHeight_mm(it->x2_mm, it->y2_mm));

				if (h_mm == rfm::INVALID_HEIGHT)
					h_mm = 0;
			}

			h_mm += scan_z_offset_mm;

			step->setZ_mm(h_mm);

			step->setSpeed_mmps(scan_speed_mmps);
			step->setRecording(true);
			pInfo->appendStep(std::move(step));
		}

		delay_sec = mpEndMeasurementDelay_sec->text().toFloat();

		if (delay_sec > 0.0)
		{
			// Add delay for dolly to stabilize...
			std::unique_ptr<cExperimentStep_Delay> delay = std::make_unique<cExperimentStep_Delay>();
			delay->setWaitTime_sec(delay_sec);
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

		for (auto& p : path)
		{
			switch (orientation)
			{
			case eSubScanOrientation::NORTH_TO_SOUTH:
			case eSubScanOrientation::SOUTH_TO_NORTH:
				p.x1_mm += separation_mm;
				p.x2_mm += separation_mm;
				break;
			case eSubScanOrientation::EAST_TO_WEST:
			case eSubScanOrientation::WEST_TO_EAST:
				p.y1_mm += separation_mm;
				p.y2_mm += separation_mm;
				break;
			}

			if (fastMode)
			{
				std::swap(p.x1_mm, p.x2_mm);
				std::swap(p.y1_mm, p.y2_mm);
			}
		}

		if (fastMode)
			std::reverse(path.begin(), path.end());
	}

	return true;
}

void cCreateHyperspectralExperimentFromSpiderCamDlg::onShowPath()
{
	emit clearPaths();

	struct path_t { int x1_mm; int y1_mm; int x2_mm; int y2_mm; };
	std::vector<path_t> path;

	int numOfPlots = mpNumOfPlots->text().toInt();
	int x1_mm = mpStartX_mm->text().toInt();
	int y1_mm = mpStartY_mm->text().toInt();

	int plot_length_mm = static_cast<int>(mpPlotLength->text().toDouble() * mPlotConversionFactor);
	int alley_length_mm = static_cast<int>(mpAlleyLength->text().toDouble() * mPlotConversionFactor);
	int scan_length_mm = getScanDistance_mm();

	int start_offset_mm = 0;
	int numOfScans = 0;
	int separation_mm = 0;

	if (mpHasSubScans->isChecked())
	{
		numOfScans = mpNumOfScans->text().toInt();
		int sub_separation_mm = getSubScanSeparation_mm();
		auto orientation = getSubScanOrientation();

		if (mpScanEveryRow->isChecked() || (mpScanInsideRows->isChecked() && (numOfScans <= 2)))
		{
			if (numOfScans <= 1)
			{
				numOfScans = 0;
			}
			else
			{
				numOfScans -= 1;

				switch (orientation)
				{
				case eSubScanOrientation::NORTH_TO_SOUTH:
					separation_mm = sub_separation_mm;
					break;
				case eSubScanOrientation::SOUTH_TO_NORTH:
					separation_mm = -sub_separation_mm;
					break;
				case eSubScanOrientation::EAST_TO_WEST:
					separation_mm = -sub_separation_mm;
					break;
				case eSubScanOrientation::WEST_TO_EAST:
					separation_mm = sub_separation_mm;
					break;
				}
			}
		}
		else if (mpScanCenterOnly->isChecked())
		{
			switch (orientation)
			{
			case eSubScanOrientation::NORTH_TO_SOUTH:
				x1_mm += (numOfScans * sub_separation_mm) / 2;
				break;
			case eSubScanOrientation::SOUTH_TO_NORTH:
				x1_mm -= (numOfScans * sub_separation_mm) / 2;
				break;
			case eSubScanOrientation::EAST_TO_WEST:
				y1_mm -= (numOfScans * sub_separation_mm) / 2;
				separation_mm = -sub_separation_mm;
				break;
			case eSubScanOrientation::WEST_TO_EAST:
				y1_mm += (numOfScans * sub_separation_mm) / 2;
				break;
			}

			numOfScans = 0;
		}
		else if (mpScanInsideRows->isChecked())
		{
			numOfScans -= 2;

			switch (orientation)
			{
			case eSubScanOrientation::NORTH_TO_SOUTH:
				x1_mm += sub_separation_mm;
				separation_mm = sub_separation_mm;
				break;
			case eSubScanOrientation::SOUTH_TO_NORTH:
				x1_mm -= sub_separation_mm;
				separation_mm = -sub_separation_mm;
				break;
			case eSubScanOrientation::EAST_TO_WEST:
				y1_mm -= sub_separation_mm;
				separation_mm = -sub_separation_mm;
				break;
			case eSubScanOrientation::WEST_TO_EAST:
				y1_mm += sub_separation_mm;
				separation_mm = sub_separation_mm;
				break;
			}
		}
	}

	if (mpStart->isChecked())
	{
		start_offset_mm = 0;
	}
	else if (mpCenter->isChecked())
	{
		start_offset_mm = (plot_length_mm / 2) - (scan_length_mm / 2);
	}
	else if (mpEnd->isChecked())
	{
		start_offset_mm = plot_length_mm - scan_length_mm;
	}

	int x2_mm = x1_mm;
	int y2_mm = y1_mm;

	switch (mpPlotOrientation->currentIndex())
	{
	case PLOT_WEST_TO_EAST:
		y1_mm += start_offset_mm;
		y2_mm = y1_mm + scan_length_mm;
		break;
	case PLOT_EAST_TO_WEST:
		y1_mm -= start_offset_mm;
		y2_mm = y1_mm - scan_length_mm;
		break;
	case PLOT_NORTH_TO_SOUTH:
		x1_mm += start_offset_mm;
		x2_mm = x1_mm + scan_length_mm;
		break;
	case PLOT_SOUTH_TO_NORTH:
		x1_mm -= start_offset_mm;
		x2_mm = x1_mm - scan_length_mm;
		break;
	}

	emit drawPath(x1_mm, y1_mm, x2_mm, y2_mm);

	path.push_back({ x1_mm, y1_mm, x2_mm, y2_mm });

	for (int i = 1; i < numOfPlots; ++i)
	{
		switch (mpPlotOrientation->currentIndex())
		{
		case PLOT_WEST_TO_EAST:
			y1_mm += (plot_length_mm + alley_length_mm);
			y2_mm = y1_mm + scan_length_mm;
			break;
		case PLOT_EAST_TO_WEST:
			y1_mm -= (plot_length_mm + alley_length_mm);
			y2_mm = y1_mm - scan_length_mm;
			break;
		case PLOT_NORTH_TO_SOUTH:
			x1_mm += (plot_length_mm + alley_length_mm);
			x2_mm = x1_mm + scan_length_mm;
			break;
		case PLOT_SOUTH_TO_NORTH:
			x1_mm -= (plot_length_mm + alley_length_mm);
			x2_mm = x1_mm - scan_length_mm;
			break;
		}

		emit drawPath(x1_mm, y1_mm, x2_mm, y2_mm);

		path.push_back({ x1_mm, y1_mm, x2_mm, y2_mm });
	}

	if (mpHasSubScans->isChecked())
	{
		auto orientation = getSubScanOrientation();

		for (int i = 1; i < numOfScans; ++i)
		{
			for (auto& p : path)
			{
				switch (orientation)
				{
				case eSubScanOrientation::NORTH_TO_SOUTH:
				case eSubScanOrientation::SOUTH_TO_NORTH:
					p.x1_mm += separation_mm;
					p.x2_mm += separation_mm;
					break;
				case eSubScanOrientation::EAST_TO_WEST:
				case eSubScanOrientation::WEST_TO_EAST:
					p.y1_mm += separation_mm;
					p.y2_mm += separation_mm;
					break;
				}

				emit drawPath(p.x1_mm, p.y1_mm, p.x2_mm, p.y2_mm);
			}
		}
	}
}

void cCreateHyperspectralExperimentFromSpiderCamDlg::recordXY()
{
	if ((mSpidercamX_mm > 0) && (mSpidercamY_mm > 0))
	{
		mpStartX_mm->setText(QString::number(mSpidercamX_mm));
		mpStartY_mm->setText(QString::number(mSpidercamY_mm));
	}
}

void cCreateHyperspectralExperimentFromSpiderCamDlg::positionUpdated(spidercam::sPosition_1_t pos)
{
	mSpidercamX_mm = pos.X_mm;
	mSpidercamY_mm = pos.Y_mm;

	mpSampleXY->setEnabled(true);
}
