
#include "CreateExperimentFromSpiderCamPointDlg.hpp"
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

cCreateExperimentFromSpiderCamDlg::cCreateExperimentFromSpiderCamDlg(QWidget* parent)
:
	QDialog(parent)
{
	setWindowTitle("Create Experiment");

	setMinimumWidth(550);

	createControls();
	createLayout();
}

cCreateExperimentFromSpiderCamDlg::~cCreateExperimentFromSpiderCamDlg()
{}

void cCreateExperimentFromSpiderCamDlg::createControls()
{
	mpTitle = new QLineEdit(this);

	mpMetaInfo = new QPushButton("Meta Info", this);
	connect(mpMetaInfo, &QPushButton::pressed, this, &cCreateExperimentFromSpiderCamDlg::onMetaInfoUpdate);

	mpCtrlInfo = new QPushButton("Controller", this);
	connect(mpCtrlInfo, &QPushButton::pressed, this, &cCreateExperimentFromSpiderCamDlg::onControllerUpdate);

	mpSensorInfo = new QPushButton("Sensors", this);
	connect(mpSensorInfo, &QPushButton::pressed, this, &cCreateExperimentFromSpiderCamDlg::onSensorUpdate);

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
	connect(mpScanUnits, &QComboBox::currentTextChanged, this, &cCreateExperimentFromSpiderCamDlg::onScanUnitChange);


	mpClearPath = new QPushButton("Clear Path", this);
	connect(mpClearPath, &QPushButton::pressed, this, &cCreateExperimentFromSpiderCamDlg::clearPaths);

	mpShowPath = new QPushButton("Show Path", this);
	connect(mpShowPath, &QPushButton::pressed, this, &cCreateExperimentFromSpiderCamDlg::onShowPath);

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


	mpHasSubScans = new QCheckBox("Has Adjacent Scans", this);
	connect(mpHasSubScans, &QCheckBox::stateChanged, this, &cCreateExperimentFromSpiderCamDlg::onHasSubScans);

	mpNumOfScans = new QLineEdit(this);
	mpNumOfScans->setValidator(new QIntValidator(1, 10));
	mpNumOfScans->setEnabled(false);
	mpNumOfScans->setText("1");

	mpSubScanOrientation = new QComboBox(this);
	mpSubScanOrientation->setEditable(false);
	mpSubScanOrientation->addItem(NORTH_TO_SOUTH);
	mpSubScanOrientation->addItem(SOUTH_TO_NORTH);
	mpSubScanOrientation->addItem(EAST_TO_WEST);
	mpSubScanOrientation->addItem(WEST_TO_EAST);
	mpSubScanOrientation->setEnabled(false);
//	connect(mpSubScanOrientation, &QComboBox::currentTextChanged, this, &cCreateExperimentFromGpsDlg::onSubOrientationChange);

	 // default to inches
	mpSubScanSeparationLabel = new QLabel(SCAN_SEPARATION_TEXT + "in)", this);
	mpSubScanSeparation = new QLineEdit(this);
	mpSubScanSeparation->setValidator(new QDoubleValidator(0, 100.0, 3));
	mpSubScanSeparation->setText("0");
	mpSubScanSeparation->setEnabled(false);

	mpSubScanUnits = new QComboBox(this);
	mpSubScanUnits->setEditable(false);
	mpSubScanUnits->addItem("Meters");
	mpSubScanUnits->addItem("Millimeters");
	mpSubScanUnits->addItem("Feet");
	mpSubScanUnits->addItem("Inches");
	mpSubScanUnits->setEnabled(false);
	mpSubScanUnits->setCurrentIndex(3);
	mSubScanConversionFactor = nConstants::IN_TO_MM;
	connect(mpSubScanUnits, &QComboBox::currentTextChanged, this, &cCreateExperimentFromSpiderCamDlg::onSubScanUnitChange);

	QString label = "Fast Mode (";
	label += QChar(0x2191);
	label += QChar(0x2193);
	label += QChar(0x2191);
	label += ")";

	mpFastMode = new QCheckBox(label, this);
}

void cCreateExperimentFromSpiderCamDlg::createLayout()
{
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

	pPosLayout->addStretch(1);

	QVBoxLayout* pVSubLayout = new QVBoxLayout();
	pVSubLayout->addWidget(mpClearPath);
	pVSubLayout->addWidget(mpShowPath);
	pPosLayout->addLayout(pVSubLayout);
	pPosLayout->addStretch(1);

	pMainLayout->addLayout(pPosLayout);

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
	pGridLayout->addWidget(mpMeasurementHeight_m, 2, 1);

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


	pGroupBox = new QGroupBox(tr("Sub Scan Information"));
	pGroupBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

	QHBoxLayout* pHSubLayout = new QHBoxLayout();
	pHSubLayout->addWidget(mpHasSubScans);
	pText = new QLabel("Number of Scans: ");
	pHSubLayout->addWidget(pText);
	pHSubLayout->addWidget(mpNumOfScans);
	pHSubLayout->addSpacing(10);
	pHSubLayout->addWidget(mpSubScanOrientation);
	pHSubLayout->addSpacing(10);
	pHSubLayout->addWidget(mpFastMode);

	pHSubLayout->addWidget(mpSubScanSeparationLabel);
	pHSubLayout->addWidget(mpSubScanSeparation);
	pHSubLayout->addSpacing(10);
	pHSubLayout->addWidget(mpSubScanUnits);

	pGroupBox->setLayout(pHSubLayout);

	pMainLayout->addWidget(pGroupBox);


	QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
		| QDialogButtonBox::Cancel | QDialogButtonBox::Apply);

	buttonBox->button(QDialogButtonBox::Apply)->setText("Generate");

	connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
	connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
	connect(buttonBox->button(QDialogButtonBox::Apply), &QPushButton::clicked, this, &cCreateExperimentFromSpiderCamDlg::generate);

	pMainLayout->addWidget(buttonBox);

	setLayout(pMainLayout);
}

void cCreateExperimentFromSpiderCamDlg::accept()
{
	generate();
	QDialog::accept();
}

void cCreateExperimentFromSpiderCamDlg::onMetaInfoUpdate()
{
	cExperimentMetaInfoDlg dlg(mMetaInfo, this);

	dlg.setExperimentTitle(mpTitle->text().toStdString());

	auto result = dlg.exec();

	if (result == QDialog::Rejected)
		return;

	mpTitle->setText(QString::fromStdString(dlg.getExperimentTitle()));
}

void cCreateExperimentFromSpiderCamDlg::onControllerUpdate()
{
	cExperimentCtrlInfoDlg dlg(mCtrlInfo.get(), this);

	auto result = dlg.exec();

	if (result == QDialog::Rejected)
		return;

	mCtrlInfo = std::move(dlg.getControllerInfo());
}

void cCreateExperimentFromSpiderCamDlg::onSensorUpdate()
{
	cExperimentSensorInfoDlg dlg(mSensorInfo, this);
	auto result = dlg.exec();

	if (result == QDialog::Accepted)
	{
		mSensorInfo.clear();
		mSensorInfo = dlg.getSensorInfo();
	}
}

void cCreateExperimentFromSpiderCamDlg::onScanUnitChange(const QString& text)
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

void cCreateExperimentFromSpiderCamDlg::onSubScanUnitChange(const QString& text)
{
	double separation = mpSubScanSeparation->text().toDouble() * mSubScanConversionFactor;

	switch (mpSubScanUnits->currentIndex())
	{
	case 0:
		mpSubScanSeparationLabel->setText(SCAN_SEPARATION_TEXT + "m)");

		mSubScanConversionFactor = nConstants::M_TO_MM;
		break;
	case 1:
		mpSubScanSeparationLabel->setText(SCAN_SEPARATION_TEXT + "mm)");

		mSubScanConversionFactor = 1.0;
		break;
	case 2:
		mpSubScanSeparationLabel->setText(SCAN_SEPARATION_TEXT + "ft)");

		mSubScanConversionFactor = nConstants::FT_TO_MM;
		break;
	case 3:
		mpSubScanSeparationLabel->setText(SCAN_SEPARATION_TEXT + "in)");

		mSubScanConversionFactor = nConstants::IN_TO_MM;
		break;
	}

	separation /= mSubScanConversionFactor;

	mpSubScanSeparation->setText(QString::number(separation));
}


void cCreateExperimentFromSpiderCamDlg::onHasSubScans(int state)
{
	if (state == Qt::Checked)
	{
		mpNumOfScans->setEnabled(true);
		mpSubScanOrientation->setEnabled(true);
		mpSubScanUnits->setEnabled(true);
		mpSubScanSeparation->setEnabled(true);
		mpFastMode->setEnabled(true);

	}
	else
	{
		mpNumOfScans->setEnabled(false);
		mpSubScanOrientation->setEnabled(false);
		mpSubScanUnits->setEnabled(false);
		mpSubScanSeparation->setEnabled(false);
		mpFastMode->setEnabled(false);
	}
}

void cCreateExperimentFromSpiderCamDlg::generate()
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

	int travel_z_mm = static_cast<int>(mpTravelHeight_m->text().toDouble() * nConstants::M_TO_MM);
	int scan_z_mm = static_cast<int>(mpMeasurementHeight_m->text().toDouble() * nConstants::M_TO_MM);
	int safe_z_mm = static_cast<int>(mpSafeHeight_m->text().toDouble() * nConstants::M_TO_MM);

	int dx_mm = x2_mm - x1_mm;
	int dy_mm = y2_mm - y1_mm;

	int vertical_speed_mmps = mpTravelVerticalSpeed_mmps->text().toInt();
	int travel_speed_mmps = mpTravelSpeed_mmps->text().toInt();
	int scan_speed_mmps = mpMeasurementSpeed_mmps->text().toInt();
	int safe_vertical_speed_mmps = mpSafeVerticalSpeed_mmps->text().toInt();

	int start_offset_mm = static_cast<int>(mpBeginningOffset_m->text().toDouble() * nConstants::M_TO_MM);
	int end_offset_mm = static_cast<int>(mpEndingOffset_m->text().toDouble() * nConstants::M_TO_MM);


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
}

void cCreateExperimentFromSpiderCamDlg::onShowPath()
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
