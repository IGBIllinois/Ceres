
#include "CreateHyperspectralExperimentFromPlotInfoDlg.hpp"
#include "Hyperspectral/HySpex/HySpexIDs.hpp"
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
#include <QRadioButton>

#include <algorithm>
#include <memory>

namespace
{
	const QString PLOT_LENGTH_TEXT = "Plot Length (";
	const QString NORTH_SOUTH_OFFSET_TEXT = "North/South (+South) Offset (";
	const QString EAST_WEST_OFFSET_TEXT = "East/West (+East) Offset (";

	const QString WEST_TO_EAST = "West to East";
	const QString EAST_TO_WEST = "East to West";
	const QString NORTH_TO_SOUTH = "North to South";
	const QString SOUTH_TO_NORTH = "South to North";

	constexpr int SCAN_WEST_TO_EAST = 0;
	constexpr int SCAN_EAST_TO_WEST = 1;
	constexpr int SCAN_NORTH_TO_SOUTH = 2;
	constexpr int SCAN_SOUTH_TO_NORTH = 3;
}

cCreateHyperspectralExperimentFromPlotInfoDlg::cCreateHyperspectralExperimentFromPlotInfoDlg(const QString& filename, QWidget* parent)
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

cCreateHyperspectralExperimentFromPlotInfoDlg::~cCreateHyperspectralExperimentFromPlotInfoDlg()
{}

void cCreateHyperspectralExperimentFromPlotInfoDlg::createControls_PointSelection()
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
	connect(mpClearPath, &QPushButton::pressed, this, &cCreateHyperspectralExperimentFromPlotInfoDlg::clearPaths);

	mpShowPath = new QPushButton("Show Path", this);
	connect(mpShowPath, &QPushButton::pressed, this, &cCreateHyperspectralExperimentFromPlotInfoDlg::onShowPath);

	mpInverseDirection = new QCheckBox("Inverse Direction", this);

	// default to meters
	mpPlotLengthLabel = new QLabel(PLOT_LENGTH_TEXT + "m)", this);
	mpPlotLength = new QLineEdit(this);
	mpPlotLength->setValidator(new QDoubleValidator(1, 10000.0, 3));
	mpPlotLength->setText("1");

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
	mpPlotUnits->setCurrentIndex(0);	// <- Set default to meters
	mPlotConversionFactor = nConstants::M_TO_MM;
	connect(mpPlotUnits, &QComboBox::currentTextChanged, this, &cCreateHyperspectralExperimentFromPlotInfoDlg::onPlotUnitChange);

	mpStart = new QRadioButton("Start");
	mpCenter = new QRadioButton("Center");
	mpEnd = new QRadioButton("End");

	mpCenter->setChecked(true);
}

void cCreateHyperspectralExperimentFromPlotInfoDlg::createControls_SubScanInfo()
{
	mpX_OffsetLabel = new QLabel(NORTH_SOUTH_OFFSET_TEXT + "in)", this);
	mpX_Offset = new QLineEdit(this);
	mpX_Offset->setValidator(new QDoubleValidator(-10000.0, 10000.0, 3));
	mpX_Offset->setText("0");

	mpY_OffsetLabel = new QLabel(EAST_WEST_OFFSET_TEXT + "in)", this);
	mpY_Offset = new QLineEdit(this);
	mpY_Offset->setValidator(new QDoubleValidator(-10000.0, 10000.0, 3));
	mpY_Offset->setText("0");

	mpOffsetUnits = new QComboBox(this);
	mpOffsetUnits->setEditable(false);
	mpOffsetUnits->addItem("Meters");
	mpOffsetUnits->addItem("Millimeters");
	mpOffsetUnits->addItem("Feet");
	mpOffsetUnits->addItem("Inches");
	mpOffsetUnits->setCurrentIndex(3);	// <- Set default to inches
	mOffsetConversionFactor = nConstants::IN_TO_MM;
	connect(mpOffsetUnits, &QComboBox::currentTextChanged, this, &cCreateHyperspectralExperimentFromPlotInfoDlg::onOffsetUnitChange);
}

void cCreateHyperspectralExperimentFromPlotInfoDlg::createLayout_PointSelection(QVBoxLayout* pMainLayout)
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

	mpStart->setParent(pGroupBox);
	mpCenter->setParent(pGroupBox);
	mpEnd->setParent(pGroupBox);

	pVSubLayout = new QVBoxLayout();

	QHBoxLayout* pPlotLayout = new QHBoxLayout();

	pPlotLayout->addWidget(mpPlotLengthLabel);
	pPlotLayout->addWidget(mpPlotLength);
	pPlotLayout->addSpacing(10);
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

void cCreateHyperspectralExperimentFromPlotInfoDlg::createLayout_SubScanInfo(QVBoxLayout* pMainLayout)
{
	QLabel* pText = nullptr;

	QGroupBox* pGroupBox = new QGroupBox(tr("Plot Offset Information"));
	pGroupBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

	QHBoxLayout* pHSubLayout = new QHBoxLayout();
	pHSubLayout->addWidget(mpX_OffsetLabel);
	pHSubLayout->addWidget(mpX_Offset);
	pHSubLayout->addSpacing(10);
	pHSubLayout->addWidget(mpY_OffsetLabel);
	pHSubLayout->addWidget(mpY_Offset);
	pHSubLayout->addSpacing(10);
	pHSubLayout->addWidget(mpOffsetUnits);

	pGroupBox->setLayout(pHSubLayout);

	pMainLayout->addWidget(pGroupBox);
}

void cCreateHyperspectralExperimentFromPlotInfoDlg::onPlotUnitChange(const QString& text)
{
	double plot_length = mpPlotLength->text().toDouble() * mPlotConversionFactor;

	switch (mpPlotUnits->currentIndex())
	{
	case 0:
		mpPlotLengthLabel->setText(PLOT_LENGTH_TEXT + "m)");

		mPlotConversionFactor = nConstants::M_TO_MM;
		break;
	case 1:
		mpPlotLengthLabel->setText(PLOT_LENGTH_TEXT + "mm)");

		mPlotConversionFactor = 1.0;
		break;
	case 2:
		mpPlotLengthLabel->setText(PLOT_LENGTH_TEXT + "ft)");

		mPlotConversionFactor = nConstants::FT_TO_MM;
		break;
	case 3:
		mpPlotLengthLabel->setText(PLOT_LENGTH_TEXT + "in)");

		mPlotConversionFactor = nConstants::IN_TO_MM;
		break;
	}

	plot_length /= mPlotConversionFactor;

	mpPlotLength->setText(QString::number(plot_length));
}

void cCreateHyperspectralExperimentFromPlotInfoDlg::onOffsetUnitChange(const QString& text)
{
	double x_offset = mpX_Offset->text().toDouble() * mOffsetConversionFactor;
	double y_offset = mpY_Offset->text().toDouble() * mOffsetConversionFactor;

	switch (mpOffsetUnits->currentIndex())
	{
	case 0:
		mpX_OffsetLabel->setText(NORTH_SOUTH_OFFSET_TEXT + "m)");
		mpY_OffsetLabel->setText(EAST_WEST_OFFSET_TEXT + "m)");

		mOffsetConversionFactor = nConstants::M_TO_MM;
		break;
	case 1:
		mpX_OffsetLabel->setText(NORTH_SOUTH_OFFSET_TEXT + "mm)");
		mpY_OffsetLabel->setText(EAST_WEST_OFFSET_TEXT + "mm)");

		mOffsetConversionFactor = 1.0;
		break;
	case 2:
		mpX_OffsetLabel->setText(NORTH_SOUTH_OFFSET_TEXT + "ft)");
		mpY_OffsetLabel->setText(EAST_WEST_OFFSET_TEXT + "ft)");

		mOffsetConversionFactor = nConstants::FT_TO_MM;
		break;
	case 3:
		mpX_OffsetLabel->setText(NORTH_SOUTH_OFFSET_TEXT + "in)");
		mpY_OffsetLabel->setText(EAST_WEST_OFFSET_TEXT + "in)");

		mOffsetConversionFactor = nConstants::IN_TO_MM;
		break;
	}

	x_offset /= mOffsetConversionFactor;
	y_offset /= mOffsetConversionFactor;

	mpX_Offset->setText(QString::number(x_offset));
	mpY_Offset->setText(QString::number(y_offset));
}

bool cCreateHyperspectralExperimentFromPlotInfoDlg::generate()
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

	int x1_mm = static_cast<int>(x1 * nConstants::M_TO_MM);
	int y1_mm = static_cast<int>(y1 * nConstants::M_TO_MM);
	int h1_mm = static_cast<int>(h1 * nConstants::M_TO_MM);

	float dx = x2 - x1;
	float dy = y2 - y1;

	double scan_direction_deg = atan2(dy, dx) * nConstants::RAD_TO_DEG;

	struct sPOINT
	{
		int x_mm = 0;
		int y_mm = 0;
		int z_mm = 0;
	};

	std::vector<sPOINT> points;

	points.push_back({ x1_mm, y1_mm, h1_mm });

	int i = 1;
	QModelIndex index = mpStartPosition->currentIndex().siblingAtRow(i++);
	while (index != endIndex)
	{
		auto x = mpModel->data(index.siblingAtColumn(1)).toFloat();
		auto y = mpModel->data(index.siblingAtColumn(2)).toFloat();
		auto h = mpModel->data(index.siblingAtColumn(3)).toFloat();

		if ((45.0 <= scan_direction_deg) && (scan_direction_deg < 135.0))
		{
			// We are scanning east to west
			if (std::abs(x - x1) < 0.5)
			{
				int x_mm = static_cast<int>(x * nConstants::M_TO_MM);
				int y_mm = static_cast<int>(y * nConstants::M_TO_MM);
				int h_mm = static_cast<int>(h * nConstants::M_TO_MM);

				points.push_back({ x_mm, y_mm, h_mm });
			}
		}
		else if ((135.0 <= scan_direction_deg) && (scan_direction_deg < 225.0))
		{
			// We are scanning north to south
			if (std::abs(y - y1) < 0.5)
			{
				int x_mm = static_cast<int>(x * nConstants::M_TO_MM);
				int y_mm = static_cast<int>(y * nConstants::M_TO_MM);
				int h_mm = static_cast<int>(h * nConstants::M_TO_MM);

				points.push_back({ x_mm, y_mm, h_mm });
			}
		}
		else if ((225.0 <= scan_direction_deg) && (scan_direction_deg < 315.0))
		{
			// We are scanning west to east
			if (std::abs(x - x1) < 0.5)
			{
				int x_mm = static_cast<int>(x * nConstants::M_TO_MM);
				int y_mm = static_cast<int>(y * nConstants::M_TO_MM);
				int h_mm = static_cast<int>(h * nConstants::M_TO_MM);

				points.push_back({ x_mm, y_mm, h_mm });
			}
		}
		else
		{
			// We are scanning south to north
			if (std::abs(y - y1) < 0.5)
			{
				int x_mm = static_cast<int>(x * nConstants::M_TO_MM);
				int y_mm = static_cast<int>(y * nConstants::M_TO_MM);
				int h_mm = static_cast<int>(h * nConstants::M_TO_MM);

				points.push_back({ x_mm, y_mm, h_mm });
			}
		}

		index = mpStartPosition->currentIndex().siblingAtRow(i++);
	}

	int x2_mm = static_cast<int>(x2 * nConstants::M_TO_MM);
	int y2_mm = static_cast<int>(y2 * nConstants::M_TO_MM);
	int h2_mm = static_cast<int>(h2 * nConstants::M_TO_MM);

	points.push_back({ x2_mm, y2_mm, h2_mm });

	if (mpInverseDirection->isChecked())
	{
		std::reverse(points.begin(), points.end());
	}

	int x_offset_mm = static_cast<int>(mpX_Offset->text().toDouble() * mOffsetConversionFactor);
	int y_offset_mm = static_cast<int>(mpY_Offset->text().toDouble() * mOffsetConversionFactor);

	int travel_z_mm = static_cast<int>(mpTravelHeight_m->text().toDouble() * nConstants::M_TO_MM);
	int safe_z_mm = static_cast<int>(mpSafeHeight_m->text().toDouble() * nConstants::M_TO_MM);

	int scan_z_offset_mm = static_cast<int>(mpHeightOffset->text().toDouble() * nConstants::M_TO_MM);
	scan_z_offset_mm += mpLensFocalDistance->currentData().toInt();


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

	auto* pGroundModel = cRappGroundModel::get();

	QSharedPointer<cExperimentFile> pInfo = QSharedPointer<cExperimentFile>(new cExperimentFile());

	pInfo->setExperimentName(title);
	pInfo->setMetaData(mMetaInfo);
	pInfo->setController(copy(mCtrlInfo));
	pInfo->setSensors(mSensorInfo);

	// Add preamble: moving dolly up to a safe travel height...
	std::unique_ptr<cExperimentStep_Movement> step = std::make_unique<cExperimentStep_Movement>();

	step->setZ_mm(travel_z_mm);
	step->setSpeed_mmps(vertical_speed_mmps);
	pInfo->appendStep(std::move(step));

	int orientation = mpPlotOrientation->currentIndex();

	auto it = points.begin();

	int x_mm = it->x_mm + x_offset_mm;
	int y_mm = it->y_mm + y_offset_mm;
	int h_mm = it->z_mm;

	switch (orientation)
	{
	case SCAN_WEST_TO_EAST:
		y_mm += start_offset_mm;
		break;
	case SCAN_EAST_TO_WEST:
		y_mm -= start_offset_mm;
		break;
	case SCAN_NORTH_TO_SOUTH:
		x_mm += start_offset_mm;
		break;
	case SCAN_SOUTH_TO_NORTH:
		x_mm -= start_offset_mm;
		break;
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
		auto h = static_cast<int>(pGroundModel->getMeshHeight_mm(x_mm, y_mm));

		if (h != rfm::INVALID_HEIGHT)
			h_mm = h;
	}

	step->setZ_mm(scan_z_offset_mm + h_mm);

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

	for (; it != points.end();)
	{
		switch (orientation)
		{
		case SCAN_WEST_TO_EAST:
			y_mm += scan_distance_mm;
			break;
		case SCAN_EAST_TO_WEST:
			y_mm -= scan_distance_mm;
			break;
		case SCAN_NORTH_TO_SOUTH:
			x_mm += scan_distance_mm;
			break;
		case SCAN_SOUTH_TO_NORTH:
			x_mm -= scan_distance_mm;
			break;
		}

		// Do measurement...
		step = std::make_unique<cExperimentStep_Movement>();
		step->setX_mm(x_mm);
		step->setY_mm(y_mm);

		if (pGroundModel)
		{
			auto h = static_cast<int>(pGroundModel->getMeshHeight_mm(x_mm, y_mm));

			if (h != rfm::INVALID_HEIGHT)
				h_mm = h;
		}

		step->setZ_mm(scan_z_offset_mm + h_mm);

		step->setSpeed_mmps(scan_speed_mmps);
		step->setRecording(true);
		pInfo->appendStep(std::move(step));

		++it;
		if (it == points.end())
			break;

		// Advance to the next measurement...
		x_mm = it->x_mm + x_offset_mm;
		y_mm = it->y_mm + y_offset_mm;
		h_mm = it->z_mm;

		switch (orientation)
		{
		case SCAN_WEST_TO_EAST:
			y_mm += start_offset_mm;
			break;
		case SCAN_EAST_TO_WEST:
			y_mm -= start_offset_mm;
			break;
		case SCAN_NORTH_TO_SOUTH:
			x_mm += start_offset_mm;
			break;
		case SCAN_SOUTH_TO_NORTH:
			x_mm -= start_offset_mm;
			break;
		}

		// Move to next measurement...
		step = std::make_unique<cExperimentStep_Movement>();
		step->setX_mm(x_mm);
		step->setY_mm(y_mm);

		if (pGroundModel)
		{
			auto h = static_cast<int>(pGroundModel->getMeshHeight_mm(x_mm, y_mm));

			if (h != rfm::INVALID_HEIGHT)
				h_mm = h;
		}

		step->setZ_mm(scan_z_offset_mm + h_mm);

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

	// Move dolly to a safe height to park it
	step = std::make_unique<cExperimentStep_Movement>();
	step->setZ_mm(safe_z_mm);
	step->setSpeed_mmps(safe_vertical_speed_mmps);
	step->setTilt_deg(safe_tilt_deg);
	step->setRoll_deg(safe_roll_deg);
	pInfo->appendStep(std::move(step));

	emit experimentChanged(pInfo);

	return true;
}

void cCreateHyperspectralExperimentFromPlotInfoDlg::onShowPath()
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
