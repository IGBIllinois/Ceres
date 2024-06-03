
#include "CreateExperimentFromPlotInfoDlg.hpp"
#include "GpsFileReader.hpp"
#include "Constants.hpp"

#include "ExperimentSteps.hpp"

#include "ExperimentMetaInfoDlg.hpp"
#include "ExperimentCtrlInfoDlg.hpp"
#include "ExperimentSensorInfoDlg.hpp"

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


cCreateExperimentFromPlotInfoDlg::cCreateExperimentFromPlotInfoDlg(cExperimentFile& info, const QString& filename, QWidget* parent)
:
	mInfo(info), QDialog(parent)
{
	setWindowTitle("Create Experiment");

//	mInfo.clearSteps();

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

	mpClearPath = new QPushButton("Clear Path", this);
	connect(mpClearPath, &QPushButton::pressed, this, &cCreateExperimentFromPlotInfoDlg::clearPaths);

	mpShowPath = new QPushButton("Show Path", this);
	connect(mpShowPath, &QPushButton::pressed, this, &cCreateExperimentFromPlotInfoDlg::onShowPath);

	mpInverseDirection = new QCheckBox("Inverse Direction", this);
	mpUseIntermediatePoints = new QCheckBox("Use Intermediate Points", this);

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

	QVBoxLayout* pVSubLayout = new QVBoxLayout();
	pVSubLayout->addWidget(mpClearPath);
	pVSubLayout->addWidget(mpShowPath);
	pPosLayout->addLayout(pVSubLayout);
	pPosLayout->addStretch(1);

	pMainLayout->addLayout(pPosLayout);

	QHBoxLayout* pOptionsLayout = new QHBoxLayout();
	pOptionsLayout->addStretch(1);
	pOptionsLayout->addWidget(mpInverseDirection);
	pOptionsLayout->addWidget(mpUseIntermediatePoints);
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


	QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
		| QDialogButtonBox::Cancel | QDialogButtonBox::Apply);

	buttonBox->button(QDialogButtonBox::Apply)->setText("Generate");

	mpSaveAs = buttonBox->addButton("Save As", QDialogButtonBox::HelpRole);
	mpSaveAs->setEnabled(false);

	connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
	connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
	connect(buttonBox->button(QDialogButtonBox::Apply), &QPushButton::clicked, this, &cCreateExperimentFromPlotInfoDlg::generate);
	connect(mpSaveAs, &QPushButton::clicked, this, &cCreateExperimentFromPlotInfoDlg::saveExperiment);

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
	QDialog::accept();
}

void cCreateExperimentFromPlotInfoDlg::onMetaInfoUpdate()
{
	cExperimentMetaInfoDlg dlg(mInfo.getMetaData(), this);

	dlg.setExperimentTitle(mpTitle->text().toStdString());

	auto result = dlg.exec();

	if (result == QDialog::Rejected)
		return;

	mpTitle->setText(QString::fromStdString(dlg.getExperimentTitle()));
}

void cCreateExperimentFromPlotInfoDlg::onControllerUpdate()
{
	cExperimentCtrlInfoDlg dlg(mInfo, this);
	dlg.exec();
}

void cCreateExperimentFromPlotInfoDlg::onSensorUpdate()
{
	cExperimentSensorInfoDlg dlg(mInfo, this);
	dlg.exec();
}

void cCreateExperimentFromPlotInfoDlg::generate()
{
	std::string str;
	QString text;
	std::vector<std::string> list;

	str = mpTitle->text().toStdString();
	if (str.empty())
	{
		QString msg = "The \"Experiment Title\" can not be blank.";
		QMessageBox msg_box(QMessageBox::Critical, "Invalid Parameter", msg);
		msg_box.exec();
		return;
	}

	mInfo.setExperimentName(str);

	QModelIndex startIndex = mpStartPosition->currentIndex();

	if (startIndex.row() < 0)
	{
		QString msg = "Please select start and end positions.";
		QMessageBox msg_box(QMessageBox::Critical, "Invalid Parameter", msg);
		msg_box.exec();
		return;
	}

	mInfo.clearSteps();

	// Add preamble...
	std::unique_ptr<cExperimentStep_Movement> step = std::make_unique<cExperimentStep_Movement>();

	int z_mm = static_cast<int>(mpTravelHeight_m->text().toDouble() * nConstants::M_TO_MM);
	int speed_mmps = mpTravelVerticalSpeed_mmps->text().toInt();
	step->setZ_mm(z_mm);
	step->setSpeed_mmps(speed_mmps);
	mInfo.appendStep(std::move(step));

	auto x1 = mpModel->data(startIndex.siblingAtColumn(1)).toFloat();
	auto y1 = mpModel->data(startIndex.siblingAtColumn(2)).toFloat();
	auto h1 = mpModel->data(startIndex.siblingAtColumn(3)).toFloat();

	int x1_mm = static_cast<int>(x1 * nConstants::M_TO_MM);
	int y1_mm = static_cast<int>(y1 * nConstants::M_TO_MM);
	int h1_mm = static_cast<int>(h1 * nConstants::M_TO_MM);

/*
	int dx_mm = x2_mm - x1_mm;
	int dy_mm = y2_mm - y1_mm;

	speed_mmps = mpTravelSpeed_mmps->text().toInt();

	if ((dx_mm == 0) && (dy_mm == 0))
	{
		step = std::make_unique<cExperimentStep_Movement>();
		step->setX_mm(x1_mm);
		step->setY_mm(y1_mm);
		step->setSpeed_mmps(speed_mmps);
		mInfo.appendStep(std::move(step));

		z_mm = static_cast<int>(mpMeasurementHeight_m->text().toDouble() * nConstants::M_TO_MM);
		speed_mmps = mpTravelVerticalSpeed_mmps->text().toInt();

		step = std::make_unique<cExperimentStep_Movement>();

		if (mpHeightReference->currentIndex() == 1)
			step->setZ_mm(z_mm + h1_mm);
		else
			step->setZ_mm(z_mm);

		step->setSpeed_mmps(speed_mmps);
		mInfo.appendStep(std::move(step));

		float delay_sec = mpStartMeasurementDelay_sec->text().toFloat();

		if (delay_sec > 0.0)
		{
			std::unique_ptr<cExperimentStep_Delay> delay = std::make_unique<cExperimentStep_Delay>();
			delay->setWaitTime_sec(delay_sec);
			mInfo.appendStep(std::move(delay));
		}

		delay_sec = mpEndMeasurementDelay_sec->text().toFloat();

		if (delay_sec > 0.0)
		{
			std::unique_ptr<cExperimentStep_Delay> delay = std::make_unique<cExperimentStep_Delay>();
			delay->setWaitTime_sec(delay_sec);
			delay->setRecording(true);
			mInfo.appendStep(std::move(delay));
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
		mInfo.appendStep(std::move(step));

		z_mm = static_cast<int>(mpMeasurementHeight_m->text().toDouble() * nConstants::M_TO_MM);
		speed_mmps = mpTravelVerticalSpeed_mmps->text().toInt();

		step = std::make_unique<cExperimentStep_Movement>();

		if (mpHeightReference->currentIndex() == 1)
			step->setZ_mm(z_mm + h1_mm);
		else
			step->setZ_mm(z_mm);

		step->setSpeed_mmps(speed_mmps);
		mInfo.appendStep(std::move(step));

		float delay_sec = mpStartMeasurementDelay_sec->text().toFloat();

		if (delay_sec > 0.0)
		{
			std::unique_ptr<cExperimentStep_Delay> delay = std::make_unique<cExperimentStep_Delay>();
			delay->setWaitTime_sec(delay_sec);
			mInfo.appendStep(std::move(delay));
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
		mInfo.appendStep(std::move(step));

		delay_sec = mpEndMeasurementDelay_sec->text().toFloat();

		if (delay_sec > 0.0)
		{
			std::unique_ptr<cExperimentStep_Delay> delay = std::make_unique<cExperimentStep_Delay>();
			delay->setWaitTime_sec(delay_sec);
			mInfo.appendStep(std::move(delay));
		}
	}

	z_mm = static_cast<int>(mpSafeHeight_m->text().toDouble() * nConstants::M_TO_MM);
	speed_mmps = mpSafeVerticalSpeed_mmps->text().toInt();

	step = std::make_unique<cExperimentStep_Movement>();
	step->setZ_mm(z_mm);
	step->setSpeed_mmps(speed_mmps);
	mInfo.appendStep(std::move(step));

	mpSaveAs->setEnabled(true);

	emit experimentChanged();
*/
}

void cCreateExperimentFromPlotInfoDlg::onShowPath()
{
	QModelIndex startIndex = mpStartPosition->currentIndex();
	if (startIndex.row() < 0)
		return;

	auto x1 = mpModel->data(startIndex.siblingAtColumn(1)).toFloat();
	auto y1 = mpModel->data(startIndex.siblingAtColumn(2)).toFloat();

	int x1_mm = static_cast<int>(x1 * nConstants::M_TO_MM);
	int y1_mm = static_cast<int>(y1 * nConstants::M_TO_MM);

//	emit drawPath(x1_mm, y1_mm, x2_mm, y2_mm);
}
