
#include "CreateHyperspectralReferenceExperimentDlg.hpp"
#include "GpsFileReader.hpp"
#include "Constants.hpp"

#include "MeasurementSteps.hpp"

#include "ExperimentMetaInfoDlg.hpp"
#include "ExperimentCtrlInfoDlg.hpp"
#include "ExperimentSensorInfoDlg.hpp"
#include "ExperimentSensorInfo.hpp"

#include "RappGroundModel.hpp"
#include "RappFieldBoundary.hpp"

#include "StringUtils.hpp"

#include "ScanPointsWidget.hpp"

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

cCreateHyperspectralReferenceExperimentDlg::cCreateHyperspectralReferenceExperimentDlg(QWidget* parent)
:
	QDialog(parent)
{
	setWindowTitle("Create Hyperspectral Reference Experiment");

	mCtrlInfo = std::make_unique<cExperimentCtrlInfo_SpiderCam>();
	mSensorInfo.push_back(std::make_shared<cExperimentSensorInfo_VNIR3000N>());
	mSensorInfo.push_back(std::make_shared<cExperimentSensorInfo_SWIR384>());
}

cCreateHyperspectralReferenceExperimentDlg::~cCreateHyperspectralReferenceExperimentDlg()
{}

void cCreateHyperspectralReferenceExperimentDlg::initialize()
{
	createControls();
	createLayout();
}

void cCreateHyperspectralReferenceExperimentDlg::createControls()
{
	createControls_TitleInfo();
	createControls_PointSelection();
	createControls_Preamble();
	createControls_Measurement();
	createControls_Postamble();
}

void cCreateHyperspectralReferenceExperimentDlg::createControls_TitleInfo()
{
	mpTitle = new QLineEdit(this);

	mpCtrlInfo = new QPushButton("Controller", this);
	connect(mpCtrlInfo, &QPushButton::pressed, this, &cCreateHyperspectralReferenceExperimentDlg::onControllerUpdate);

	mpRequiredSensorInfo = new QPushButton("Required Sensors", this);
	connect(mpRequiredSensorInfo, &QPushButton::pressed, this, &cCreateHyperspectralReferenceExperimentDlg::onSensorUpdate);
}

void cCreateHyperspectralReferenceExperimentDlg::createControls_Preamble()
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

void cCreateHyperspectralReferenceExperimentDlg::createControls_Measurement()
{
	mpBeginningOffset_m = new QLineEdit(this);
	mpBeginningOffset_m->setValidator(new QDoubleValidator(0.0, 10.0, 3));
	mpBeginningOffset_m->setText("1.0");

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

	mpReferenceHeight_m = new QLineEdit(this);
	mpReferenceHeight_m->setValidator(new QDoubleValidator(0.0, 10.0, 3));
	mpReferenceHeight_m->setText("0.5");

	mpLensFocalDistance = new QComboBox(this);
	mpLensFocalDistance->setEditable(false);
	mpLensFocalDistance->addItem("1 m", static_cast<int>(1.0 * nConstants::M_TO_MM));
	mpLensFocalDistance->addItem("3 m", static_cast<int>(3.0 * nConstants::M_TO_MM));
	mpLensFocalDistance->setCurrentIndex(1);

	mpMeasurementSpeed_mmps = new QLineEdit(this);
	mpMeasurementSpeed_mmps->setValidator(new QIntValidator(0, 1000));
	mpMeasurementSpeed_mmps->setText("100");

	mpMeasurementTime_sec = new QLineEdit(this);
	mpMeasurementTime_sec->setValidator(new QDoubleValidator(0.1, 300.0, 3));
	mpMeasurementTime_sec->setText("");

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

void cCreateHyperspectralReferenceExperimentDlg::createControls_Postamble()
{
	mpSafeHeight_m = new QLineEdit(this);
	mpSafeHeight_m->setValidator(new QDoubleValidator(5.0, 10.0, 3));
	mpSafeHeight_m->setText("8.0");

	mpSafeVerticalSpeed_mmps = new QLineEdit(this);
	mpSafeVerticalSpeed_mmps->setValidator(new QIntValidator(5, 2000));
	mpSafeVerticalSpeed_mmps->setText("250");

	mpGeneratePlacementExperiment = new QCheckBox("Generate Placement Experiment", this);
}

void cCreateHyperspectralReferenceExperimentDlg::createLayout()
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
	connect(buttonBox->button(QDialogButtonBox::Apply), &QPushButton::clicked, this, &cCreateHyperspectralReferenceExperimentDlg::generate);

	pMainLayout->addWidget(buttonBox);

	setLayout(pMainLayout);
}

void cCreateHyperspectralReferenceExperimentDlg::createLayout_TitleInfo(QVBoxLayout* pMainLayout)
{
	QLabel* pText = nullptr;

	QHBoxLayout* pTitleLayout = new QHBoxLayout();
	pText = new QLabel("Experiment Title");
	pTitleLayout->addWidget(pText);
	pTitleLayout->addWidget(mpTitle, 1);
	pTitleLayout->addWidget(mpCtrlInfo);
	pTitleLayout->addWidget(mpRequiredSensorInfo);

	pMainLayout->addLayout(pTitleLayout);

	pMainLayout->addSpacing(10);
}

void cCreateHyperspectralReferenceExperimentDlg::createLayout_Preamble(QVBoxLayout* pMainLayout)
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

void cCreateHyperspectralReferenceExperimentDlg::createLayout_Measurement(QVBoxLayout* pMainLayout)
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

	pText = new QLabel("GPS Integration Time (sec)");
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

	pText = new QLabel("Reference Height AGL (m)");
	pGridLayout->addWidget(pText, 4, 0);
	pGridLayout->addWidget(mpReferenceHeight_m, 4, 1);

//	QHBoxLayout* pMeasurementLayout = new QHBoxLayout();
//	pMeasurementLayout->addWidget(mpMeasurementHeight_m, 1);
//	pMeasurementLayout->addWidget(mpHeightReference);
//	pGridLayout->addLayout(pMeasurementLayout, 2, 1);

	pText = new QLabel("Lens Focal Distance");
	pGridLayout->addWidget(pText, 4, 3);
	pGridLayout->addWidget(mpLensFocalDistance, 4, 4);

	pText = new QLabel("Measurement Speed (mm/s)");
	pGridLayout->addWidget(pText, 6, 0);

	QHBoxLayout* pMeasurementLayout = new QHBoxLayout();

	pMeasurementLayout->addWidget(mpMeasurementSpeed_mmps, 1);
	pText = new QLabel("or Time (sec) ");
	pMeasurementLayout->addWidget(pText);
	pMeasurementLayout->addWidget(mpMeasurementTime_sec, 1);

	pGridLayout->addLayout(pMeasurementLayout, 6, 1);

	pText = new QLabel("End Delay (sec)");
	pGridLayout->addWidget(pText, 6, 3);
	pGridLayout->addWidget(mpEndMeasurementDelay_sec, 6, 4);

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

void cCreateHyperspectralReferenceExperimentDlg::createLayout_Postamble(QVBoxLayout* pMainLayout)
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

	pGridLayout->addWidget(mpGeneratePlacementExperiment, 0, 6);

	pGroupBox->setLayout(pGridLayout);
	pMainLayout->addWidget(pGroupBox);

	pMainLayout->addSpacing(10);
}

void cCreateHyperspectralReferenceExperimentDlg::accept()
{
	if (!generate())
		return;

	QDialog::accept();
}

void cCreateHyperspectralReferenceExperimentDlg::onControllerUpdate()
{
	cExperimentCtrlInfoDlg dlg(mCtrlInfo.get(), this);

	auto result = dlg.exec();

	if (result == QDialog::Rejected)
		return;

	mCtrlInfo = std::move(dlg.getControllerInfo());
}

void cCreateHyperspectralReferenceExperimentDlg::onSensorUpdate()
{
	cExperimentSensorInfoDlg dlg(mSensorInfo, this);
	auto result = dlg.exec();

	if (result == QDialog::Accepted)
	{
		mSensorInfo.clear();
		mSensorInfo = dlg.getSensorInfo();
	}
}

bool cCreateHyperspectralReferenceExperimentDlg::generate()
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

	auto path = mpPath->path();
	if (path.empty())
	{
		QString msg = "The measurement path can not be blank.";
		QMessageBox msg_box(QMessageBox::Critical, "Invalid Parameter", msg);
		msg_box.exec();
		return false;
	}

	auto* pGroundModel = cRappGroundModel::get();

	auto point = path.front();

	int x1_mm = point.x_mm;
	int y1_mm = point.y_mm;

	if (!rfb::withinBoundary(x1_mm, y1_mm))
	{
		QString msg = "The SpiderCam position must be a valid field position (10,000mm to 180,000mm).";
		QMessageBox msg_box(QMessageBox::Critical, "Invalid Parameter", msg);
		msg_box.exec();
		return false;
	}

	int h1_mm = 0;

	if (pGroundModel)
	{
		h1_mm = static_cast<int>(pGroundModel->getMeshHeight_mm(x1_mm, y1_mm));

		if (h1_mm == rfm::INVALID_HEIGHT)
			h1_mm = 0;
	}

	int distance_mm = 0;
	static_cast<int>(mpScanDistance->text().toDouble() * mScanConversionFactor);

	int x2_mm = x1_mm;
	int y2_mm = y1_mm;
	int h2_mm = h1_mm;

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

	if (pGroundModel)
	{
		h2_mm = static_cast<int>(pGroundModel->getMeshHeight_mm(x2_mm, y2_mm));

		if (h2_mm == rfm::INVALID_HEIGHT)
			h2_mm = 0;
	}

	int travel_z_mm = static_cast<int>(mpTravelHeight_m->text().toDouble() * nConstants::M_TO_MM);
	int scan_z_mm = static_cast<int>(mpReferenceHeight_m->text().toDouble() * nConstants::M_TO_MM);
	scan_z_mm += mpLensFocalDistance->currentData().toInt();

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

	int dx_mm = x2_mm - x1_mm;
	int dy_mm = y2_mm - y1_mm;

	int vertical_speed_mmps = mpTravelVerticalSpeed_mmps->text().toInt();
	int travel_speed_mmps = mpTravelSpeed_mmps->text().toInt();
	//BAF	int scan_speed_mmps = mpMeasurementSpeed_mmps->text().toInt();
	int safe_vertical_speed_mmps = mpSafeVerticalSpeed_mmps->text().toInt();

	QSharedPointer<cExperimentFile> pInfo = QSharedPointer<cExperimentFile>(new cExperimentFile());

	pInfo->setExperimentName(title);
	pInfo->setExperimentType(eExperimentType::HYPERSPECTRAL);
	pInfo->setController(copy(mCtrlInfo));
	pInfo->setSensors(mSensorInfo);

	// Add preamble: moving dolly up to a safe travel height...
	std::unique_ptr<cMeasurementStep_Movement> step = std::make_unique<cMeasurementStep_Movement>();

	step->setZ_mm(travel_z_mm);
	step->setSpeed_mmps(vertical_speed_mmps);
	pInfo->appendStep(std::move(step));

	// Moving dolly to the beginning of the measurement scan...
	step = std::make_unique<cMeasurementStep_Movement>();
	step->setX_mm(x1_mm);
	step->setY_mm(y1_mm);
	step->setSpeed_mmps(travel_speed_mmps);
	pInfo->appendStep(std::move(step));

	// Move the dolly to measurement height...
	step = std::make_unique<cMeasurementStep_Movement>();

	step->setZ_mm(scan_z_mm + h1_mm);

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

	for (const auto& sensor : mSensorInfo)
	{
		if (sensor->getType() == vnir_3000N_id)
		{
			// We need to take a background spectra...
			std::unique_ptr<cMeasurementStep_HySpex_Command> close_shutter = std::make_unique<cMeasurementStep_HySpex_Command>("VNIR-3000N", "close shutter");
			pInfo->appendStep(std::move(close_shutter));

			std::unique_ptr<cMeasurementStep_HySpex_Command> background = std::make_unique<cMeasurementStep_HySpex_Command>("VNIR-3000N", "background");
			pInfo->appendStep(std::move(background));

			std::unique_ptr<cMeasurementStep_HySpex_Command> open_shutter = std::make_unique<cMeasurementStep_HySpex_Command>("VNIR-3000N", "open shutter");
			pInfo->appendStep(std::move(open_shutter));
		}

		if (sensor->getType() == swir_384_id)
		{
			// We need to take a background spectra...
			std::unique_ptr<cMeasurementStep_HySpex_Command> close_shutter = std::make_unique<cMeasurementStep_HySpex_Command>("SWIR-384", "close shutter");
			pInfo->appendStep(std::move(close_shutter));

			std::unique_ptr<cMeasurementStep_HySpex_Command> background = std::make_unique<cMeasurementStep_HySpex_Command>("SWIR-384", "background");
			pInfo->appendStep(std::move(background));

			std::unique_ptr<cMeasurementStep_HySpex_Command> open_shutter = std::make_unique<cMeasurementStep_HySpex_Command>("SWIR-384", "open shutter");
			pInfo->appendStep(std::move(open_shutter));
		}
	}

	if ((dx_mm == 0) && (dy_mm == 0))
	{
		float measurement_time_sec = mpMeasurementTime_sec->text().toFloat();

		if (measurement_time_sec > 0.0)
		{
			// Do measurement...
			std::unique_ptr<cMeasurementStep_Delay> delay = std::make_unique<cMeasurementStep_Delay>();
			delay->setWaitTime_sec(measurement_time_sec);
			delay->setRecording(true);
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

	if (mpGeneratePlacementExperiment->isChecked())
	{
		title += " Placement";

		QSharedPointer<cExperimentFile> pInfo = QSharedPointer<cExperimentFile>(new cExperimentFile());

		pInfo->setExperimentName(title);
		pInfo->setExperimentType(eExperimentType::HYPERSPECTRAL);
		pInfo->setController(copy(mCtrlInfo));
		pInfo->setSensors(mSensorInfo);

		// Add preamble: moving dolly up to a safe travel height...
		std::unique_ptr<cMeasurementStep_Movement> step = std::make_unique<cMeasurementStep_Movement>();

		step->setZ_mm(travel_z_mm);
		step->setSpeed_mmps(vertical_speed_mmps);
		pInfo->appendStep(std::move(step));

		// Moving dolly to the beginning of the measurement scan...
		step = std::make_unique<cMeasurementStep_Movement>();
		step->setX_mm(x1_mm);
		step->setY_mm(y1_mm);
		step->setSpeed_mmps(travel_speed_mmps);
		pInfo->appendStep(std::move(step));

		// Move the dolly to measurement height...
		step = std::make_unique<cMeasurementStep_Movement>();

		step->setZ_mm(scan_z_mm + h1_mm);

		step->setSpeed_mmps(vertical_speed_mmps);

		step->setTilt_deg(tilt_deg);
		step->setRoll_deg(roll_deg);
		step->setPan_deg(pan_deg);

		pInfo->appendStep(std::move(step));

		emit experimentChanged(pInfo);
	}

	return true;
}




