
#include "CreateLidarExperimentFromGpsDlg.hpp"
#include "GpsFileReader.hpp"
#include "Constants.hpp"

#include "MeasurementSteps.hpp"
#include "MeasurementSteps_Ssnx.hpp"

#include "ExperimentMetaInfoDlg.hpp"
#include "ExperimentCtrlInfoDlg.hpp"
#include "ExperimentSensorInfoDlg.hpp"

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

	const QString SCAN_SEPARATION_TEXT = "Separation (";

	const QString WEST_TO_EAST = "West to East";
	const QString EAST_TO_WEST = "East to West";
	const QString NORTH_TO_SOUTH = "North to South";
	const QString SOUTH_TO_NORTH = "South to North";

	constexpr int SUB_SCAN_NORTH_TO_SOUTH = 0;
	constexpr int SUB_SCAN_SOUTH_TO_NORTH = 1;
	constexpr int SUB_SCAN_EAST_TO_WEST = 2;
	constexpr int SUB_SCAN_WEST_TO_EAST = 3;
}

cCreateLidarExperimentFromGpsDlg::cCreateLidarExperimentFromGpsDlg(const QString& filename, QWidget* parent)
:
	cCreateLidarExperimentDlg(parent)
{
	setMinimumWidth(550);

	mpModel = new QStandardItemModel(10, 4, this);
	mpModel->setHeaderData(0, Qt::Horizontal, tr("Name"));
	mpModel->setHeaderData(1, Qt::Horizontal, tr("X (m)"));
	mpModel->setHeaderData(2, Qt::Horizontal, tr("Y (m)"));
	mpModel->setHeaderData(3, Qt::Horizontal, tr("Z (m)"));

	positions_t points;

	{
		cGpsFileReader reader;
		reader.loadFromFile(filename.toStdString());
		points = reader.GetPoints();

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

	mpReverseModel = new QStandardItemModel(10, 4, this);
	mpReverseModel->setHeaderData(0, Qt::Horizontal, tr("Name"));
	mpReverseModel->setHeaderData(1, Qt::Horizontal, tr("X (m)"));
	mpReverseModel->setHeaderData(2, Qt::Horizontal, tr("Y (m)"));
	mpReverseModel->setHeaderData(3, Qt::Horizontal, tr("Z (m)"));

	{
		auto n = points.size();

		if (n > 0)
		{
			int j = n - 1;
			for (int i = 0; i < n; ++i)
			{
				const auto& pos = points[j--];

				mpReverseModel->insertRows(i, 1, QModelIndex());
				mpReverseModel->setData(mpReverseModel->index(i, 0, QModelIndex()), QString::fromStdString(pos.label));
				mpReverseModel->setData(mpReverseModel->index(i, 1, QModelIndex()), pos.x_m);
				mpReverseModel->setData(mpReverseModel->index(i, 2, QModelIndex()), pos.y_m);
				mpReverseModel->setData(mpReverseModel->index(i, 3, QModelIndex()), pos.z_m);
			}
		}
	}

	initialize();
}

cCreateLidarExperimentFromGpsDlg::~cCreateLidarExperimentFromGpsDlg()
{}

void cCreateLidarExperimentFromGpsDlg::createControls_PointSelection()
{
	mpStartX_mm = new QLineEdit(this);
	mpStartX_mm->setValidator(new QIntValidator(10000, 190000));

	mpStartY_mm = new QLineEdit(this);
	mpStartY_mm->setValidator(new QIntValidator(10000, 190000));

	mpEndX_mm = new QLineEdit(this);
	mpEndX_mm->setValidator(new QIntValidator(10000, 190000));

	mpEndY_mm = new QLineEdit(this);
	mpEndY_mm->setValidator(new QIntValidator(10000, 190000));

	mpStartPosition = new QTableView(this);
	mpStartPosition->setModel(mpModel);

	connect(mpStartPosition, &QTableView::activated, this, &cCreateLidarExperimentFromGpsDlg::onStartItem);
	connect(mpStartPosition, &QTableView::pressed, this, &cCreateLidarExperimentFromGpsDlg::onStartItem);

	mpStartPosition->verticalHeader()->hide();

	QHeaderView* headerView = mpStartPosition->horizontalHeader();
	headerView->setDefaultAlignment(Qt::AlignHCenter);
	headerView->setStretchLastSection(false);

	mpStartPosition->setHorizontalScrollMode(QAbstractItemView::ScrollPerItem);
	mpStartPosition->setSelectionBehavior(QAbstractItemView::SelectRows);
	mpStartPosition->setSelectionMode(QAbstractItemView::SingleSelection);
	mpStartPosition->setSortingEnabled(false);
	mpStartPosition->setFixedWidth(419);

	mpStartPointAtBegin = new QRadioButton("Start of Plot", this);
	mpStartPointAtCenter = new QRadioButton("Center of Plot", this);
	mpStartPointAtEnd = new QRadioButton("End of Plot", this);
	mpStartPointAtBegin->setChecked(true);


	mpEndPosition = new QTableView(this);
//	mpEndPosition->setModel(mpModel);
	mpEndPosition->setModel(mpReverseModel);

	connect(mpEndPosition, &QTableView::activated, this, &cCreateLidarExperimentFromGpsDlg::onEndItem);
	connect(mpEndPosition, &QTableView::pressed, this, &cCreateLidarExperimentFromGpsDlg::onEndItem);

	mpEndPosition->verticalHeader()->hide();

	headerView = mpEndPosition->horizontalHeader();
	headerView->setDefaultAlignment(Qt::AlignHCenter);
	headerView->setStretchLastSection(false);

	mpEndPosition->setHorizontalScrollMode(QAbstractItemView::ScrollPerItem);
	mpEndPosition->setSelectionBehavior(QAbstractItemView::SelectRows);
	mpEndPosition->setSelectionMode(QAbstractItemView::SingleSelection);
	mpEndPosition->setSortingEnabled(false);
	mpEndPosition->setFixedWidth(419);

	mpEndPointAtBegin = new QRadioButton("Start of Plot", this);
	mpEndPointAtCenter = new QRadioButton("Center of Plot", this);
	mpEndPointAtEnd = new QRadioButton("End of Plot", this);
	mpEndPointAtEnd->setChecked(true);

	mpClearPath = new QPushButton("Clear Path", this);
	connect(mpClearPath, &QPushButton::pressed, this, &cCreateLidarExperimentFromGpsDlg::clearPaths);

	mpShowPath = new QPushButton("Show Path", this);
	connect(mpShowPath, &QPushButton::pressed, this, &cCreateLidarExperimentFromGpsDlg::onShowPath);

	mpInverseDirection = new QCheckBox("Inverse Direction", this);

	mpUnits = new QComboBox(this);
	mpUnits->setEditable(false);
	mpUnits->addItem("Meters");
	mpUnits->addItem("Millimeters");
	mpUnits->addItem("Feet");
	mpUnits->addItem("Inches");
	connect(mpUnits, &QComboBox::currentTextChanged, this, &cCreateLidarExperimentFromGpsDlg::onUnitChange);
	mConversionFactor = nConstants::M_TO_MM;

	mpPlotLengthLabel = new QLabel(PLOT_LENGTH_TEXT + "m)", this);
	mpPlotLength = new QLineEdit(this);
	mpPlotLength->setValidator(new QDoubleValidator(0, 100.0, 3));
	mpPlotLength->setText("1");
}

void cCreateLidarExperimentFromGpsDlg::createLayout_PointSelection(QVBoxLayout* pMainLayout)
{
	QLabel* pText = nullptr;
	QGroupBox* pGroupBox = nullptr;
	QGridLayout* pGridLayout = nullptr;


	// Setup the "Start" Group Box
	QGroupBox* pStartGroupBox = new QGroupBox(tr("Start of Scan"));
	pStartGroupBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

	QHBoxLayout* pStartLayout = new QHBoxLayout();

	pText = new QLabel("X (mm)");
	pStartLayout->addWidget(pText);
	pStartLayout->addSpacing(5);
	pStartLayout->addWidget(mpStartX_mm);
	pStartLayout->addSpacing(10);
	pText = new QLabel("Y (mm)");
	pStartLayout->addWidget(pText);
	pStartLayout->addSpacing(5);
	pStartLayout->addWidget(mpStartY_mm);

	QVBoxLayout* pVStartLayout = new QVBoxLayout();
	pVStartLayout->addLayout(pStartLayout);
	pVStartLayout->addSpacing(10);
	pVStartLayout->addWidget(mpStartPosition, 1);
	pVStartLayout->addSpacing(10);

	QHBoxLayout* pStartPlacementLayout = new QHBoxLayout();

	pStartPlacementLayout->addStretch(1);
	pStartPlacementLayout->addWidget(mpStartPointAtBegin);
	pStartPlacementLayout->addWidget(mpStartPointAtCenter);
	pStartPlacementLayout->addWidget(mpStartPointAtEnd);
	pStartPlacementLayout->addStretch(1);
	mpStartPointAtBegin->setChecked(true);

	pVStartLayout->addLayout(pStartPlacementLayout);
	pStartGroupBox->setLayout(pVStartLayout);

	// Setup the "End" Group Box
	QGroupBox* pEndGroupBox = new QGroupBox(tr("End of Scan"));
	pEndGroupBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

	QHBoxLayout* pEndLayout = new QHBoxLayout();

	pText = new QLabel("X (mm)");
	pEndLayout->addWidget(pText);
	pEndLayout->addSpacing(5);
	pEndLayout->addWidget(mpEndX_mm);
	pEndLayout->addSpacing(10);
	pText = new QLabel("Y (mm)");
	pEndLayout->addWidget(pText);
	pEndLayout->addSpacing(5);
	pEndLayout->addWidget(mpEndY_mm);

	QVBoxLayout* pVEndLayout = new QVBoxLayout();
	pVEndLayout->addLayout(pEndLayout);
	pVEndLayout->addSpacing(10);
	pVEndLayout->addWidget(mpEndPosition);
	pVEndLayout->addSpacing(10);

	QHBoxLayout* pEndPlacementLayout = new QHBoxLayout();

	pEndPlacementLayout->addStretch(1);
	pEndPlacementLayout->addWidget(mpEndPointAtBegin);
	pEndPlacementLayout->addWidget(mpEndPointAtCenter);
	pEndPlacementLayout->addWidget(mpEndPointAtEnd);
	pEndPlacementLayout->addStretch(1);
	mpEndPointAtEnd->setChecked(true);


	pVEndLayout->addLayout(pEndPlacementLayout);
	pEndGroupBox->setLayout(pVEndLayout);


	QHBoxLayout* pPosLayout = new QHBoxLayout();
	pPosLayout->addStretch(1);
	pPosLayout->addWidget(pStartGroupBox);
	pPosLayout->addSpacing(10);
	pPosLayout->addWidget(pEndGroupBox);
	pPosLayout->addSpacing(10);

	QVBoxLayout* pVSubLayout = new QVBoxLayout();
	pVSubLayout->addWidget(mpClearPath);
	pVSubLayout->addWidget(mpShowPath);
	pPosLayout->addLayout(pVSubLayout);
	pPosLayout->addStretch(1);

	pMainLayout->addLayout(pPosLayout);

	QHBoxLayout* pOptionsLayout = new QHBoxLayout();
	pOptionsLayout->addStretch(1);
	pOptionsLayout->addWidget(mpPlotLengthLabel);
	pOptionsLayout->addWidget(mpPlotLength);
	pOptionsLayout->addSpacing(10);
	pOptionsLayout->addWidget(mpUnits);
	pOptionsLayout->addSpacing(30);
	pOptionsLayout->addWidget(mpInverseDirection);
	pOptionsLayout->addStretch(1);

	pMainLayout->addLayout(pOptionsLayout);

	pMainLayout->addSpacing(10);
}

void cCreateLidarExperimentFromGpsDlg::onUnitChange(const QString& text)
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

void cCreateLidarExperimentFromGpsDlg::onStartItem(const QModelIndex& index)
{
	auto x1 = mpModel->data(index.siblingAtColumn(1)).toFloat();
	auto y1 = mpModel->data(index.siblingAtColumn(2)).toFloat();

	mpStartX_mm->setText(QString::number(static_cast<int>(x1 * nConstants::M_TO_MM)));
	mpStartY_mm->setText(QString::number(static_cast<int>(y1 * nConstants::M_TO_MM)));
}

void cCreateLidarExperimentFromGpsDlg::onEndItem(const QModelIndex& index)
{
//BAF	auto x1 = mpModel->data(index.siblingAtColumn(1)).toFloat();
//BAF	auto y1 = mpModel->data(index.siblingAtColumn(2)).toFloat();
	auto x1 = mpReverseModel->data(index.siblingAtColumn(1)).toFloat();
	auto y1 = mpReverseModel->data(index.siblingAtColumn(2)).toFloat();

	mpEndX_mm->setText(QString::number(static_cast<int>(x1 * nConstants::M_TO_MM)));
	mpEndY_mm->setText(QString::number(static_cast<int>(y1 * nConstants::M_TO_MM)));
}

bool cCreateLidarExperimentFromGpsDlg::generate()
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
		auto pos = title.rfind("Pass");

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
	QModelIndex endIndex = mpEndPosition->currentIndex();

	if ((startIndex.row() < 0) || (endIndex.row() < 0))
	{
		QString msg = "Please select start and end positions.";
		QMessageBox msg_box(QMessageBox::Critical, "Invalid Parameter", msg);
		msg_box.exec();
		return false;
	}

	if (mpStartX_mm->text().isEmpty() || mpStartY_mm->text().isEmpty())
		return false;

	if (mpEndX_mm->text().isEmpty() || mpEndY_mm->text().isEmpty())
		return false;

	int x1_mm = mpStartX_mm->text().toInt();
	int y1_mm = mpStartY_mm->text().toInt();

	int x2_mm = mpEndX_mm->text().toInt();
	int y2_mm = mpEndY_mm->text().toInt();

	double angle = atan2(static_cast<double>(y2_mm) - static_cast<double>(y1_mm), static_cast<double>(x2_mm) - static_cast<double>(x1_mm));

	double plot_length_mm = mpPlotLength->text().toDouble() * mConversionFactor;

	if (mpStartPointAtBegin->isChecked())
	{
		// Do nothing
	}
	else if (mpStartPointAtCenter->isChecked())
	{
		plot_length_mm /= 2.0;
		double dx = plot_length_mm * cos(angle);
		double dy = plot_length_mm * sin(angle);

		x1_mm -= dx;
		y1_mm -= dy;
	}
	else if (mpStartPointAtEnd->isChecked())
	{
		double dx = plot_length_mm * cos(angle);
		double dy = plot_length_mm * sin(angle);

		x1_mm -= dx;
		y1_mm -= dy;
	}

	if (mpEndPointAtBegin->isChecked())
	{
		double dx = plot_length_mm * cos(angle);
		double dy = plot_length_mm * sin(angle);

		x2_mm += dx;
		y2_mm += dy;
	}
	else if (mpEndPointAtCenter->isChecked())
	{
		plot_length_mm /= 2.0;
		double dx = plot_length_mm * cos(angle);
		double dy = plot_length_mm * sin(angle);

		x2_mm += dx;
		y2_mm += dy;
	}
	else if (mpEndPointAtEnd->isChecked())
	{
		// Do nothing
	}


	auto h1 = mpModel->data(startIndex.siblingAtColumn(3)).toFloat();
//BAF	auto h2 = mpModel->data(endIndex.siblingAtColumn(3)).toFloat();
	auto h2 = mpReverseModel->data(endIndex.siblingAtColumn(3)).toFloat();
	
	int h1_mm = static_cast<int>(h1 * nConstants::M_TO_MM);
	int h2_mm = static_cast<int>(h2 * nConstants::M_TO_MM);

	if (mpInverseDirection->isChecked())
	{
		std::swap(x1_mm, x2_mm);
		std::swap(y1_mm, y2_mm);
		std::swap(h1_mm, h2_mm);
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

	bool fastMode = false;

	int numOfScans = 1;
	int orientation = 0;
	double separation_mm = 0.0;
	int lateral_offset_mm = 0;

	if (mpHasSubScans->isChecked())
	{
		numOfScans = mpNumOfScans->text().toInt();
		orientation = mpSubScanOrientation->currentIndex();
		separation_mm = mpSubScanSeparation->text().toDouble() * mSubScanConversionFactor;
		fastMode = mpFastMode->isChecked();

		if (mpScanCenterOnly->isChecked() && (numOfScans > 1))
		{
			double middle_offset_mm = (separation_mm * (numOfScans-1)) / 2.0;

			switch (orientation)
			{
			case SUB_SCAN_NORTH_TO_SOUTH:
				lateral_offset_mm = static_cast<int>(middle_offset_mm);
				break;

			case SUB_SCAN_SOUTH_TO_NORTH:
				lateral_offset_mm = -1 * static_cast<int>(middle_offset_mm);
				break;

			case SUB_SCAN_EAST_TO_WEST:
				lateral_offset_mm = -1 * static_cast<int>(middle_offset_mm);
				break;

			case SUB_SCAN_WEST_TO_EAST:
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
		}
	}

	int dx_mm = x2_mm - x1_mm;
	int dy_mm = y2_mm - y1_mm;

	for (int scan = 0; scan < numOfScans; ++scan)
	{
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
				step->setHeightAGL_mm(scan_z_mm);
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
				x_mm += lateral_offset_mm;

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
				y_mm += lateral_offset_mm;
			}
			else
			{
//				double m = dy_mm / dx_mm;
//				double b = y1_mm - m * x1_mm;
				if (std::abs(dx_mm) < std::abs(dy_mm))
				{
					x_mm = (x2_mm + x1_mm) / 2;
					x_mm += lateral_offset_mm;

					if (y1_mm > y2_mm)
						y_mm = y1_mm + start_offset_mm;
					else
						y_mm = y1_mm - start_offset_mm;
				}
				else
				{
					if (x1_mm > x2_mm)
						x_mm = x1_mm + start_offset_mm;
					else
						x_mm = x1_mm - start_offset_mm;

					y_mm = (y2_mm + y1_mm) / 2;
					y_mm += lateral_offset_mm;
				}

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
			else
			{
				if (std::abs(dx_mm) < std::abs(dy_mm))
				{
					if (y1_mm > y2_mm)
						y_mm = y2_mm - end_offset_mm;
					else
						y_mm = y2_mm + end_offset_mm;
				}
				else
				{
					if (x1_mm > x2_mm)
						x_mm = x2_mm - end_offset_mm;
					else
						x_mm = x2_mm + end_offset_mm;
				}

			}

			auto marker = std::make_unique<cMeasurementStep_Marker>();
			marker->setMarkerType(cMeasurementStep_Marker::eMarkerType::START_OF_MEASUREMENT);
			pInfo->appendStep(std::move(marker));

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

		if (fastMode)
		{
			std::swap(x1_mm, x2_mm);
			std::swap(y1_mm, y2_mm);
			std::swap(h1_mm, h2_mm);
		}
	}

	return true;
}

void cCreateLidarExperimentFromGpsDlg::onShowPath()
{
	QModelIndex startIndex = mpStartPosition->currentIndex();
	if (startIndex.row() < 0)
		return;

	QModelIndex endIndex   = mpEndPosition->currentIndex();
	if (endIndex.row() < 0)
		return;

	auto x1 = mpModel->data(startIndex.siblingAtColumn(1)).toFloat();
	auto y1 = mpModel->data(startIndex.siblingAtColumn(2)).toFloat();

//BAF	auto x2 = mpModel->data(endIndex.siblingAtColumn(1)).toFloat();
//BAF	auto y2 = mpModel->data(endIndex.siblingAtColumn(2)).toFloat();
	auto x2 = mpReverseModel->data(endIndex.siblingAtColumn(1)).toFloat();
	auto y2 = mpReverseModel->data(endIndex.siblingAtColumn(2)).toFloat();

	int x1_mm = static_cast<int>(x1 * nConstants::M_TO_MM);
	int y1_mm = static_cast<int>(y1 * nConstants::M_TO_MM);
	int x2_mm = static_cast<int>(x2 * nConstants::M_TO_MM);
	int y2_mm = static_cast<int>(y2 * nConstants::M_TO_MM);

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
}
