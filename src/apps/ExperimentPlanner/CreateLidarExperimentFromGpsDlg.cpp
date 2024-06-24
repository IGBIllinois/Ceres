
#include "CreateLidarExperimentFromGpsDlg.hpp"
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

cCreateLidarExperimentFromGpsDlg::~cCreateLidarExperimentFromGpsDlg()
{}

void cCreateLidarExperimentFromGpsDlg::createControls_PointSelection()
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
	connect(mpClearPath, &QPushButton::pressed, this, &cCreateLidarExperimentFromGpsDlg::clearPaths);

	mpShowPath = new QPushButton("Show Path", this);
	connect(mpShowPath, &QPushButton::pressed, this, &cCreateLidarExperimentFromGpsDlg::onShowPath);

	mpInverseDirection = new QCheckBox("Inverse Direction", this);
}

void cCreateLidarExperimentFromGpsDlg::createLayout_PointSelection(QVBoxLayout* pMainLayout)
{
	QLabel* pText = nullptr;
	QGroupBox* pGroupBox = nullptr;
	QGridLayout* pGridLayout = nullptr;

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
}

bool cCreateLidarExperimentFromGpsDlg::generate()
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

	QModelIndex startIndex = mpStartPosition->currentIndex();
	QModelIndex endIndex = mpEndPosition->currentIndex();

	if ((startIndex.row() < 0) || (endIndex.row() < 0))
	{
		QString msg = "Please select start and end positions.";
		QMessageBox msg_box(QMessageBox::Critical, "Invalid Parameter", msg);
		msg_box.exec();
		return false;
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

	int x1_mm = static_cast<int>(x1 * nConstants::M_TO_MM);
	int y1_mm = static_cast<int>(y1 * nConstants::M_TO_MM);
	int h1_mm = static_cast<int>(h1 * nConstants::M_TO_MM);
	int x2_mm = static_cast<int>(x2 * nConstants::M_TO_MM);
	int y2_mm = static_cast<int>(y2 * nConstants::M_TO_MM);
	int h2_mm = static_cast<int>(h2 * nConstants::M_TO_MM);

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

			step->setTilt_deg(tilt_deg);
			step->setRoll_deg(roll_deg);
			step->setPan_deg(pan_deg);

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
		step->setTilt_deg(safe_tilt_deg);
		step->setRoll_deg(safe_roll_deg);
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

	auto x2 = mpModel->data(endIndex.siblingAtColumn(1)).toFloat();
	auto y2 = mpModel->data(endIndex.siblingAtColumn(2)).toFloat();

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
