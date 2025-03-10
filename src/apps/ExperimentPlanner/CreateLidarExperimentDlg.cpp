
#include "CreateLidarExperimentDlg.hpp"
#include "GpsFileReader.hpp"
#include "Constants.hpp"

#include "ExperimentSteps.hpp"

#include "ExperimentMetaInfoDlg.hpp"
#include "ExperimentCtrlInfoDlg.hpp"
#include "ExperimentSensorInfoDlg.hpp"
#include "ExperimentSensorInfo.hpp"

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

cCreateLidarExperimentDlg::cCreateLidarExperimentDlg(QWidget* parent)
:
	QDialog(parent)
{
	setWindowTitle("Create LiDAR Experiment");

	mCtrlInfo = std::make_unique<cExperimentCtrlInfo_SpiderCam>();
	mSensorInfo.push_back(std::make_shared<cExperimentSensorInfo_Ouster>());
}

cCreateLidarExperimentDlg::~cCreateLidarExperimentDlg()
{}

void cCreateLidarExperimentDlg::initialize()
{
	createControls();
	createLayout();
}

void cCreateLidarExperimentDlg::createControls()
{
	createControls_TitleInfo();
	createControls_PointSelection();
	createControls_Preamble();
	createControls_Measurement();
	createControls_Postamble();
	createControls_SubScanInfo();
}

void cCreateLidarExperimentDlg::createControls_TitleInfo()
{
	mpTitle = new QLineEdit(this);

	mpMetaInfo = new QPushButton("Meta Info", this);
	connect(mpMetaInfo, &QPushButton::pressed, this, &cCreateLidarExperimentDlg::onMetaInfoUpdate);

	mpCtrlInfo = new QPushButton("Controller", this);
	connect(mpCtrlInfo, &QPushButton::pressed, this, &cCreateLidarExperimentDlg::onControllerUpdate);

	mpSensorInfo = new QPushButton("Sensors", this);
	connect(mpSensorInfo, &QPushButton::pressed, this, &cCreateLidarExperimentDlg::onSensorUpdate);
}

void cCreateLidarExperimentDlg::createControls_Preamble()
{
	mpTravelHeight_m = new QLineEdit(this);
	mpTravelHeight_m->setValidator(new QDoubleValidator(5.0, 10.0, 3));
	mpTravelHeight_m->setText("8.0");

	mpTravelVerticalSpeed_mmps = new QLineEdit(this);
	mpTravelVerticalSpeed_mmps->setValidator(new QIntValidator(5, 2000));
	mpTravelVerticalSpeed_mmps->setText("250");

	mpTravelSpeed_mmps = new QLineEdit(this);
	mpTravelSpeed_mmps->setValidator(new QIntValidator(5, 2000));
	mpTravelSpeed_mmps->setText("1000");
}

void cCreateLidarExperimentDlg::createControls_Measurement()
{
	mpBeginningOffset_m = new QLineEdit(this);
	mpBeginningOffset_m->setValidator(new QDoubleValidator(0.0, 10.0, 3));
	mpBeginningOffset_m->setText("2.0");

	mpStartMeasurementDelay_sec = new QLineEdit(this);
	mpStartMeasurementDelay_sec->setValidator(new QDoubleValidator(0.0, 300.0, 3));
	mpStartMeasurementDelay_sec->setText("4.0");

	mpMinIntegrationTime_sec = new QLineEdit(this);
	mpMinIntegrationTime_sec->setValidator(new QDoubleValidator(0.0, 300.0, 3));
	mpMinIntegrationTime_sec->setText("5.0");

	mpMaxIntegrationTime_sec = new QLineEdit(this);
	mpMaxIntegrationTime_sec->setValidator(new QDoubleValidator(5.0, 300.0, 3));
	mpMaxIntegrationTime_sec->setText("10.0");

	mpErrorThreshold_mm = new QLineEdit(this);
	mpErrorThreshold_mm->setValidator(new QIntValidator(50, 1000));
	mpErrorThreshold_mm->setText("100");

	mpMeasurementHeight_m = new QLineEdit(this);
	mpMeasurementHeight_m->setValidator(new QDoubleValidator(0.0, 10.0, 3));
	mpMeasurementHeight_m->setText("4.0");

	mpHeightReference = new QComboBox(this);
	mpHeightReference->setEditable(false);
	mpHeightReference->addItem("SpiderCam");
	mpHeightReference->addItem("AGL");

	mpMeasurementSpeed_mmps = new QLineEdit(this);
	mpMeasurementSpeed_mmps->setValidator(new QIntValidator(0, 1000));
	mpMeasurementSpeed_mmps->setText("450");

	mpEndingOffset_m = new QLineEdit(this);
	mpEndingOffset_m->setValidator(new QDoubleValidator(0.0, 10.0, 3));
	mpEndingOffset_m->setText("2.0");

	mpEndMeasurementDelay_sec = new QLineEdit(this);
	mpEndMeasurementDelay_sec->setValidator(new QDoubleValidator(0.0, 300.0, 3));
	mpEndMeasurementDelay_sec->setText("1.0");

	mpGimbleTilt_deg = new QLineEdit(this);
	mpGimbleTilt_deg->setValidator(new QDoubleValidator(-90.0, 90.0, 3));
	mpGimbleTilt_deg->setText("");

	mpGimbleRoll_deg = new QLineEdit(this);
	mpGimbleRoll_deg->setValidator(new QDoubleValidator(-90.0, 90.0, 3));
	mpGimbleRoll_deg->setText("");

	mpGimblePan_deg = new QLineEdit(this);
	mpGimblePan_deg->setValidator(new QDoubleValidator(0.0, 360.0, 3));
	mpGimblePan_deg->setText("");

	mpSensorOffset_mm = new QLineEdit(this);
	mpSensorOffset_mm->setValidator(new QIntValidator(0, 3000.0));
	mpSensorOffset_mm->setText("1000");
}

void cCreateLidarExperimentDlg::createControls_Postamble()
{
	mpSafeHeight_m = new QLineEdit(this);
	mpSafeHeight_m->setValidator(new QDoubleValidator(5.0, 10.0, 3));
	mpSafeHeight_m->setText("8.0");

	mpSafeVerticalSpeed_mmps = new QLineEdit(this);
	mpSafeVerticalSpeed_mmps->setValidator(new QIntValidator(5, 2000));
	mpSafeVerticalSpeed_mmps->setText("250");
}

void cCreateLidarExperimentDlg::createControls_SubScanInfo()
{
	mpHasSubScans = new QCheckBox("Has Adjacent Scans", this);
	connect(mpHasSubScans, &QCheckBox::stateChanged, this, &cCreateLidarExperimentDlg::onHasSubScans);

	mpNumOfScans = new QLineEdit(this);
	mpNumOfScans->setValidator(new QIntValidator(1, 10));
	mpNumOfScans->setEnabled(false);
	mpNumOfScans->setText("1");
	connect(mpNumOfScans, &QLineEdit::editingFinished, this, &cCreateLidarExperimentDlg::onNumSubScansChanged);

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
	connect(mpSubScanUnits, &QComboBox::currentTextChanged, this, &cCreateLidarExperimentDlg::onSubScanUnitChange);

	QString label = "Fast Mode (";
	label += QChar(0x2191);
	label += QChar(0x2193);
	label += QChar(0x2191);
	label += ")";

	mpFastMode = new QCheckBox(label, this);

	mpScanEveryRow = new QRadioButton("Scan Every Row", this);
	mpScanEveryRow->setChecked(true);
	mpScanEveryRow->setEnabled(false);

	mpScanCenterOnly = new QRadioButton("Scan Center Only", this);
	mpScanCenterOnly->setEnabled(false);

	mpScanInsideRows = new QRadioButton("Scan Inside Rows", this);
	mpScanInsideRows->setEnabled(false);

}

void cCreateLidarExperimentDlg::createLayout()
{
	QLabel* pText = nullptr;
	QGridLayout* pGridLayout = nullptr;

	QVBoxLayout* pMainLayout = new QVBoxLayout();

	createLayout_TitleInfo(pMainLayout);
	createLayout_PointSelection(pMainLayout);
	createLayout_Preamble(pMainLayout);
	createLayout_Measurement(pMainLayout);
	createLayout_Postamble(pMainLayout);
	createLayout_SubScanInfo(pMainLayout);

	QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
		| QDialogButtonBox::Cancel | QDialogButtonBox::Apply);

	buttonBox->button(QDialogButtonBox::Apply)->setText("Generate");

	connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
	connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
	connect(buttonBox->button(QDialogButtonBox::Apply), &QPushButton::clicked, this, &cCreateLidarExperimentDlg::generate);

	pMainLayout->addWidget(buttonBox);

	setLayout(pMainLayout);
}

void cCreateLidarExperimentDlg::createLayout_TitleInfo(QVBoxLayout* pMainLayout)
{
	QLabel* pText = nullptr;

	QHBoxLayout* pTitleLayout = new QHBoxLayout();
	pText = new QLabel("Experiment Title");
	pTitleLayout->addWidget(pText);
	pTitleLayout->addWidget(mpTitle, 1);
	pTitleLayout->addWidget(mpMetaInfo);
	pTitleLayout->addWidget(mpCtrlInfo);
	pTitleLayout->addWidget(mpSensorInfo);

	pMainLayout->addLayout(pTitleLayout);

	pMainLayout->addSpacing(10);
}

void cCreateLidarExperimentDlg::createLayout_Preamble(QVBoxLayout* pMainLayout)
{
	QLabel* pText = nullptr;

	QGroupBox* pGroupBox = new QGroupBox(tr("Preamble Information"));
	pGroupBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

	QGridLayout* pGridLayout = new QGridLayout();
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
}

void cCreateLidarExperimentDlg::createLayout_Measurement(QVBoxLayout* pMainLayout)
{
	QLabel* pText = nullptr;

	QGroupBox* pGroupBox = new QGroupBox(tr("Measurement Information"));
	pGroupBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

	QGridLayout* pGridLayout = new QGridLayout();
	pGridLayout->setColumnMinimumWidth(2, 10);

	pText = new QLabel("Start Offset (m)");
	pGridLayout->addWidget(pText, 0, 0);
	pGridLayout->addWidget(mpBeginningOffset_m, 0, 1);

	pText = new QLabel("Start Delay (sec)");
	pGridLayout->addWidget(pText, 0, 3);
	pGridLayout->addWidget(mpStartMeasurementDelay_sec, 0, 4);

	pText = new QLabel("Integration Time (sec)");
	pGridLayout->addWidget(pText, 2, 0);

	QHBoxLayout* pIntegrationLayout = new QHBoxLayout();
	pText = new QLabel("Min: ");
	pIntegrationLayout->addWidget(pText);
	pIntegrationLayout->addWidget(mpMinIntegrationTime_sec, 1);
	pText = new QLabel("Max: ");
	pIntegrationLayout->addWidget(pText);
	pIntegrationLayout->addWidget(mpMaxIntegrationTime_sec, 1);
	pGridLayout->addLayout(pIntegrationLayout, 2, 1);

	pText = new QLabel("Error Threshold (mm)");
	pGridLayout->addWidget(pText, 2, 3);
	pGridLayout->addWidget(mpErrorThreshold_mm, 2, 4);

	pText = new QLabel("Measurement Height (m)");
	pGridLayout->addWidget(pText, 4, 0);

	//	pGridLayout->addWidget(mpMeasurementHeight_m, 2, 1);
	QHBoxLayout* pMeasurementLayout = new QHBoxLayout();
	pMeasurementLayout->addWidget(mpMeasurementHeight_m, 1);
	pMeasurementLayout->addWidget(mpHeightReference);
	pGridLayout->addLayout(pMeasurementLayout, 4, 1);

	pText = new QLabel("Measurement Speed (mm/s)");
	pGridLayout->addWidget(pText, 4, 3);
	pGridLayout->addWidget(mpMeasurementSpeed_mmps, 4, 4);

	pText = new QLabel("End Delay (sec)");
	pGridLayout->addWidget(pText, 6, 0);
	pGridLayout->addWidget(mpEndMeasurementDelay_sec, 6, 1);

	pText = new QLabel("End Offset (m)");
	pGridLayout->addWidget(pText, 6, 3);
	pGridLayout->addWidget(mpEndingOffset_m, 6, 4);

	pGroupBox->setLayout(pGridLayout);
	pMainLayout->addWidget(pGroupBox);


	pGroupBox = new QGroupBox(tr("Sensor Information"));
	pGroupBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

	pGridLayout = new QGridLayout();
	pGridLayout->setColumnMinimumWidth(2, 10);

	pText = new QLabel("Gimbal Tilt (deg)");
	pGridLayout->addWidget(pText, 0, 0);
	pGridLayout->addWidget(mpGimbleTilt_deg, 0, 1);

	pText = new QLabel("Gimbal Roll (deg)");
	pGridLayout->addWidget(pText, 0, 3);
	pGridLayout->addWidget(mpGimbleRoll_deg, 0, 4);

	pText = new QLabel("Gimbal Pan (deg)");
	pGridLayout->addWidget(pText, 0, 6);
	pGridLayout->addWidget(mpGimblePan_deg, 0, 7);

	pText = new QLabel("Sensor Offset (mm)");
	pGridLayout->addWidget(pText, 0, 9);
	pGridLayout->addWidget(mpSensorOffset_mm, 0, 10);

	pGroupBox->setLayout(pGridLayout);
	pMainLayout->addWidget(pGroupBox);

	pMainLayout->addSpacing(10);
}

void cCreateLidarExperimentDlg::createLayout_Postamble(QVBoxLayout* pMainLayout)
{
	QLabel* pText = nullptr;

	QGroupBox* pGroupBox = new QGroupBox(tr("Postamble Information"));
	pGroupBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

	QGridLayout* pGridLayout = new QGridLayout();
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
}

void cCreateLidarExperimentDlg::createLayout_SubScanInfo(QVBoxLayout* pMainLayout)
{
	QLabel* pText = nullptr;

	QGroupBox* pGroupBox = new QGroupBox(tr("Sub Scan Information"));
	pGroupBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

	QVBoxLayout* pVSubLayout = new QVBoxLayout();

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

	pVSubLayout->addLayout(pHSubLayout);

	pHSubLayout = new QHBoxLayout();
	pHSubLayout->addWidget(mpScanEveryRow);
	pHSubLayout->addWidget(mpScanCenterOnly);
	pHSubLayout->addWidget(mpScanInsideRows);
	pHSubLayout->addStretch(1);

	pVSubLayout->addLayout(pHSubLayout);

	pGroupBox->setLayout(pVSubLayout);

	pMainLayout->addWidget(pGroupBox);
}

void cCreateLidarExperimentDlg::accept()
{
	if (!generate())
		return;

	QDialog::accept();
}

void cCreateLidarExperimentDlg::onMetaInfoUpdate()
{
	cExperimentMetaInfoDlg dlg(mMetaInfo, this);

	dlg.setExperimentTitle(mpTitle->text().toStdString());

	auto result = dlg.exec();

	if (result == QDialog::Rejected)
		return;

	mpTitle->setText(QString::fromStdString(dlg.getExperimentTitle()));
}

void cCreateLidarExperimentDlg::onControllerUpdate()
{
	cExperimentCtrlInfoDlg dlg(mCtrlInfo.get(), this);

	auto result = dlg.exec();

	if (result == QDialog::Rejected)
		return;

	mCtrlInfo = std::move(dlg.getControllerInfo());
}

void cCreateLidarExperimentDlg::onSensorUpdate()
{
	cExperimentSensorInfoDlg dlg(mSensorInfo, this);
	auto result = dlg.exec();

	if (result == QDialog::Accepted)
	{
		mSensorInfo.clear();
		mSensorInfo = dlg.getSensorInfo();
	}
}

void cCreateLidarExperimentDlg::onSubScanUnitChange(const QString& text)
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


void cCreateLidarExperimentDlg::onHasSubScans(int state)
{
	if (state == Qt::Checked)
	{
		mpNumOfScans->setEnabled(true);
		mpSubScanOrientation->setEnabled(true);
		mpSubScanUnits->setEnabled(true);
		mpSubScanSeparation->setEnabled(true);
		mpFastMode->setEnabled(true);
		mpScanEveryRow->setEnabled(true);

		int num = mpNumOfScans->text().toInt();
		if (num > 1)
			mpScanCenterOnly->setEnabled(true);
		if (num > 2)
			mpScanInsideRows->setEnabled(true);
	}
	else
	{
		mpNumOfScans->setEnabled(false);
		mpSubScanOrientation->setEnabled(false);
		mpSubScanUnits->setEnabled(false);
		mpSubScanSeparation->setEnabled(false);
		mpFastMode->setEnabled(false);
		mpScanEveryRow->setEnabled(false);
		mpScanCenterOnly->setEnabled(false);
		mpScanInsideRows->setEnabled(false);
	}
}

void cCreateLidarExperimentDlg::onNumSubScansChanged()
{
	if (!mpHasSubScans->isChecked())
		return;

	int num = mpNumOfScans->text().toInt();
	if (num > 1)
		mpScanCenterOnly->setEnabled(true);
	else
		mpScanCenterOnly->setEnabled(false);

	if (num > 2)
		mpScanInsideRows->setEnabled(true);
	else
		mpScanInsideRows->setEnabled(false);
}



