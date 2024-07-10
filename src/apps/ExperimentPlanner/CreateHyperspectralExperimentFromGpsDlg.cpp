
#include "CreateHyperspectralExperimentFromGpsDlg.hpp"
#include "Hyperspectral/HySpex/HySpexFactory.hpp"
#include "GpsFileReader.hpp"
#include "Constants.hpp"

#include "ExperimentSteps.hpp"
#include "ExperimentSteps_HySpex.hpp"

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
	const QString PLOT_LENGTH_TEXT = "Plot Length (";
	const QString ALLEY_LENGTH_TEXT = "Alley Length (";

	const QString WEST_TO_EAST = "West to East";
	const QString EAST_TO_WEST = "East to West";
	const QString NORTH_TO_SOUTH = "North to South";
	const QString SOUTH_TO_NORTH = "South to North";

	constexpr int SCAN_WEST_TO_EAST = 0;
	constexpr int SCAN_EAST_TO_WEST = 1;
	constexpr int SCAN_NORTH_TO_SOUTH = 2;
	constexpr int SCAN_SOUTH_TO_NORTH = 3;
}

cCreateHyperspectralExperimentFromGpsDlg::cCreateHyperspectralExperimentFromGpsDlg(const QString& filename, QWidget* parent)
:
	cCreateHyperspectralExperimentDlg(parent)
{
	setMinimumWidth(550);

	mpModel = new QStandardItemModel(10, 4, this);
	mpModel->setHeaderData(0, Qt::Horizontal, tr("Name"));
	mpModel->setHeaderData(1, Qt::Horizontal, tr("X (m)"));
	mpModel->setHeaderData(2, Qt::Horizontal, tr("Y (m)"));
	mpModel->setHeaderData(3, Qt::Horizontal, tr("Z (m)"));

	{
		cGpsFileReader reader;
		reader.loadFromFile(filename.toStdString());
		auto points = reader.GetPoints();

		auto n = points.size();

		for (int i = 0; i < n; ++i)
		{
			const auto& pos = points[i];

			mpModel->insertRows(i, 1, QModelIndex());
			mpModel->setData(mpModel->index(i, 0, QModelIndex()), QString::fromStdString(pos.label));
			mpModel->setData(mpModel->index(i, 1, QModelIndex()), pos.x_m);
			mpModel->setData(mpModel->index(i, 2, QModelIndex()), pos.y_m);
			mpModel->setData(mpModel->index(i, 3, QModelIndex()), pos.z_m);
		}
	}

	initialize();
}

cCreateHyperspectralExperimentFromGpsDlg::~cCreateHyperspectralExperimentFromGpsDlg()
{}

void cCreateHyperspectralExperimentFromGpsDlg::createControls_PointSelection()
{
	mpStartPosition = new QTableView(this);
	mpStartPosition->setModel(mpModel);

	mpStartPosition->verticalHeader()->hide();

	QHeaderView* headerView = mpStartPosition->horizontalHeader();
	headerView->setDefaultAlignment(Qt::AlignHCenter);
	headerView->setStretchLastSection(false);

	mpStartPosition->setHorizontalScrollMode(QAbstractItemView::ScrollPerItem);
	mpStartPosition->setSelectionBehavior(QAbstractItemView::SelectRows);
	mpStartPosition->setSelectionMode(QAbstractItemView::SingleSelection);
	mpStartPosition->setSortingEnabled(false);
	mpStartPosition->setFixedWidth(419);

	mpEndPosition = new QTableView(this);
	mpEndPosition->setModel(mpModel);

	mpEndPosition->verticalHeader()->hide();

	headerView = mpEndPosition->horizontalHeader();
	headerView->setDefaultAlignment(Qt::AlignHCenter);
	headerView->setStretchLastSection(false);

	mpEndPosition->setHorizontalScrollMode(QAbstractItemView::ScrollPerItem);
	mpEndPosition->setSelectionBehavior(QAbstractItemView::SelectRows);
	mpEndPosition->setSelectionMode(QAbstractItemView::SingleSelection);
	mpEndPosition->setSortingEnabled(false);
	mpEndPosition->setFixedWidth(419);

	mpClearPath = new QPushButton("Clear Path", this);
	connect(mpClearPath, &QPushButton::pressed, this, &cCreateHyperspectralExperimentFromGpsDlg::clearPaths);

	mpShowPath = new QPushButton("Show Path", this);
	connect(mpShowPath, &QPushButton::pressed, this, &cCreateHyperspectralExperimentFromGpsDlg::onShowPath);

	mpInverseDirection = new QCheckBox("Inverse Direction", this);

	// default to feet
	mpPlotLengthLabel = new QLabel(PLOT_LENGTH_TEXT + "ft)", this);
	mpPlotLength = new QLineEdit(this);
	mpPlotLength->setValidator(new QDoubleValidator(1, 10000.0, 3));
	mpPlotLength->setText("8");

	mpAlleyLengthLabel = new QLabel(ALLEY_LENGTH_TEXT + "ft)", this);
	mpAlleyLength = new QLineEdit(this);
	mpAlleyLength->setValidator(new QDoubleValidator(0, 10000.0, 3));
	mpAlleyLength->setText("0");

/*
	mpPlotOrientation = new QComboBox(this);
	mpPlotOrientation->setEditable(false);
	mpPlotOrientation->addItem(WEST_TO_EAST);
	mpPlotOrientation->addItem(EAST_TO_WEST);
	mpPlotOrientation->addItem(NORTH_TO_SOUTH);
	mpPlotOrientation->addItem(SOUTH_TO_NORTH);
*/

	mpPlotUnits = new QComboBox(this);
	mpPlotUnits->setEditable(false);
	mpPlotUnits->addItem("Meters");
	mpPlotUnits->addItem("Millimeters");
	mpPlotUnits->addItem("Feet");
	mpPlotUnits->addItem("Inches");
	mpPlotUnits->setCurrentIndex(2);
	mPlotConversionFactor = nConstants::FT_TO_MM;
	connect(mpPlotUnits, &QComboBox::currentTextChanged, this, &cCreateHyperspectralExperimentFromGpsDlg::onPlotUnitChange);

	mpMeasureFrom = new QGroupBox(this);
	mpStart = new QRadioButton("Start", mpMeasureFrom);
	mpCenter = new QRadioButton("Center", mpMeasureFrom);
	mpEnd = new QRadioButton("End", mpMeasureFrom);

	mpCenter->setChecked(true);
}

void cCreateHyperspectralExperimentFromGpsDlg::createLayout_PointSelection(QVBoxLayout* pMainLayout)
{
	QLabel* pText = nullptr;
	QGroupBox* pGroupBox = nullptr;
	QGridLayout* pGridLayout = nullptr;

	QHBoxLayout* pPosLayout = new QHBoxLayout();

	pPosLayout->addStretch(1);
	pPosLayout->addWidget(mpStartPosition);
	pPosLayout->addSpacing(10);
	pPosLayout->addWidget(mpEndPosition);
	pPosLayout->addSpacing(10);

	QVBoxLayout* pVSubLayout = new QVBoxLayout();
	pVSubLayout->addWidget(mpClearPath);
	pVSubLayout->addWidget(mpShowPath);
	pPosLayout->addLayout(pVSubLayout);
	pPosLayout->addStretch(1);

	pMainLayout->addLayout(pPosLayout);

	QHBoxLayout* pOptionsLayout = new QHBoxLayout();

	pOptionsLayout->addStretch(1);
	pOptionsLayout->addWidget(mpInverseDirection);
	pOptionsLayout->addStretch(1);

	pMainLayout->addLayout(pOptionsLayout);

	pGroupBox = new QGroupBox(tr("Plot Information"));
	pGroupBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

	pVSubLayout = new QVBoxLayout();

	QHBoxLayout* pPlotLayout = new QHBoxLayout();

	pPlotLayout->addWidget(mpPlotLengthLabel);
	pPlotLayout->addWidget(mpPlotLength);
	pPlotLayout->addSpacing(10);
	pPlotLayout->addWidget(mpAlleyLengthLabel);
	pPlotLayout->addWidget(mpAlleyLength);
	pPlotLayout->addSpacing(10);
	pText = new QLabel("Units:", this);
	pPlotLayout->addWidget(pText);
	pPlotLayout->addWidget(mpPlotUnits);
	//	pOptionsLayout->addWidget(mpPlotOrientation);

	pVSubLayout->addLayout(pPlotLayout);

	pPlotLayout = new QHBoxLayout();
	pPlotLayout->addStretch(1);
	pText = new QLabel("Measure From:", this);
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

void cCreateHyperspectralExperimentFromGpsDlg::onPlotUnitChange(const QString& text)
{
	double plot_length = mpPlotLength->text().toDouble() * mPlotConversionFactor;
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

bool cCreateHyperspectralExperimentFromGpsDlg::generate()
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

	QModelIndex startIndex = mpStartPosition->currentIndex();
	QModelIndex endIndex = mpEndPosition->currentIndex();

	if ((startIndex.row() < 0) || (endIndex.row() < 0))
	{
		QString msg = "Please select start and end positions.";
		QMessageBox msg_box(QMessageBox::Critical, "Invalid Parameter", msg);
		msg_box.exec();
		return false;
	}

	int plot_length_mm = static_cast<int>(mpPlotLength->text().toDouble() * mPlotConversionFactor);

	if (plot_length_mm <= 0)
	{
		QString msg = "Plot length has to be greater than zero!";
		QMessageBox msg_box(QMessageBox::Critical, "Invalid Parameter", msg);
		msg_box.exec();
		return false;
	}

	auto x1 = mpModel->data(startIndex.siblingAtColumn(1)).toFloat();
	auto y1 = mpModel->data(startIndex.siblingAtColumn(2)).toFloat();
	auto h1 = mpModel->data(startIndex.siblingAtColumn(3)).toFloat();

	auto x2 = mpModel->data(endIndex.siblingAtColumn(1)).toFloat();
	auto y2 = mpModel->data(endIndex.siblingAtColumn(2)).toFloat();
	auto h2 = mpModel->data(endIndex.siblingAtColumn(3)).toFloat();

	if (mpInverseDirection->isChecked())
	{
		std::swap(x1, x2);
		std::swap(y1, y2);
		std::swap(h1, h2);
	}

	int x1_mm = static_cast<int>(x1 * nConstants::M_TO_MM);
	int y1_mm = static_cast<int>(y1 * nConstants::M_TO_MM);
	int h1_mm = static_cast<int>(h1 * nConstants::M_TO_MM);
	int x2_mm = static_cast<int>(x2 * nConstants::M_TO_MM);
	int y2_mm = static_cast<int>(y2 * nConstants::M_TO_MM);
	int h2_mm = static_cast<int>(h2 * nConstants::M_TO_MM);

	int distance_mm = static_cast<int>(sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1)) * nConstants::M_TO_MM);
	int alley_length_mm = static_cast<int>(mpAlleyLength->text().toDouble() * mPlotConversionFactor);
	int numMeasurements = distance_mm / (plot_length_mm + alley_length_mm);

	int travel_z_mm = static_cast<int>(mpTravelHeight_m->text().toDouble() * nConstants::M_TO_MM);
	int safe_z_mm = static_cast<int>(mpSafeHeight_m->text().toDouble() * nConstants::M_TO_MM);

	int scan_z_offset_mm = static_cast<int>(mpHeightOffset->text().toDouble() * nConstants::M_TO_MM);
	scan_z_offset_mm += mpLensFocalDistance->currentData().toInt();

/*
	if (pGroundModel)
	{
		h2_mm = static_cast<int>(pGroundModel->getMeshHeight_mm(x2_mm, y2_mm));

		if (h2_mm == rfm::INVALID_HEIGHT)
			h2_mm = 0;
	}
*/

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

	int vertical_speed_mmps = mpTravelVerticalSpeed_mmps->text().toInt();
	int travel_speed_mmps = mpTravelSpeed_mmps->text().toInt();
	int scan_speed_mmps = mpMeasurementSpeed_mmps->text().toInt();
	int safe_vertical_speed_mmps = mpSafeVerticalSpeed_mmps->text().toInt();

	int start_offset_mm = 0;
	
	int scan_distance_mm = getScanDistance_mm();

	if (mpStart->isChecked())
	{
		if (mpInverseDirection->isChecked())
			start_offset_mm = plot_length_mm - scan_distance_mm;
		else
			start_offset_mm = 0;
	}
	else if (mpCenter->isChecked())
	{
		start_offset_mm = (plot_length_mm / 2) - (scan_distance_mm / 2);
	}
	else if (mpEnd->isChecked())
	{
		if (mpInverseDirection->isChecked())
			start_offset_mm = 0;
		else
			start_offset_mm = plot_length_mm - scan_distance_mm;
	}

	/* Grab the info for multiple scans if selected */
	int startNum = 0;
	bool hasNumber = nStringUtils::endsWithInt(title, &startNum);

	bool mFastMode = false;

	int numOfScans = 1;
	eSubScanOrientation orientation = eSubScanOrientation::NORTH_TO_SOUTH;
	double separation_mm = 0.0;
	int lateral_offset_mm = 0;

	if (mpHasSubScans->isChecked())
	{
		numOfScans = mpNumOfScans->text().toInt();
		orientation = getSubScanOrientation();
		separation_mm = getSubScanSeparation_mm();
		mFastMode = mpFastMode->isChecked();

		if (mpScanCenterOnly->isChecked() && (numOfScans > 1))
		{
			double middle_offset_mm = (separation_mm * (numOfScans - 1)) / 2.0;

			switch (orientation)
			{
			case eSubScanOrientation::NORTH_TO_SOUTH:
				lateral_offset_mm = static_cast<int>(middle_offset_mm);
				break;

			case eSubScanOrientation::SOUTH_TO_NORTH:
				lateral_offset_mm = -1 * static_cast<int>(middle_offset_mm);
				break;

			case eSubScanOrientation::EAST_TO_WEST:
				lateral_offset_mm = -1 * static_cast<int>(middle_offset_mm);
				break;

			case eSubScanOrientation::WEST_TO_EAST:
				lateral_offset_mm = static_cast<int>(middle_offset_mm);
				break;
			}

			separation_mm = 0.0;
			numOfScans = 1;
		}

		if (mpScanInsideRows->isChecked() && (numOfScans > 2))
		{
			numOfScans -= 2;

			switch (orientation)
			{
			case eSubScanOrientation::NORTH_TO_SOUTH:
				x1_mm += separation_mm;
				x2_mm += separation_mm;
				break;

			case eSubScanOrientation::SOUTH_TO_NORTH:
				x1_mm -= separation_mm;
				x2_mm -= separation_mm;
				break;

			case eSubScanOrientation::EAST_TO_WEST:
				y1_mm -= separation_mm;
				y2_mm -= separation_mm;
				break;

			case eSubScanOrientation::WEST_TO_EAST:
				y1_mm += separation_mm;
				y2_mm += separation_mm;
				break;
			}
		}
	}

	// We use dx and dy to determine direction of scan
	int dx_mm = x2_mm - x1_mm;
	int dy_mm = y2_mm - y1_mm;
	auto* pGroundModel = cRappGroundModel::get();

	for (int scan = 0; scan < numOfScans; ++scan)
	{
		QSharedPointer<cExperimentFile> pInfo = QSharedPointer<cExperimentFile>(new cExperimentFile());

		if (hasNumber)
			nStringUtils::replaceIntAtEnd(title, startNum++);

		pInfo->setExperimentName(title);
		pInfo->setMetaData(mMetaInfo);
		pInfo->setController(copy(mCtrlInfo));
		pInfo->setSensors(mSensorInfo);

		// Add preamble: moving dolly up to a safe travel height...
		std::unique_ptr<cExperimentStep_Movement> step = std::make_unique<cExperimentStep_Movement>();

		step->setZ_mm(travel_z_mm);
		step->setSpeed_mmps(vertical_speed_mmps);
		pInfo->appendStep(std::move(step));

		if ((dx_mm == 0) && (dy_mm == 0))
		{
			// Moving dolly to the beginning of the measurement scan...
			step = std::make_unique<cExperimentStep_Movement>();
			step->setX_mm(x1_mm);
			step->setY_mm(y1_mm);
			step->setSpeed_mmps(travel_speed_mmps);
			pInfo->appendStep(std::move(step));


			// Move the dolly to measurement height...
			step = std::make_unique<cExperimentStep_Movement>();

			if (pGroundModel)
			{
				auto h_mm = static_cast<int>(pGroundModel->getMeshHeight_mm(x1_mm, y1_mm));

				if (h_mm != rfm::INVALID_HEIGHT)
					h1_mm = h_mm;
			}

			step->setZ_mm(scan_z_offset_mm + h1_mm);

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

			delay_sec = mpEndMeasurementDelay_sec->text().toFloat();

			if (delay_sec > 0.0)
			{
				// Do measurement...
				std::unique_ptr<cExperimentStep_Delay> delay = std::make_unique<cExperimentStep_Delay>();
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
				x_mm += lateral_offset_mm;

				if (y1_mm > y2_mm)
					y_mm = y1_mm - start_offset_mm;
				else
					y_mm = y1_mm + start_offset_mm;
			}
			else if (std::abs(dy_mm) < 500)
			{
				if (x1_mm > x2_mm)
					x_mm = x1_mm - start_offset_mm;
				else
					x_mm = x1_mm + start_offset_mm;

				y_mm = (y2_mm + y1_mm) / 2;
				y_mm += lateral_offset_mm;
			}

			// Moving dolly to the beginning of the measurement scan...
			step = std::make_unique<cExperimentStep_Movement>();
			step->setX_mm(x_mm);
			step->setY_mm(y_mm);
			step->setSpeed_mmps(travel_speed_mmps);
			pInfo->appendStep(std::move(step));

			// Move the dolly to measurement height...
			step = std::make_unique<cExperimentStep_Movement>();

			if (pGroundModel)
			{
				auto h_mm = static_cast<int>(pGroundModel->getMeshHeight_mm(x_mm, y_mm));

				if (h_mm != rfm::INVALID_HEIGHT)
					h1_mm = h_mm;
			}

			step->setZ_mm(scan_z_offset_mm + h1_mm);

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

			for (int n = 0; n < numMeasurements; ++n)
			{
				if (std::abs(dx_mm) < 500)
				{
					if (y1_mm > y2_mm)
						y_mm -= scan_distance_mm;
					else
						y_mm += scan_distance_mm;
				}
				else if (std::abs(dy_mm) < 500)
				{
					if (x1_mm > x2_mm)
						x_mm -= scan_distance_mm;
					else
						x_mm += scan_distance_mm;
				}

				// Do measurement...
				step = std::make_unique<cExperimentStep_Movement>();
				step->setX_mm(x_mm);
				step->setY_mm(y_mm);

				if (pGroundModel)
				{
					auto h_mm = static_cast<int>(pGroundModel->getMeshHeight_mm(x_mm, y_mm));

					if (h_mm != rfm::INVALID_HEIGHT)
						h2_mm = h_mm;
				}

				step->setZ_mm(scan_z_offset_mm + h2_mm);

				step->setSpeed_mmps(scan_speed_mmps);
				step->setRecording(true);
				pInfo->appendStep(std::move(step));

				// Advance to the next measurement...
				if (std::abs(dx_mm) < 500)
				{
					if (y1_mm > y2_mm)
					{
						y1_mm -= (plot_length_mm + alley_length_mm);
						y_mm = y1_mm - start_offset_mm;

						if (y_mm <= y2_mm)
							break;
					}
					else
					{
						y1_mm += (plot_length_mm + alley_length_mm);
						y_mm = y1_mm + start_offset_mm;

						if (y_mm >= y2_mm)
							break;
					}
				}
				else if (std::abs(dy_mm) < 500)
				{
					if (x1_mm > x2_mm)
					{
						x1_mm -= (plot_length_mm + alley_length_mm);
						x_mm = x1_mm - start_offset_mm;

						if (x_mm <= x2_mm)
							break;
					}
					else
					{
						x1_mm += (plot_length_mm + alley_length_mm);
						x_mm = x1_mm + start_offset_mm;

						if (x_mm >= x2_mm)
							break;
					}
				}

				// Move to next measurement...
				step = std::make_unique<cExperimentStep_Movement>();
				step->setX_mm(x_mm);
				step->setY_mm(y_mm);

				if (pGroundModel)
				{
					auto h_mm = static_cast<int>(pGroundModel->getMeshHeight_mm(x_mm, y_mm));

					if (h_mm != rfm::INVALID_HEIGHT)
						h2_mm = h_mm;
				}

				step->setZ_mm(scan_z_offset_mm + h2_mm);

				step->setSpeed_mmps(scan_speed_mmps);
				step->setRecording(false);
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
		}

		// Move dolly to a safe height to park it
		step = std::make_unique<cExperimentStep_Movement>();
		step->setZ_mm(safe_z_mm);
		step->setSpeed_mmps(safe_vertical_speed_mmps);
		step->setTilt_deg(safe_tilt_deg);
		step->setRoll_deg(safe_roll_deg);
		pInfo->appendStep(std::move(step));

		emit experimentChanged(pInfo);

		switch (orientation)
		{
		case eSubScanOrientation::NORTH_TO_SOUTH:
			x1_mm += separation_mm;
			x2_mm += separation_mm;
			break;
		case eSubScanOrientation::SOUTH_TO_NORTH:
			x1_mm -= separation_mm;
			x2_mm -= separation_mm;
			break;
		case eSubScanOrientation::WEST_TO_EAST:
			y1_mm -= separation_mm;
			y2_mm -= separation_mm;
			break;
		case eSubScanOrientation::EAST_TO_WEST:
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

void cCreateHyperspectralExperimentFromGpsDlg::onShowPath()
{
	QModelIndex startIndex = mpStartPosition->currentIndex();
	if (startIndex.row() < 0)
		return;

	QModelIndex endIndex = mpEndPosition->currentIndex();
	if (endIndex.row() < 0)
		return;

	auto x1 = mpModel->data(startIndex.siblingAtColumn(1)).toFloat();
	auto y1 = mpModel->data(startIndex.siblingAtColumn(2)).toFloat();

	auto x2 = mpModel->data(endIndex.siblingAtColumn(1)).toFloat();
	auto y2 = mpModel->data(endIndex.siblingAtColumn(2)).toFloat();

	int x1_mm = static_cast<int>(x1 * nConstants::M_TO_MM);
	int y1_mm = static_cast<int>(y1 * nConstants::M_TO_MM);
	int x2_mm = static_cast<int>(x2 * nConstants::M_TO_MM);
	int y2_mm = static_cast<int>(y2 * nConstants::M_TO_MM);


/*
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
			case SUB_SCAN_NORTH_TO_SOUTH:
				x1_mm += separation_mm;
				x2_mm += separation_mm;
				break;

			case SUB_SCAN_SOUTH_TO_NORTH:
				x1_mm -= separation_mm;
				x2_mm -= separation_mm;
				break;

			case SUB_SCAN_EAST_TO_WEST:
				y1_mm -= separation_mm;
				y2_mm -= separation_mm;
				break;

			case SUB_SCAN_WEST_TO_EAST:
				y1_mm += separation_mm;
				y2_mm += separation_mm;
				break;
			}

			emit drawPath(x1_mm, y1_mm, x2_mm, y2_mm);
		}
	}
*/

	int distance_mm = 0;

/*
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
*/


	emit drawPath(x1_mm, y1_mm, x2_mm, y2_mm);


	if (mpHasSubScans->isChecked())
	{
		int separation_mm = getSubScanSeparation_mm();

		int numOfScans = mpNumOfScans->text().toInt();

		auto orientation = getSubScanOrientation();

		for (int i = 1; i < numOfScans; ++i)
		{
			switch (orientation)
			{
			case eSubScanOrientation::NORTH_TO_SOUTH:
				x1_mm += separation_mm;
				x2_mm += separation_mm;
				break;
			case eSubScanOrientation::SOUTH_TO_NORTH:
				x1_mm -= separation_mm;
				x2_mm -= separation_mm;
				break;
			case eSubScanOrientation::WEST_TO_EAST:
				y1_mm -= separation_mm;
				y2_mm -= separation_mm;
				break;
			case eSubScanOrientation::EAST_TO_WEST:
				y1_mm += separation_mm;
				y2_mm += separation_mm;
				break;
			}

			emit drawPath(x1_mm, y1_mm, x2_mm, y2_mm);
		}
	}
}
