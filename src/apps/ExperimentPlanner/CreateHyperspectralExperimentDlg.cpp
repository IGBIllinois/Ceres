
#include "CreateHyperspectralExperimentDlg.hpp"
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

	constexpr int SUB_SCAN_NORTH_TO_SOUTH = 0;
	constexpr int SUB_SCAN_SOUTH_TO_NORTH = 1;
	constexpr int SUB_SCAN_EAST_TO_WEST = 2;
	constexpr int SUB_SCAN_WEST_TO_EAST = 3;
}

cCreateHyperspectralExperimentDlg::cCreateHyperspectralExperimentDlg(QWidget* parent)
:
	QDialog(parent)
{
	setWindowTitle("Create Hyperspectral Experiment");

	mCtrlInfo = std::make_unique<cExperimentCtrlInfo_SpiderCam>();
	mSensorInfo.push_back(std::make_shared<cExperimentSensorInfo_VNIR3000N>());
	mSensorInfo.push_back(std::make_shared<cExperimentSensorInfo_SWIR384>());
}

cCreateHyperspectralExperimentDlg::~cCreateHyperspectralExperimentDlg()
{}

cCreateHyperspectralExperimentDlg::eSubScanOrientation cCreateHyperspectralExperimentDlg::getSubScanOrientation() const
{
	switch (mpSubScanOrientation->currentIndex())
	{
	case SUB_SCAN_WEST_TO_EAST:
		return eSubScanOrientation::WEST_TO_EAST;
	case SUB_SCAN_EAST_TO_WEST:
		return eSubScanOrientation::EAST_TO_WEST;
	case SUB_SCAN_NORTH_TO_SOUTH:
		return eSubScanOrientation::NORTH_TO_SOUTH;
	case SUB_SCAN_SOUTH_TO_NORTH:
		return eSubScanOrientation::SOUTH_TO_NORTH;
	}

	return eSubScanOrientation::NORTH_TO_SOUTH;
}

int cCreateHyperspectralExperimentDlg::getScanDistance_mm() const
{
	if (mpScanDistance->text().isEmpty()) return 0;

	return static_cast<int>(mpScanDistance->text().toDouble() * mScanConversionFactor);
}

int cCreateHyperspectralExperimentDlg::getSubScanSeparation_mm() const
{
	if (mpSubScanSeparation->text().isEmpty()) return 0;

	return static_cast<int>(mpSubScanSeparation->text().toDouble() * mSubScanConversionFactor);
}

void cCreateHyperspectralExperimentDlg::initialize()
{
	createControls();
	createLayout();
}

void cCreateHyperspectralExperimentDlg::createControls()
{
	createControls_TitleInfo();
	createControls_PointSelection();
	createControls_Preamble();
	createControls_Measurement();
	createControls_Postamble();
	createControls_SubScanInfo();
}

void cCreateHyperspectralExperimentDlg::createControls_TitleInfo()
{
	mpTitle = new QLineEdit(this);

	mpMetaInfo = new QPushButton("Meta Info", this);
	connect(mpMetaInfo, &QPushButton::pressed, this, &cCreateHyperspectralExperimentDlg::onMetaInfoUpdate);

	mpCtrlInfo = new QPushButton("Controller", this);
	connect(mpCtrlInfo, &QPushButton::pressed, this, &cCreateHyperspectralExperimentDlg::onControllerUpdate);

	mpSensorInfo = new QPushButton("Sensors", this);
	connect(mpSensorInfo, &QPushButton::pressed, this, &cCreateHyperspectralExperimentDlg::onSensorUpdate);
}

void cCreateHyperspectralExperimentDlg::createControls_Preamble()
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

void cCreateHyperspectralExperimentDlg::createControls_Measurement()
{
	mpStartMeasurementDelay_sec = new QLineEdit(this);
	mpStartMeasurementDelay_sec->setValidator(new QDoubleValidator(0.0, 300.0, 3));
	mpStartMeasurementDelay_sec->setText("4.0");

	// default to feet
	mpScanDistanceLabel = new QLabel(SCAN_DISTANCE_TEXT + "ft)", this);
	mpScanDistance = new QLineEdit(this);
	mpScanDistance->setValidator(new QDoubleValidator(0, 10000.0, 3));
	mpScanDistance->setText("1");

	mpScanUnits = new QComboBox(this);
	mpScanUnits->setEditable(false);
	mpScanUnits->addItem("Meters");
	mpScanUnits->addItem("Millimeters");
	mpScanUnits->addItem("Feet");
	mpScanUnits->addItem("Inches");
	mpScanUnits->setCurrentIndex(2);
	mScanConversionFactor = nConstants::FT_TO_MM;
	connect(mpScanUnits, &QComboBox::currentTextChanged, this, &cCreateHyperspectralExperimentDlg::onScanUnitChange);

	mpHeightOffset = new QLineEdit(this);
	mpHeightOffset->setValidator(new QDoubleValidator(0.0, 10000.0, 3));
	mpHeightOffset->setText("0.0");

	mpAskForOffset = new QCheckBox("Ask", this);
	connect(mpAskForOffset, &QCheckBox::stateChanged, this, &cCreateHyperspectralExperimentDlg::onAskHeightOffset);

	mpLensFocalDistance = new QComboBox(this);
	mpLensFocalDistance->setEditable(false);
	mpLensFocalDistance->addItem("1 m", static_cast<int>(1.0 * nConstants::M_TO_MM));
	mpLensFocalDistance->addItem("3 m", static_cast<int>(3.0 * nConstants::M_TO_MM));
	mpLensFocalDistance->setCurrentIndex(1);

	mpMeasurementSpeed_mmps = new QLineEdit(this);
	mpMeasurementSpeed_mmps->setValidator(new QIntValidator(0, 1000));
	mpMeasurementSpeed_mmps->setText("100");

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

void cCreateHyperspectralExperimentDlg::createControls_Postamble()
{
	mpSafeHeight_m = new QLineEdit(this);
	mpSafeHeight_m->setValidator(new QDoubleValidator(5.0, 10.0, 3));
	mpSafeHeight_m->setText("8.0");

	mpSafeVerticalSpeed_mmps = new QLineEdit(this);
	mpSafeVerticalSpeed_mmps->setValidator(new QIntValidator(5, 2000));
	mpSafeVerticalSpeed_mmps->setText("250");
}

void cCreateHyperspectralExperimentDlg::createControls_SubScanInfo()
{
	mpHasSubScans = new QCheckBox("Has Adjacent Scans", this);
	connect(mpHasSubScans, &QCheckBox::stateChanged, this, &cCreateHyperspectralExperimentDlg::onHasSubScans);

	mpNumOfScans = new QLineEdit(this);
	mpNumOfScans->setValidator(new QIntValidator(1, 10));
	mpNumOfScans->setEnabled(false);
	mpNumOfScans->setText("1");
	connect(mpNumOfScans, &QLineEdit::editingFinished, this, &cCreateHyperspectralExperimentDlg::onNumSubScansChanged);

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
	connect(mpSubScanUnits, &QComboBox::currentTextChanged, this, &cCreateHyperspectralExperimentDlg::onSubScanUnitChange);

	QString label = "Fast Mode (";
	label += QChar(0x2191);
	label += QChar(0x2193);
	label += QChar(0x2191);
	label += ")";

	mpFastMode = new QCheckBox(label, this);

	mpScanOptions = new QGroupBox(this);
	mpScanEveryRow = new QRadioButton("Scan Every Row", mpScanOptions);
	mpScanEveryRow->setChecked(true);
	mpScanEveryRow->setEnabled(false);

	mpScanCenterOnly = new QRadioButton("Scan Center Only", mpScanOptions);
	mpScanCenterOnly->setEnabled(false);

	mpScanInsideRows = new QRadioButton("Scan Inside Rows", mpScanOptions);
	mpScanInsideRows->setEnabled(false);
}

void cCreateHyperspectralExperimentDlg::createLayout()
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
	connect(buttonBox->button(QDialogButtonBox::Apply), &QPushButton::clicked, this, &cCreateHyperspectralExperimentDlg::generate);

	pMainLayout->addWidget(buttonBox);

	setLayout(pMainLayout);
}

void cCreateHyperspectralExperimentDlg::createLayout_TitleInfo(QVBoxLayout* pMainLayout)
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

void cCreateHyperspectralExperimentDlg::createLayout_Preamble(QVBoxLayout* pMainLayout)
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

void cCreateHyperspectralExperimentDlg::createLayout_Measurement(QVBoxLayout* pMainLayout)
{
	QLabel* pText = nullptr;

	QGroupBox* pGroupBox = new QGroupBox(tr("Measurement Information"));
	pGroupBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

	QGridLayout* pGridLayout = new QGridLayout();
	pGridLayout->setColumnMinimumWidth(2, 10);

	pText = new QLabel("Start Delay (sec)");
	pGridLayout->addWidget(pText, 0, 0);
	pGridLayout->addWidget(mpStartMeasurementDelay_sec, 0, 1);

	pText = new QLabel("End Delay (sec)");
	pGridLayout->addWidget(pText, 0, 3);
	pGridLayout->addWidget(mpEndMeasurementDelay_sec, 0, 4);

	pText = new QLabel("Lens Focal Distance");
	pGridLayout->addWidget(pText, 0, 6);
	pGridLayout->addWidget(mpLensFocalDistance, 0, 7);

	pText = new QLabel("Height Offset AGL (m)");
	pGridLayout->addWidget(pText, 2, 0);
	QHBoxLayout* pOffsetLayout = new QHBoxLayout();
	pOffsetLayout->addWidget(mpHeightOffset, 1);
	pOffsetLayout->addWidget(mpAskForOffset);
	pGridLayout->addLayout(pOffsetLayout, 2, 1);

	pGridLayout->addWidget(mpScanDistanceLabel, 2, 3);
	QHBoxLayout* pScanLayout = new QHBoxLayout();
	pScanLayout->addWidget(mpScanDistance, 1);
	pScanLayout->addWidget(mpScanUnits);
	pGridLayout->addLayout(pScanLayout, 2, 4);

	pText = new QLabel("Measurement Speed (mm/s)");
	pGridLayout->addWidget(pText, 2, 6);
	pGridLayout->addWidget(mpMeasurementSpeed_mmps, 2, 7);

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

void cCreateHyperspectralExperimentDlg::createLayout_Postamble(QVBoxLayout* pMainLayout)
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

void cCreateHyperspectralExperimentDlg::createLayout_SubScanInfo(QVBoxLayout * pMainLayout)
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

void cCreateHyperspectralExperimentDlg::accept()
{
	if (!generate())
		return;

	QDialog::accept();
}

void cCreateHyperspectralExperimentDlg::onMetaInfoUpdate()
{
	cExperimentMetaInfoDlg dlg(mMetaInfo, this);

	dlg.setExperimentTitle(mpTitle->text().toStdString());

	auto result = dlg.exec();

	if (result == QDialog::Rejected)
		return;

	mpTitle->setText(QString::fromStdString(dlg.getExperimentTitle()));
}

void cCreateHyperspectralExperimentDlg::onControllerUpdate()
{
	cExperimentCtrlInfoDlg dlg(mCtrlInfo.get(), this);

	auto result = dlg.exec();

	if (result == QDialog::Rejected)
		return;

	mCtrlInfo = std::move(dlg.getControllerInfo());
}

void cCreateHyperspectralExperimentDlg::onSensorUpdate()
{
	cExperimentSensorInfoDlg dlg(mSensorInfo, this);
	auto result = dlg.exec();

	if (result == QDialog::Accepted)
	{
		mSensorInfo.clear();
		mSensorInfo = dlg.getSensorInfo();
	}
}

void cCreateHyperspectralExperimentDlg::onScanUnitChange(const QString& text)
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

void cCreateHyperspectralExperimentDlg::onAskHeightOffset(int state)
{
	if (state == Qt::Checked)
	{
		mpHeightOffset->setEnabled(false);
	}
	else
	{
		mpHeightOffset->setEnabled(true);
	}
}

void cCreateHyperspectralExperimentDlg::onSubScanUnitChange(const QString& text)
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

void cCreateHyperspectralExperimentDlg::onHasSubScans(int state)
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

void cCreateHyperspectralExperimentDlg::onNumSubScansChanged()
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



