
#include "CreateExperimentFromGpsDlg.hpp"
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
	const QString SCAN_SEPARATION_TEXT = "Separation (";

	const QString WEST_TO_EAST = "West to East";
	const QString EAST_TO_WEST = "East to West";
	const QString NORTH_TO_SOUTH = "North to South";
	const QString SOUTH_TO_NORTH = "South to North";
}

cCreateExperimentFromGpsDlg::cCreateExperimentFromGpsDlg(const QString& filename, QWidget* parent)
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

cCreateExperimentFromGpsDlg::~cCreateExperimentFromGpsDlg()
{}

void cCreateExperimentFromGpsDlg::createControls()
{
	mpTitle = new QLineEdit(this);

	mpMetaInfo = new QPushButton("Meta Info", this);
	connect(mpMetaInfo, &QPushButton::pressed, this, &cCreateExperimentFromGpsDlg::onMetaInfoUpdate);

	mpCtrlInfo = new QPushButton("Controller", this);
	connect(mpCtrlInfo, &QPushButton::pressed, this, &cCreateExperimentFromGpsDlg::onControllerUpdate);

	mpSensorInfo = new QPushButton("Sensors", this);
	connect(mpSensorInfo, &QPushButton::pressed, this, &cCreateExperimentFromGpsDlg::onSensorUpdate);

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
	connect(mpClearPath, &QPushButton::pressed, this, &cCreateExperimentFromGpsDlg::clearPaths);

	mpShowPath = new QPushButton("Show Path", this);
	connect(mpShowPath, &QPushButton::pressed, this, &cCreateExperimentFromGpsDlg::onShowPath);

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


	mpHasSubScans = new QCheckBox("Has Adjacent Scans", this);
	connect(mpHasSubScans, &QCheckBox::stateChanged, this, &cCreateExperimentFromGpsDlg::onHasSubScans);

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

	mpUnits = new QComboBox(this);
	mpUnits->setEditable(false);
	mpUnits->addItem("Meters");
	mpUnits->addItem("Millimeters");
	mpUnits->addItem("Feet");
	mpUnits->addItem("Inches");
	mpUnits->setEnabled(false);
	connect(mpUnits, &QComboBox::currentTextChanged, this, &cCreateExperimentFromGpsDlg::onUnitChange);

	mpSubScanSeparationLabel = new QLabel(SCAN_SEPARATION_TEXT + "m)", this);
	mpSubScanSeparation = new QLineEdit(this);
	mpSubScanSeparation->setValidator(new QDoubleValidator(0, 100.0, 3));
	mpSubScanSeparation->setText("0");
	mpSubScanSeparation->setEnabled(false);

	QString label = "Fast Mode (";
	label += QChar(0x2191);
	label += QChar(0x2193);
	label += QChar(0x2191);
	label += ")";

	mpFastMode = new QCheckBox(label, this);
}

void cCreateExperimentFromGpsDlg::createLayout()
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
	pHSubLayout->addWidget(mpUnits);

	pGroupBox->setLayout(pHSubLayout);

	pMainLayout->addWidget(pGroupBox);


	QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
		| QDialogButtonBox::Cancel | QDialogButtonBox::Apply);

	buttonBox->button(QDialogButtonBox::Apply)->setText("Generate");

	connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
	connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
	connect(buttonBox->button(QDialogButtonBox::Apply), &QPushButton::clicked, this, &cCreateExperimentFromGpsDlg::generate);

	pMainLayout->addWidget(buttonBox);

	setLayout(pMainLayout);
}

void cCreateExperimentFromGpsDlg::accept()
{
	generate();
	QDialog::accept();
}

void cCreateExperimentFromGpsDlg::onMetaInfoUpdate()
{
	cExperimentMetaInfoDlg dlg(mMetaInfo, this);

	dlg.setExperimentTitle(mpTitle->text().toStdString());

	auto result = dlg.exec();

	if (result == QDialog::Rejected)
		return;

	mpTitle->setText(QString::fromStdString(dlg.getExperimentTitle()));
}

void cCreateExperimentFromGpsDlg::onControllerUpdate()
{
//	cExperimentCtrlInfoDlg dlg(mExperimentFile, this);

//	auto result = dlg.exec();

//	if (result == QDialog::Rejected)
//		return;
}

void cCreateExperimentFromGpsDlg::onSensorUpdate()
{
	cExperimentSensorInfoDlg dlg(mSensorInfo, this);
	auto result = dlg.exec();

	if (result == QDialog::Accepted)
	{
		mSensorInfo.clear();
		mSensorInfo = dlg.getSensorInfo();
	}
}

void cCreateExperimentFromGpsDlg::onUnitChange(const QString& text)
{
	double separation = mpSubScanSeparation->text().toDouble() * mConversionFactor;

	switch (mpUnits->currentIndex())
	{
	case 0:
		mpSubScanSeparationLabel->setText(SCAN_SEPARATION_TEXT + "m)");

		mConversionFactor = nConstants::M_TO_MM;
		break;
	case 1:
		mpSubScanSeparationLabel->setText(SCAN_SEPARATION_TEXT + "mm)");

		mConversionFactor = 1.0;
		break;
	case 2:
		mpSubScanSeparationLabel->setText(SCAN_SEPARATION_TEXT + "ft)");

		mConversionFactor = nConstants::FT_TO_MM;
		break;
	case 3:
		mpSubScanSeparationLabel->setText(SCAN_SEPARATION_TEXT + "in)");

		mConversionFactor = nConstants::IN_TO_MM;
		break;
	}

	separation /= mConversionFactor;

	mpSubScanSeparation->setText(QString::number(separation));
}


void cCreateExperimentFromGpsDlg::onHasSubScans(int state)
{
	if (state == Qt::Checked)
	{
		mpNumOfScans->setEnabled(true);
		mpSubScanOrientation->setEnabled(true);
		mpUnits->setEnabled(true);
		mpSubScanSeparation->setEnabled(true);
		mpFastMode->setEnabled(true);

	}
	else
	{
		mpNumOfScans->setEnabled(false);
		mpSubScanOrientation->setEnabled(false);
		mpUnits->setEnabled(false);
		mpSubScanSeparation->setEnabled(false);
		mpFastMode->setEnabled(false);
	}
}

void cCreateExperimentFromGpsDlg::generate()
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

	QSharedPointer<cExperimentFile> pInfo = QSharedPointer<cExperimentFile>(new cExperimentFile());

	int startNum = 0;
	bool hasNumber = nStringUtils::endsWithInt(title, &startNum);

	if (hasNumber)
		nStringUtils::replaceIntAtEnd(title, startNum++);

	pInfo->setExperimentName(title);
	pInfo->setMetaData(mMetaInfo);
	pInfo->setSensors(mSensorInfo);

	// Add preamble...
	std::unique_ptr<cExperimentStep_Movement> step = std::make_unique<cExperimentStep_Movement>();

	int z_mm = static_cast<int>(mpTravelHeight_m->text().toDouble() * nConstants::M_TO_MM);
	int speed_mmps = mpTravelVerticalSpeed_mmps->text().toInt();
	step->setZ_mm(z_mm);
	step->setSpeed_mmps(speed_mmps);
	pInfo->appendStep(std::move(step));

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

	int dx_mm = x2_mm - x1_mm;
	int dy_mm = y2_mm - y1_mm;

	speed_mmps = mpTravelSpeed_mmps->text().toInt();

	bool mFastMode = false;

	int numOfScans = 1;
	double separation_mm = 0.0;

	if (mpHasSubScans->isChecked())
	{
		numOfScans = mpNumOfScans->text().toInt();
//		QComboBox* mpSubScanOrientation = nullptr;
		separation_mm = mpSubScanSeparation->text().toDouble() * mConversionFactor;
		mFastMode = mpFastMode->isChecked();
	}


	if ((dx_mm == 0) && (dy_mm == 0))
	{
		step = std::make_unique<cExperimentStep_Movement>();
		step->setX_mm(x1_mm);
		step->setY_mm(y1_mm);
		step->setSpeed_mmps(speed_mmps);
		pInfo->appendStep(std::move(step));

		z_mm = static_cast<int>(mpMeasurementHeight_m->text().toDouble() * nConstants::M_TO_MM);
		speed_mmps = mpTravelVerticalSpeed_mmps->text().toInt();

		step = std::make_unique<cExperimentStep_Movement>();

		if (mpHeightReference->currentIndex() == 1)
			step->setZ_mm(z_mm + h1_mm);
		else
			step->setZ_mm(z_mm);

		step->setSpeed_mmps(speed_mmps);
		pInfo->appendStep(std::move(step));

		float delay_sec = mpStartMeasurementDelay_sec->text().toFloat();

		if (delay_sec > 0.0)
		{
			std::unique_ptr<cExperimentStep_Delay> delay = std::make_unique<cExperimentStep_Delay>();
			delay->setWaitTime_sec(delay_sec);
			pInfo->appendStep(std::move(delay));
		}

		delay_sec = mpEndMeasurementDelay_sec->text().toFloat();

		if (delay_sec > 0.0)
		{
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
		int offset_mm = static_cast<int>(mpBeginningOffset_m->text().toDouble() * nConstants::M_TO_MM);

		if (std::abs(dx_mm) < 500)
		{
			x_mm = (x2_mm + x1_mm) / 2;

			if (y1_mm > y2_mm)
				y_mm = y1_mm + offset_mm;
			else
				y_mm = y1_mm - offset_mm;
		}
		else if (std::abs(dy_mm) < 500)
		{
			if (x1_mm > x2_mm)
				x_mm = x1_mm + offset_mm;
			else
				x_mm = x1_mm - offset_mm;

			y_mm = (y2_mm + y1_mm) / 2;
		}

		step = std::make_unique<cExperimentStep_Movement>();
		step->setX_mm(x_mm);
		step->setY_mm(y_mm);
		step->setSpeed_mmps(speed_mmps);
		pInfo->appendStep(std::move(step));

		z_mm = static_cast<int>(mpMeasurementHeight_m->text().toDouble() * nConstants::M_TO_MM);
		speed_mmps = mpTravelVerticalSpeed_mmps->text().toInt();

		step = std::make_unique<cExperimentStep_Movement>();

		if (mpHeightReference->currentIndex() == 1)
			step->setZ_mm(z_mm + h1_mm);
		else
			step->setZ_mm(z_mm);

		step->setSpeed_mmps(speed_mmps);
		pInfo->appendStep(std::move(step));

		float delay_sec = mpStartMeasurementDelay_sec->text().toFloat();

		if (delay_sec > 0.0)
		{
			std::unique_ptr<cExperimentStep_Delay> delay = std::make_unique<cExperimentStep_Delay>();
			delay->setWaitTime_sec(delay_sec);
			pInfo->appendStep(std::move(delay));
		}

		offset_mm = static_cast<int>(mpEndingOffset_m->text().toDouble() * nConstants::M_TO_MM);
		speed_mmps = mpMeasurementSpeed_mmps->text().toInt();

		if (std::abs(dx_mm) < 500)
		{
			if (y1_mm > y2_mm)
				y_mm = y2_mm - offset_mm;
			else
				y_mm = y2_mm + offset_mm;
		}
		else if (std::abs(dy_mm) < 500)
		{
			if (x1_mm > x2_mm)
				x_mm = x2_mm - offset_mm;
			else
				x_mm = x2_mm + offset_mm;
		}

		step = std::make_unique<cExperimentStep_Movement>();
		step->setX_mm(x_mm);
		step->setY_mm(y_mm);

		if ((mpHeightReference->currentIndex() == 1) &&
			((z_mm + h1_mm) != (z_mm + h2_mm)))
		{
			step->setZ_mm(z_mm + h2_mm);
		}

		step->setSpeed_mmps(speed_mmps);
		step->setRecording(true);
		pInfo->appendStep(std::move(step));

		delay_sec = mpEndMeasurementDelay_sec->text().toFloat();

		if (delay_sec > 0.0)
		{
			std::unique_ptr<cExperimentStep_Delay> delay = std::make_unique<cExperimentStep_Delay>();
			delay->setWaitTime_sec(delay_sec);
			pInfo->appendStep(std::move(delay));
		}
	}

	z_mm = static_cast<int>(mpSafeHeight_m->text().toDouble() * nConstants::M_TO_MM);
	speed_mmps = mpSafeVerticalSpeed_mmps->text().toInt();

	step = std::make_unique<cExperimentStep_Movement>();
	step->setZ_mm(z_mm);
	step->setSpeed_mmps(speed_mmps);
	pInfo->appendStep(std::move(step));

	emit experimentChanged(pInfo);
}

void cCreateExperimentFromGpsDlg::onShowPath()
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


	if (mpHasSubScans->isChecked())
	{
		int separation_mm = static_cast<int>(mpSubScanSeparation->text().toDouble() * mConversionFactor);

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
