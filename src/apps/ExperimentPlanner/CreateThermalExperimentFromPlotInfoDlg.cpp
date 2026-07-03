
#include "CreateThermalExperimentFromPlotInfoDlg.hpp"
#include "GpsFileReader.hpp"
#include "Constants.hpp"

#include "MeasurementSteps.hpp"
#include "MeasurementSteps_Ssnx.hpp"

#include "ExperimentMetaInfoDlg.hpp"
#include "ExperimentCtrlInfoDlg.hpp"
#include "ExperimentSensorInfoDlg.hpp"

#include "ScanPointsWidget.hpp"

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

	const QString WEST_TO_EAST = "West to East";
	const QString EAST_TO_WEST = "East to West";
	const QString NORTH_TO_SOUTH = "North to South";
	const QString SOUTH_TO_NORTH = "South to North";

	constexpr int SCAN_NORTH_TO_SOUTH = 0;
	constexpr int SCAN_SOUTH_TO_NORTH = 1;
	constexpr int SCAN_EAST_TO_WEST = 2;
	constexpr int SCAN_WEST_TO_EAST = 3;

}

cCreateThermalExperimentFromPlotInfoDlg::cCreateThermalExperimentFromPlotInfoDlg(const QString& filename, QWidget* parent)
:
	cCreateThermalExperimentDlg(parent)
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

cCreateThermalExperimentFromPlotInfoDlg::~cCreateThermalExperimentFromPlotInfoDlg()
{}

void cCreateThermalExperimentFromPlotInfoDlg::createControls_PointSelection()
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

	//mpEndPosition = new QTableView(this);
	//mpEndPosition->setModel(mpModel);

	//mpEndPosition->verticalHeader()->hide();

	//headerView = mpEndPosition->horizontalHeader();
	//headerView->setDefaultAlignment(Qt::AlignHCenter);
	//headerView->setStretchLastSection(false);

	//mpEndPosition->setHorizontalScrollMode(QAbstractItemView::ScrollPerItem);
	//mpEndPosition->setSelectionBehavior(QAbstractItemView::SelectRows);
	//mpEndPosition->setSelectionMode(QAbstractItemView::SingleSelection);
	//mpEndPosition->setSortingEnabled(false);
	//mpEndPosition->setFixedWidth(419);

	mpClearPath = new QPushButton("Clear Path", this);
	connect(mpClearPath, &QPushButton::pressed, this, &cCreateThermalExperimentFromPlotInfoDlg::clearPaths);

	mpShowPath = new QPushButton("Show Path", this);
	connect(mpShowPath, &QPushButton::pressed, this, &cCreateThermalExperimentFromPlotInfoDlg::onShowPath);


	mpPointAtStart  = new QRadioButton("Start of Scan", this);
	mpPointAtCenter = new QRadioButton("Center of Scan", this);
	mpPointAtEnd    = new QRadioButton("End of Scan", this);
	mpPointAtStart->setChecked(true);

//	mpInverseDirection = new QCheckBox("Inverse Direction", this);

	mpPlotOrientation = new QComboBox(this);
	mpPlotOrientation->setEditable(false);
	mpPlotOrientation->addItem(NORTH_TO_SOUTH);
	mpPlotOrientation->addItem(SOUTH_TO_NORTH);
	mpPlotOrientation->addItem(EAST_TO_WEST);
	mpPlotOrientation->addItem(WEST_TO_EAST);
	mpPlotOrientation->setCurrentIndex(SCAN_WEST_TO_EAST);

	mpUnits = new QComboBox(this);
	mpUnits->setEditable(false);
	mpUnits->addItem("Meters");
	mpUnits->addItem("Millimeters");
	mpUnits->addItem("Feet");
	mpUnits->addItem("Inches");
	connect(mpUnits, &QComboBox::currentTextChanged, this, &cCreateThermalExperimentFromPlotInfoDlg::onUnitChange);
	mConversionFactor = nConstants::M_TO_MM;

	mpPlotLengthLabel = new QLabel(PLOT_LENGTH_TEXT + "m)", this);
	mpPlotLength = new QLineEdit(this);
	mpPlotLength->setValidator(new QDoubleValidator(0, 100.0, 3));
	mpPlotLength->setText("1");
}

void cCreateThermalExperimentFromPlotInfoDlg::createLayout_PointSelection(QVBoxLayout* pMainLayout)
{
	QLabel* pText = nullptr;
	QGroupBox* pGroupBox = nullptr;
	QGridLayout* pGridLayout = nullptr;

	QHBoxLayout* pPosLayout = new QHBoxLayout();
	pPosLayout->addStretch(1);
	pPosLayout->addWidget(mpStartPosition);
	//pPosLayout->addSpacing(10);
	//pPosLayout->addWidget(mpEndPosition);
	pPosLayout->addSpacing(10);

	QVBoxLayout* pVSubLayout = new QVBoxLayout();
	pVSubLayout->addWidget(mpClearPath);
	pVSubLayout->addWidget(mpShowPath);
	pPosLayout->addLayout(pVSubLayout);
	pPosLayout->addStretch(1);

	pMainLayout->addLayout(pPosLayout);

	QHBoxLayout* pOptionsLayout = new QHBoxLayout();
	pOptionsLayout->addStretch(1);
	pOptionsLayout->addWidget(mpPointAtStart);
	pOptionsLayout->addSpacing(10);
	pOptionsLayout->addWidget(mpPointAtCenter);
	pOptionsLayout->addSpacing(10);
	pOptionsLayout->addWidget(mpPointAtEnd);
//	pOptionsLayout->addSpacing(10);
//	pOptionsLayout->addWidget(mpInverseDirection);
	pOptionsLayout->addStretch(1);

	pMainLayout->addLayout(pOptionsLayout);

	pGroupBox = new QGroupBox(tr("Plot Information"));
	pGroupBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

	QHBoxLayout* pHSubLayout = new QHBoxLayout();
	pHSubLayout->addWidget(mpPlotLengthLabel);
	pHSubLayout->addWidget(mpPlotLength);
	pHSubLayout->addSpacing(10);
	pHSubLayout->addWidget(mpPlotOrientation);
	pHSubLayout->addSpacing(10);
	pHSubLayout->addWidget(mpUnits);

	pGroupBox->setLayout(pHSubLayout);

	pMainLayout->addWidget(pGroupBox);

	pMainLayout->addSpacing(10);
}

void cCreateThermalExperimentFromPlotInfoDlg::onUnitChange(const QString& text)
{
	double length = mpPlotLength->text().toDouble() * mConversionFactor;

	switch (mpUnits->currentIndex())
	{
	case 0:
		mpPlotLengthLabel->setText(PLOT_LENGTH_TEXT + "m)");

		mConversionFactor = nConstants::M_TO_MM;
		break;
	case 1:
		mpPlotLengthLabel->setText(PLOT_LENGTH_TEXT + "mm)");

		mConversionFactor = 1.0;
		break;
	case 2:
		mpPlotLengthLabel->setText(PLOT_LENGTH_TEXT + "ft)");

		mConversionFactor = nConstants::FT_TO_MM;
		break;
	case 3:
		mpPlotLengthLabel->setText(PLOT_LENGTH_TEXT + "in)");

		mConversionFactor = nConstants::IN_TO_MM;
		break;
	}

	length /= mConversionFactor;

	mpPlotLength->setText(QString::number(length));
}


bool cCreateThermalExperimentFromPlotInfoDlg::generate()
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

	bool auto_advance = mpAutoAdvance->isChecked();

	if (mMeasurementTitle.empty())
	{
		mMeasurementTitle = title;
	}
	else if (mMeasurementTitle != title)
	{
		mMeasurementTitle = title;
	}

	if (mExperimentTitle.empty())
	{
		auto pos = title.find("Pass");

		if (pos == std::string::npos)
			mExperimentTitle = title;
		else
		{
			if (pos > 0)
			{
				if (std::isspace(title[pos - 1]) || (title[pos - 1] == '_'))
					--pos;
			}
			mExperimentTitle = title.substr(0, pos);
		}
	}

	QModelIndex startIndex = mpStartPosition->currentIndex();

	if ((startIndex.row() < 0))
	{
		QString msg = "Please select a start position.";
		QMessageBox msg_box(QMessageBox::Critical, "Invalid Parameter", msg);
		msg_box.exec();
		return false;
	}

	auto x1 = mpModel->data(startIndex.siblingAtColumn(1)).toFloat();
	auto y1 = mpModel->data(startIndex.siblingAtColumn(2)).toFloat();
	auto h1 = mpModel->data(startIndex.siblingAtColumn(3)).toFloat();

	int plot_orientation = mpPlotOrientation->currentIndex();
	int plot_length_mm = static_cast<int>(mpPlotLength->text().toDouble() * mConversionFactor);

	int x1_mm = static_cast<int>(x1 * nConstants::M_TO_MM);
	int y1_mm = static_cast<int>(y1 * nConstants::M_TO_MM);
	int h1_mm = static_cast<int>(h1 * nConstants::M_TO_MM);

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

	int start_offset_mm = static_cast<int>(mpBeginningOffset_m->text().toDouble() * nConstants::M_TO_MM);
	int end_offset_mm = static_cast<int>(mpEndingOffset_m->text().toDouble() * nConstants::M_TO_MM);

	int x2_mm = x1_mm;
	int y2_mm = y1_mm;
	int h2_mm = h1_mm;

	if (mpPointAtStart->isChecked())
	{
		switch (plot_orientation)
		{
		case SCAN_NORTH_TO_SOUTH:
			x2_mm = x1_mm + plot_length_mm + end_offset_mm;
			x1_mm -= start_offset_mm;
			break;
		case SCAN_SOUTH_TO_NORTH:
			x2_mm = x1_mm - plot_length_mm - end_offset_mm;
			x1_mm += start_offset_mm;
			break;
		case SCAN_EAST_TO_WEST:
			y2_mm = y1_mm - plot_length_mm - end_offset_mm;
			y1_mm += start_offset_mm;
			break;
		default:
		case SCAN_WEST_TO_EAST:
			y2_mm = y1_mm + plot_length_mm + end_offset_mm;
			y1_mm -= start_offset_mm;
			break;
		}
	}
	else if (mpPointAtCenter->isChecked())
	{
		int half_plot_length_mm = plot_length_mm / 2;

		switch (plot_orientation)
		{
		case SCAN_NORTH_TO_SOUTH:
			x1_mm -= half_plot_length_mm;
			x2_mm = x1_mm + plot_length_mm + end_offset_mm;
			x1_mm -= start_offset_mm;
			break;
		case SCAN_SOUTH_TO_NORTH:
			x1_mm += half_plot_length_mm;
			x2_mm = x1_mm - plot_length_mm - end_offset_mm;
			x1_mm += start_offset_mm;
			break;
		case SCAN_EAST_TO_WEST:
			y1_mm += half_plot_length_mm;
			y2_mm = y1_mm - plot_length_mm - end_offset_mm;
			y1_mm += start_offset_mm;
			break;
		default:
		case SCAN_WEST_TO_EAST:
			y1_mm -= half_plot_length_mm;
			y2_mm = y1_mm + plot_length_mm + end_offset_mm;
			y1_mm -= start_offset_mm;
			break;
		}
	}
	else if (mpPointAtEnd->isChecked())
	{
		switch (plot_orientation)
		{
		case SCAN_NORTH_TO_SOUTH:
			x1_mm = x2_mm - plot_length_mm - start_offset_mm;
			x2_mm += end_offset_mm;
			break;
		case SCAN_SOUTH_TO_NORTH:
			x1_mm = x2_mm + plot_length_mm + start_offset_mm;
			x2_mm -= end_offset_mm;
			break;
		case SCAN_EAST_TO_WEST:
			y1_mm = y2_mm + plot_length_mm + start_offset_mm;
			y2_mm -= end_offset_mm;
			break;
		default:
		case SCAN_WEST_TO_EAST:
			y1_mm = y2_mm - plot_length_mm - start_offset_mm;
			y2_mm += end_offset_mm;
			break;
		}
	}
	else
		return false;

	int vertical_speed_mmps = mpTravelVerticalSpeed_mmps->text().toInt();
	int travel_speed_mmps = mpTravelSpeed_mmps->text().toInt();
	int scan_speed_mmps = mpMeasurementSpeed_mmps->text().toInt();
	int safe_vertical_speed_mmps = mpSafeVerticalSpeed_mmps->text().toInt();

	double minIntegrationTime_sec = mpMinIntegrationTime_sec->text().toDouble();
	double maxIntegrationTime_sec = mpMaxIntegrationTime_sec->text().toDouble();

	if (maxIntegrationTime_sec < minIntegrationTime_sec)
		std::swap(minIntegrationTime_sec, maxIntegrationTime_sec);

	int errorThreshold_mm = mpErrorThreshold_mm->text().toInt();

	/* Grab the info for multiple scans if selected */
	int startNum = 0;
	bool hasNumber = nStringUtils::endsWithInt(title, &startNum);

	QSharedPointer<cExperimentFile> pInfo = QSharedPointer<cExperimentFile>(new cExperimentFile());

	if (hasNumber && auto_advance)
	{
		nStringUtils::replaceIntAtEnd(title, ++startNum);
		mpTitle->setText(QString::fromStdString(title));
	}

	pInfo->setMeasurementName(mMeasurementTitle);
	pInfo->setExperimentName(mExperimentTitle);
	pInfo->setExperimentType(eExperimentType::LIDAR);
	pInfo->setMetaData(mMetaInfo);
	pInfo->setController(copy(mCtrlInfo));
	pInfo->setSensors(mSensorInfo);

	// Add preamble: moving dolly up to a safe travel height...
	std::unique_ptr<cMeasurementStep_Movement> step = std::make_unique<cMeasurementStep_Movement>();

	step->setZ_mm(travel_z_mm);
	step->setSpeed_mmps(vertical_speed_mmps);
	pInfo->appendStep(std::move(step));

	if (plot_length_mm == 0)
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

		// Mark starting position
		auto marker = std::make_unique<cMeasurementStep_Marker>();
		marker->setMarkerType(cMeasurementStep_Marker::eMarkerType::START_OF_MEASUREMENT);
		pInfo->appendStep(std::move(marker));

		// Do measurement...
		step = std::make_unique<cMeasurementStep_Movement>();
		step->setX_mm(x2_mm);
		step->setY_mm(y2_mm);

		if ((mpHeightReference->currentIndex() == 1) &&
			((scan_z_mm + h1_mm) != (scan_z_mm + h2_mm)))
		{
			step->setZ_mm(scan_z_mm + h2_mm);
		}

		step->setSpeed_mmps(scan_speed_mmps);
		step->setRecording(true);
		pInfo->appendStep(std::move(step));

		// Mark ending position
		marker = std::make_unique<cMeasurementStep_Marker>();
		marker->setMarkerType(cMeasurementStep_Marker::eMarkerType::END_OF_MEASUREMENT);
		pInfo->appendStep(std::move(marker));

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

	return true;
}

void cCreateThermalExperimentFromPlotInfoDlg::onShowPath()
{
	QModelIndex startIndex = mpStartPosition->currentIndex();
	if (startIndex.row() < 0)
		return;

	auto x1 = mpModel->data(startIndex.siblingAtColumn(1)).toFloat();
	auto y1 = mpModel->data(startIndex.siblingAtColumn(2)).toFloat();
	auto x2 = 0.0f;
	auto y2 = 0.0f;

	int plot_length_m = static_cast<int>(mpPlotLength->text().toDouble() * mConversionFactor * nConstants::MM_TO_M);
	int plot_orientation = mpPlotOrientation->currentIndex();

	if (mpPointAtStart->isChecked())
	{
		switch (plot_orientation)
		{
		case SCAN_NORTH_TO_SOUTH:
			x2 = x1 + plot_length_m;
			y2 = y1;
			break;
		case SCAN_SOUTH_TO_NORTH:
			x2 = x1 - plot_length_m;
			y2 = y1;
			break;
		case SCAN_EAST_TO_WEST:
			x2 = x1;
			y2 = y1 - plot_length_m;
			break;
		default:
		case SCAN_WEST_TO_EAST:
			x2 = x1;
			y2 = y1 + plot_length_m;
			break;
		}
	}
	else if (mpPointAtCenter->isChecked())
	{
		int half_plot_length_m = plot_length_m / 2;

		switch (plot_orientation)
		{
		case SCAN_NORTH_TO_SOUTH:
			x1 -= half_plot_length_m;
			x2 = x1 + plot_length_m;
			y2 = y1;
			break;
		case SCAN_SOUTH_TO_NORTH:
			x1 += half_plot_length_m;
			x2 = x1 - plot_length_m;
			y2 = y1;
			break;
		case SCAN_EAST_TO_WEST:
			x2 = x1;
			y1 += half_plot_length_m;
			y2 = y1 - plot_length_m;
			break;
		default:
		case SCAN_WEST_TO_EAST:
			x2 = x1;
			y1 -= half_plot_length_m;
			y2 = y1 + plot_length_m;
			break;
		}
	}
	else if (mpPointAtEnd->isChecked())
	{
		switch (plot_orientation)
		{
		case SCAN_NORTH_TO_SOUTH:
			x2 = x1 - plot_length_m;
			y2 = y1;
			break;
		case SCAN_SOUTH_TO_NORTH:
			x2 = x1 + plot_length_m;
			y2 = y1;
			break;
		case SCAN_EAST_TO_WEST:
			x2 = x1;
			y2 = y1 + plot_length_m;
			break;
		default:
		case SCAN_WEST_TO_EAST:
			x2 = x1;
			y2 = y1 - plot_length_m;
			break;
		}
	}
	else
		return;

	int x1_mm = static_cast<int>(x1 * nConstants::M_TO_MM);
	int y1_mm = static_cast<int>(y1 * nConstants::M_TO_MM);
	int x2_mm = static_cast<int>(x2 * nConstants::M_TO_MM);
	int y2_mm = static_cast<int>(y2 * nConstants::M_TO_MM);

	emit drawPath(x1_mm, y1_mm, x2_mm, y2_mm);
}
