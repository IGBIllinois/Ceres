
#include "CreateThermalExperimentDlg.hpp"
#include "GpsFileReader.hpp"
#include "Constants.hpp"

#include "MeasurementSteps.hpp"

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

cCreateThermalExperimentDlg::cCreateThermalExperimentDlg(QWidget* parent)
:
	QDialog(parent)
{
	setWindowTitle("Create Thermal Measurement");

	mCtrlInfo = std::make_unique<cExperimentCtrlInfo_SpiderCam>();
	mSensorInfo.push_back(std::make_shared<cExperimentSensorInfo_FLIR>());
}

cCreateThermalExperimentDlg::~cCreateThermalExperimentDlg()
{}

void cCreateThermalExperimentDlg::initialize()
{
	createControls();
	createLayout();
}

void cCreateThermalExperimentDlg::createControls()
{
	createControls_TitleInfo();
	createControls_PointSelection();
	createControls_Preamble();
	createControls_Measurement();
	createControls_Postamble();
}

void cCreateThermalExperimentDlg::createControls_TitleInfo()
{
	mpTitle = new QLineEdit(this);
	mpAutoAdvance = new QCheckBox("Auto Advance", this);
	mpAutoAdvance->setChecked(true);

	mpMetaInfo = new QPushButton("Meta Info", this);
	connect(mpMetaInfo, &QPushButton::pressed, this, &cCreateThermalExperimentDlg::onMetaInfoUpdate);

	mpCtrlInfo = new QPushButton("Controller", this);
	connect(mpCtrlInfo, &QPushButton::pressed, this, &cCreateThermalExperimentDlg::onControllerUpdate);

	mpRequiredSensorInfo = new QPushButton("Required Sensors", this);
	connect(mpRequiredSensorInfo, &QPushButton::pressed, this, &cCreateThermalExperimentDlg::onSensorUpdate);
}

void cCreateThermalExperimentDlg::createControls_Preamble()
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

void cCreateThermalExperimentDlg::createControls_Measurement()
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

void cCreateThermalExperimentDlg::createControls_Postamble()
{
	mpSafeHeight_m = new QLineEdit(this);
	mpSafeHeight_m->setValidator(new QDoubleValidator(5.0, 10.0, 3));
	mpSafeHeight_m->setText("8.0");

	mpSafeVerticalSpeed_mmps = new QLineEdit(this);
	mpSafeVerticalSpeed_mmps->setValidator(new QIntValidator(5, 2000));
	mpSafeVerticalSpeed_mmps->setText("250");
}

void cCreateThermalExperimentDlg::createLayout()
{
	QLabel* pText = nullptr;
	QGridLayout* pGridLayout = nullptr;

	QVBoxLayout* pMainLayout = new QVBoxLayout();

	createLayout_TitleInfo(pMainLayout);
	createLayout_PointSelection(pMainLayout);
	createLayout_Preamble(pMainLayout);
	createLayout_Measurement(pMainLayout);
	createLayout_Postamble(pMainLayout);

	QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
		| QDialogButtonBox::Cancel | QDialogButtonBox::Apply);

	buttonBox->button(QDialogButtonBox::Apply)->setText("Generate");

	connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
	connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
	connect(buttonBox->button(QDialogButtonBox::Apply), &QPushButton::clicked, this, &cCreateThermalExperimentDlg::generate);

	pMainLayout->addWidget(buttonBox);

	setLayout(pMainLayout);
}

void cCreateThermalExperimentDlg::createLayout_TitleInfo(QVBoxLayout* pMainLayout)
{
	QLabel* pText = nullptr;

	QHBoxLayout* pTitleLayout = new QHBoxLayout();
	pText = new QLabel("Measurement Title");
	pTitleLayout->addWidget(pText);
	pTitleLayout->addWidget(mpTitle, 1);
	pTitleLayout->addWidget(mpAutoAdvance);
	pTitleLayout->addWidget(mpMetaInfo);
	pTitleLayout->addWidget(mpCtrlInfo);
	pTitleLayout->addWidget(mpRequiredSensorInfo);

	pMainLayout->addLayout(pTitleLayout);

	pMainLayout->addSpacing(10);
}

void cCreateThermalExperimentDlg::createLayout_Preamble(QVBoxLayout* pMainLayout)
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

void cCreateThermalExperimentDlg::createLayout_Measurement(QVBoxLayout* pMainLayout)
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

void cCreateThermalExperimentDlg::createLayout_Postamble(QVBoxLayout* pMainLayout)
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

void cCreateThermalExperimentDlg::accept()
{
	if (mMeasurementTitle.empty())
	{
		if (!generate())
			return;
	}

	QDialog::accept();
}

void cCreateThermalExperimentDlg::onMetaInfoUpdate()
{
	cExperimentMetaInfoDlg dlg(mMetaInfo, this);

	std::string title = mpTitle->text().toStdString();

	dlg.setMeasurementTitle(title);

	if (!title.empty() && mExperimentTitle.empty())
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

	dlg.setExperimentTitle(mExperimentTitle);

	auto result = dlg.exec();

	if (result == QDialog::Rejected)
		return;

	mExperimentTitle = dlg.getExperimentTitle();
	mMeasurementTitle = dlg.getMeasurementTitle();

	mpTitle->setText(QString::fromStdString(mMeasurementTitle));
}

void cCreateThermalExperimentDlg::onControllerUpdate()
{
	cExperimentCtrlInfoDlg dlg(mCtrlInfo.get(), this);

	auto result = dlg.exec();

	if (result == QDialog::Rejected)
		return;

	mCtrlInfo = std::move(dlg.getControllerInfo());
}

void cCreateThermalExperimentDlg::onSensorUpdate()
{
	cExperimentSensorInfoDlg dlg(mSensorInfo, this);
	auto result = dlg.exec();

	if (result == QDialog::Accepted)
	{
		mSensorInfo.clear();
		mSensorInfo = dlg.getSensorInfo();
	}
}

