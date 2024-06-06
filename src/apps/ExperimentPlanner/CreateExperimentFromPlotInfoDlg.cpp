
#include "CreateExperimentFromPlotInfoDlg.hpp"
#include "GpsFileReader.hpp"
#include "Constants.hpp"

#include "ExperimentSteps.hpp"

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


#include <algorithm>
#include <memory>

namespace
{
	const QString PLOT_LENGTH_TEXT = "Plot Length (";

	const QString WEST_TO_EAST = "West to East";
	const QString EAST_TO_WEST = "East to West";
	const QString NORTH_TO_SOUTH = "North to South";
	const QString SOUTH_TO_NORTH = "South to North";
}

cCreateExperimentFromPlotInfoDlg::cCreateExperimentFromPlotInfoDlg(const QString& filename, QWidget* parent)
:
	QDialog(parent)
{
	setWindowTitle("Create Experiment");

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

	createControls();
	createLayout();
}

cCreateExperimentFromPlotInfoDlg::~cCreateExperimentFromPlotInfoDlg()
{}

void cCreateExperimentFromPlotInfoDlg::createControls()
{
	mpTitle = new QLineEdit(this);

	mpMetaInfo = new QPushButton("Meta Info", this);
	connect(mpMetaInfo, &QPushButton::pressed, this, &cCreateExperimentFromPlotInfoDlg::onMetaInfoUpdate);

	mpCtrlInfo = new QPushButton("Controller", this);
	connect(mpCtrlInfo, &QPushButton::pressed, this, &cCreateExperimentFromPlotInfoDlg::onControllerUpdate);

	mpSensorInfo = new QPushButton("Sensors", this);
	connect(mpSensorInfo, &QPushButton::pressed, this, &cCreateExperimentFromPlotInfoDlg::onSensorUpdate);

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
	connect(mpClearPath, &QPushButton::pressed, this, &cCreateExperimentFromPlotInfoDlg::clearPaths);

	mpShowPath = new QPushButton("Show Path", this);
	connect(mpShowPath, &QPushButton::pressed, this, &cCreateExperimentFromPlotInfoDlg::onShowPath);

	mpInverseDirection = new QCheckBox("Inverse Direction", this);

	mpTravelHeight_m = new QLineEdit(this);
	mpTravelHeight_m->setValidator(new QDoubleValidator(5.0, 10.0, 3));
	mpTravelHeight_m->setText("8.0");

	mpTravelVerticalSpeed_mmps = new QLineEdit(this);
	mpTravelVerticalSpeed_mmps->setValidator(new QIntValidator(5, 2000));
	mpTravelVerticalSpeed_mmps->setText("250");

	mpTravelSpeed_mmps = new QLineEdit(this);
	mpTravelSpeed_mmps->setValidator(new QIntValidator(5, 2000));
	mpTravelSpeed_mmps->setText("1000");

	mpBeginningOffset_m = new QLineEdit(this);
	mpBeginningOffset_m->setValidator(new QDoubleValidator(0.0, 10.0, 3));
	mpBeginningOffset_m->setText("2.0");

	mpEndingOffset_m = new QLineEdit(this);
	mpEndingOffset_m->setValidator(new QDoubleValidator(0.0, 10.0, 3));
	mpEndingOffset_m->setText("2.0");

	mpStartMeasurementDelay_sec = new QLineEdit(this);
	mpStartMeasurementDelay_sec->setValidator(new QDoubleValidator(0.0, 300.0, 3));
	mpStartMeasurementDelay_sec->setText("4.0");

	mpMeasurementHeight_m = new QLineEdit(this);
	mpMeasurementHeight_m->setValidator(new QDoubleValidator(0.0, 10.0, 3));
	mpMeasurementHeight_m->setText("5.0");

	mpHeightReference = new QComboBox(this);
	mpHeightReference->setEditable(false);
	mpHeightReference->addItem("SpiderCam");
	mpHeightReference->addItem("AGL");

	mpMeasurementSpeed_mmps = new QLineEdit(this);
	mpMeasurementSpeed_mmps->setValidator(new QIntValidator(0, 1000));
	mpMeasurementSpeed_mmps->setText("450");

	mpEndMeasurementDelay_sec = new QLineEdit(this);
	mpEndMeasurementDelay_sec->setValidator(new QDoubleValidator(0.0, 300.0, 3));
	mpEndMeasurementDelay_sec->setText("1.0");

	mpSafeHeight_m = new QLineEdit(this);
	mpSafeHeight_m->setValidator(new QDoubleValidator(5.0, 10.0, 3));
	mpSafeHeight_m->setText("8.0");

	mpSafeVerticalSpeed_mmps = new QLineEdit(this);
	mpSafeVerticalSpeed_mmps->setValidator(new QIntValidator(5, 2000));
	mpSafeVerticalSpeed_mmps->setText("250");


	mpPlotOrientation = new QComboBox(this);
	mpPlotOrientation->setEditable(false);
	mpPlotOrientation->addItem(NORTH_TO_SOUTH);
	mpPlotOrientation->addItem(SOUTH_TO_NORTH);
	mpPlotOrientation->addItem(EAST_TO_WEST);
	mpPlotOrientation->addItem(WEST_TO_EAST);
//	connect(mpSubScanOrientation, &QComboBox::currentTextChanged, this, &cCreateExperimentFromPlotInfoDlg::onSubOrientationChange);

	mpUnits = new QComboBox(this);
	mpUnits->setEditable(false);
	mpUnits->addItem("Meters");
	mpUnits->addItem("Millimeters");
	mpUnits->addItem("Feet");
	mpUnits->addItem("Inches");
	connect(mpUnits, &QComboBox::currentTextChanged, this, &cCreateExperimentFromPlotInfoDlg::onUnitChange);
	mConversionFactor = nConstants::M_TO_MM;

	mpPlotLengthLabel = new QLabel(PLOT_LENGTH_TEXT + "m)", this);
	mpPlotLength = new QLineEdit(this);
	mpPlotLength->setValidator(new QDoubleValidator(0, 100.0, 3));
	mpPlotLength->setText("1");
}

void cCreateExperimentFromPlotInfoDlg::createLayout()
{
/*
	QLabel* pText = nullptr;
	QGroupBox* pGroupBox = nullptr;
	QGridLayout* pGridLayout = nullptr;
	QHBoxLayout* pHSubLayout = nullptr;
	QVBoxLayout* pVSubLayout = nullptr;

	QVBoxLayout* pMainLayout = new QVBoxLayout();

	QHBoxLayout* pSideBySideLayout = new QHBoxLayout();

	QVBoxLayout* pLeftLayout = new QVBoxLayout();
	createLayout_LeftSide(pLeftLayout);

	QVBoxLayout* pRightLayout = new QVBoxLayout();
	createLayout_RightSide(pRightLayout);

	pSideBySideLayout->addLayout(pLeftLayout);

	//	pSideBySideLayout->addStrut(1);

	pSideBySideLayout->addLayout(pRightLayout);

	pMainLayout->addLayout(pSideBySideLayout);

	pMainLayout->addSpacing(20);
*/

	QLabel* pText = nullptr;
	QGroupBox* pGroupBox = nullptr;
	QGridLayout* pGridLayout = nullptr;

	QVBoxLayout* pMainLayout = new QVBoxLayout();

	QHBoxLayout* pTitleLayout = new QHBoxLayout();
	pText = new QLabel("Experiment Title");
	pTitleLayout->addWidget(pText);
	pTitleLayout->addWidget(mpTitle, 1);
	pTitleLayout->addWidget(mpMetaInfo);
	pTitleLayout->addWidget(mpCtrlInfo);
	pTitleLayout->addWidget(mpSensorInfo);

	pMainLayout->addLayout(pTitleLayout);

	pMainLayout->addSpacing(10);

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

	pGroupBox = new QGroupBox(tr("Preamble Information"));
	pGroupBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

	pGridLayout = new QGridLayout();
	pGridLayout->setColumnMinimumWidth(2, 10);

	pText = new QLabel("Travel Height (m)");
	pGridLayout->addWidget(pText, 0, 0);
	pGridLayout->addWidget(mpTravelHeight_m, 0, 1);

	pText = new QLabel("Vertical Speed (mm/s)");
	pGridLayout->addWidget(pText, 0, 3);
	pGridLayout->addWidget(mpTravelVerticalSpeed_mmps, 0, 4);

	pText = new QLabel("Travel Speed (mm/s)");
	pGridLayout->addWidget(pText, 0, 6);
	pGridLayout->addWidget(mpTravelSpeed_mmps, 0, 7);

	pGroupBox->setLayout(pGridLayout);
	pMainLayout->addWidget(pGroupBox);

	pMainLayout->addSpacing(10);

	pGroupBox = new QGroupBox(tr("Measurement Information"));
	pGroupBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

	pGridLayout = new QGridLayout();
	pGridLayout->setColumnMinimumWidth(2, 10);

	pText = new QLabel("Start Offset (m)");
	pGridLayout->addWidget(pText, 0, 0);
	pGridLayout->addWidget(mpBeginningOffset_m, 0, 1);

	pText = new QLabel("Start Delay (sec)");
	pGridLayout->addWidget(pText, 0, 3);
	pGridLayout->addWidget(mpStartMeasurementDelay_sec, 0, 4);

	pText = new QLabel("Measurement Height (m)");
	pGridLayout->addWidget(pText, 2, 0);

	QHBoxLayout* pMeasurementLayout = new QHBoxLayout();
	pMeasurementLayout->addWidget(mpMeasurementHeight_m, 1);
	pMeasurementLayout->addWidget(mpHeightReference);
	pGridLayout->addLayout(pMeasurementLayout, 2, 1);

//	pGridLayout->addWidget(mpMeasurementHeight_m, 2, 1);


	pText = new QLabel("Measurement Speed (mm/s)");
	pGridLayout->addWidget(pText, 2, 3);
	pGridLayout->addWidget(mpMeasurementSpeed_mmps, 2, 4);

	pText = new QLabel("End Delay (sec)");
	pGridLayout->addWidget(pText, 4, 0);
	pGridLayout->addWidget(mpEndMeasurementDelay_sec, 4, 1);

	pText = new QLabel("End Offset (m)");
	pGridLayout->addWidget(pText, 4, 3);
	pGridLayout->addWidget(mpEndingOffset_m, 4, 4);

	pGroupBox->setLayout(pGridLayout);
	pMainLayout->addWidget(pGroupBox);

	pMainLayout->addSpacing(10);


	pGroupBox = new QGroupBox(tr("Postamble Information"));
	pGroupBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

	pGridLayout = new QGridLayout();
	pGridLayout->setColumnMinimumWidth(2, 10);

	pText = new QLabel("Safe Height (m)");
	pGridLayout->addWidget(pText, 0, 0);
	pGridLayout->addWidget(mpSafeHeight_m, 0, 1);

	pText = new QLabel("Vertical Speed (mm/s)");
	pGridLayout->addWidget(pText, 0, 3);
	pGridLayout->addWidget(mpSafeVerticalSpeed_mmps, 0, 4);

	pGroupBox->setLayout(pGridLayout);
	pMainLayout->addWidget(pGroupBox);

	pMainLayout->addSpacing(10);

	QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
		| QDialogButtonBox::Cancel | QDialogButtonBox::Apply);

	buttonBox->button(QDialogButtonBox::Apply)->setText("Generate");

	connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
	connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
	connect(buttonBox->button(QDialogButtonBox::Apply), &QPushButton::clicked, this, &cCreateExperimentFromPlotInfoDlg::generate);

	pMainLayout->addWidget(buttonBox);

	setLayout(pMainLayout);
}

void cCreateExperimentFromPlotInfoDlg::createLayout_LeftSide(QVBoxLayout* pMainLayout)
{

}

void cCreateExperimentFromPlotInfoDlg::createLayout_RightSide(QVBoxLayout* pMainLayout)
{

}

void cCreateExperimentFromPlotInfoDlg::accept()
{
	generate();
	QDialog::accept();
}

void cCreateExperimentFromPlotInfoDlg::onMetaInfoUpdate()
{
	cExperimentMetaInfoDlg dlg(mMetaInfo, this);

	dlg.setExperimentTitle(mpTitle->text().toStdString());

	auto result = dlg.exec();

	if (result == QDialog::Rejected)
		return;

	mpTitle->setText(QString::fromStdString(dlg.getExperimentTitle()));
}

void cCreateExperimentFromPlotInfoDlg::onControllerUpdate()
{
	cExperimentCtrlInfoDlg dlg(mCtrlInfo.get(), this);

	auto result = dlg.exec();

	if (result == QDialog::Rejected)
		return;

	mCtrlInfo = std::move(dlg.getControllerInfo());
}

void cCreateExperimentFromPlotInfoDlg::onSensorUpdate()
{
	cExperimentSensorInfoDlg dlg(mSensorInfo, this);
	auto result = dlg.exec();

	if (result == QDialog::Accepted)
	{
		mSensorInfo.clear();
		mSensorInfo = dlg.getSensorInfo();
	}
}

void cCreateExperimentFromPlotInfoDlg::onUnitChange(const QString& text)
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


void cCreateExperimentFromPlotInfoDlg::generate()
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
		return;
	}

	QModelIndex startIndex = mpStartPosition->currentIndex();
	QModelIndex endIndex = mpEndPosition->currentIndex();

	if ((startIndex.row() < 0) || (endIndex.row() < 0))
	{
		QString msg = "Please select start and end positions.";
		QMessageBox msg_box(QMessageBox::Critical, "Invalid Parameter", msg);
		msg_box.exec();
		return;
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

	QString plot_orientation = mpPlotOrientation->currentText();
	int plot_length_mm = static_cast<int>(mpPlotLength->text().toDouble() * mConversionFactor);

	int x1_mm = static_cast<int>(x1 * nConstants::M_TO_MM);
	int y1_mm = static_cast<int>(y1 * nConstants::M_TO_MM);
	int h1_mm = static_cast<int>(h1 * nConstants::M_TO_MM);
	int x2_mm = static_cast<int>(x2 * nConstants::M_TO_MM);
	int y2_mm = static_cast<int>(y2 * nConstants::M_TO_MM);
	int h2_mm = static_cast<int>(h2 * nConstants::M_TO_MM);

	int travel_z_mm = static_cast<int>(mpTravelHeight_m->text().toDouble() * nConstants::M_TO_MM);
	int scan_z_mm = static_cast<int>(mpMeasurementHeight_m->text().toDouble() * nConstants::M_TO_MM);
	int safe_z_mm = static_cast<int>(mpSafeHeight_m->text().toDouble() * nConstants::M_TO_MM);

	int dx_mm = x2_mm - x1_mm;
	int dy_mm = y2_mm - y1_mm;

	double scan_direction_deg = atan2(dy_mm, dx_mm) * nConstants::RAD_TO_DEG;

	if ((45.0 <= scan_direction_deg) && (scan_direction_deg < 135.0))
	{
		// We are scanning east to west
		if (plot_orientation == EAST_TO_WEST)
		{
			y2_mm += plot_length_mm;
		}
		else if (plot_orientation == WEST_TO_EAST)
		{
			y1_mm -= plot_length_mm;
		}
		else if (plot_orientation == NORTH_TO_SOUTH)
		{
			x1_mm += (plot_length_mm / 2);
			x2_mm += (plot_length_mm / 2);
		}
		else if (plot_orientation == SOUTH_TO_NORTH)
		{
			x1_mm -= (plot_length_mm / 2);
			x2_mm -= (plot_length_mm / 2);
		}
	}
	else if ((135.0 <= scan_direction_deg) && (scan_direction_deg < 225.0))
	{
		// We are scanning north to south
		if (plot_orientation == NORTH_TO_SOUTH)
		{
			x2_mm += plot_length_mm;
		}
		else if (plot_orientation == SOUTH_TO_NORTH)
		{
			x1_mm -= plot_length_mm;
		}
		else if (plot_orientation == EAST_TO_WEST)
		{
			y1_mm += (plot_length_mm / 2);
			y2_mm += (plot_length_mm / 2);
		}
		else if (plot_orientation == WEST_TO_EAST)
		{
			y1_mm -= (plot_length_mm / 2);
			y2_mm -= (plot_length_mm / 2);
		}
	}
	else if ((225.0 <= scan_direction_deg) && (scan_direction_deg < 315.0))
	{
		// We are scanning west to east
		if (plot_orientation == WEST_TO_EAST)
		{
			y2_mm -= plot_length_mm;
		}
		else if (plot_orientation == EAST_TO_WEST)
		{
			y1_mm += plot_length_mm;
		}
		else if (plot_orientation == NORTH_TO_SOUTH)
		{
			x1_mm += (plot_length_mm / 2);
			x2_mm += (plot_length_mm / 2);
		}
		else if (plot_orientation == SOUTH_TO_NORTH)
		{
			x1_mm -= (plot_length_mm / 2);
			x2_mm -= (plot_length_mm / 2);
		}
	}
	else
	{
		// We are scanning south to north

	}

	int vertical_speed_mmps = mpTravelVerticalSpeed_mmps->text().toInt();
	int travel_speed_mmps = mpTravelSpeed_mmps->text().toInt();
	int scan_speed_mmps = mpMeasurementSpeed_mmps->text().toInt();
	int safe_vertical_speed_mmps = mpSafeVerticalSpeed_mmps->text().toInt();

	int start_offset_mm = static_cast<int>(mpBeginningOffset_m->text().toDouble() * nConstants::M_TO_MM);
	int end_offset_mm = static_cast<int>(mpEndingOffset_m->text().toDouble() * nConstants::M_TO_MM);

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

		if (mpHeightReference->currentIndex() == 1)
			step->setZ_mm(scan_z_mm + h1_mm);
		else
			step->setZ_mm(scan_z_mm);

		step->setSpeed_mmps(vertical_speed_mmps);
		pInfo->appendStep(std::move(step));

		float delay_sec = mpStartMeasurementDelay_sec->text().toFloat();

		if (delay_sec > 0.0)
		{
			// Add delay for dolly to stabilize...
			std::unique_ptr<cExperimentStep_Delay> delay = std::make_unique<cExperimentStep_Delay>();
			delay->setWaitTime_sec(delay_sec);
			pInfo->appendStep(std::move(delay));
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
		step = std::make_unique<cExperimentStep_Movement>();
		step->setX_mm(x_mm);
		step->setY_mm(y_mm);
		step->setSpeed_mmps(travel_speed_mmps);
		pInfo->appendStep(std::move(step));

		// Move the dolly to measurement height...
		step = std::make_unique<cExperimentStep_Movement>();

		if (mpHeightReference->currentIndex() == 1)
			step->setZ_mm(scan_z_mm + h1_mm);
		else
			step->setZ_mm(scan_z_mm);

		step->setSpeed_mmps(vertical_speed_mmps);
		pInfo->appendStep(std::move(step));

		float delay_sec = mpStartMeasurementDelay_sec->text().toFloat();

		if (delay_sec > 0.0)
		{
			// Add delay for dolly to stabilize...
			std::unique_ptr<cExperimentStep_Delay> delay = std::make_unique<cExperimentStep_Delay>();
			delay->setWaitTime_sec(delay_sec);
			pInfo->appendStep(std::move(delay));
		}

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
		step = std::make_unique<cExperimentStep_Movement>();
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
			std::unique_ptr<cExperimentStep_Delay> delay = std::make_unique<cExperimentStep_Delay>();
			delay->setWaitTime_sec(delay_sec);
			pInfo->appendStep(std::move(delay));
		}
	}

	// Move dolly to a safe height to park it
	step = std::make_unique<cExperimentStep_Movement>();
	step->setZ_mm(safe_z_mm);
	step->setSpeed_mmps(safe_vertical_speed_mmps);
	pInfo->appendStep(std::move(step));

	emit experimentChanged(pInfo);
}

void cCreateExperimentFromPlotInfoDlg::onShowPath()
{
	QModelIndex startIndex = mpStartPosition->currentIndex();
	if (startIndex.row() < 0)
		return;

	QModelIndex endIndex   = mpEndPosition->currentIndex();
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

	emit drawPath(x1_mm, y1_mm, x2_mm, y2_mm);
}
