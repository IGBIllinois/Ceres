
#include "ScanPointsWidget.hpp"
#include "../../Utilities/Constants.hpp"
#include "MeasurementPointItemModel.hpp"

#include <QTableWidget>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QStringList>
#include <QComboBox>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QIntValidator>
#include <QHeaderView>
#include <QAbstractItemModel>
#include <QStandardItemModel>
#include <QMessageBox>


cScanPointsWidget::cScanPointsWidget(QWidget* parent)
:
	QWidget(parent)
{
	createControls_PointSelection();
	createLayout_PointSelection();

	setMinimumWidth(435);
}

cScanPointsWidget::~cScanPointsWidget()
{
}

void cScanPointsWidget::hideMeasurementPoints(bool hide)
{
	mpMeasurementPoints->setVisible(!hide);
}

void cScanPointsWidget::showMeasurementPoints(bool show)
{
	mpMeasurementPoints->setVisible(show);
}

void cScanPointsWidget::addPoint(int x_mm, int y_mm)
{
	addPoint("", x_mm, y_mm);
}

void cScanPointsWidget::addPoint(const std::string& id, int x_mm, int y_mm)
{
	if (mpStartX_mm->text().isEmpty())
	{
		if (!id.empty())
			mpStartLabel->setText(QString::fromStdString(id));

		mpStartX_mm->setText(QString::number(x_mm));
		mpStartY_mm->setText(QString::number(y_mm));

		return;
	}

	if (mpEndX_mm->text().isEmpty())
	{
		if (!id.empty())
			mpEndLabel->setText(QString::fromStdString(id));

		mpEndX_mm->setText(QString::number(x_mm));
		mpEndY_mm->setText(QString::number(y_mm));

		return;
	}
}

std::vector<sMeasurementPoint> cScanPointsWidget::path()
{
	std::vector<sMeasurementPoint> result;

	if (mpStartX_mm->text().isEmpty())
	{
		sMeasurementPoint point;

		point.x_mm = mpStartX_mm->text().toInt();
		point.y_mm = mpStartY_mm->text().toInt();
		point.scan = static_cast<ePointRepresentsScan>(mpStartType->currentIndex());

		result.push_back(point);
	}

//	mpMeasurementPoints->rowCount();

	if (mpEndX_mm->text().isEmpty())
	{
		sMeasurementPoint point;

		point.x_mm = mpEndX_mm->text().toInt();
		point.y_mm = mpEndY_mm->text().toInt();
		point.scan = static_cast<ePointRepresentsScan>(mpEndType->currentIndex());

		result.push_back(point);
	}

	return result;
}


void cScanPointsWidget::onShowPath()
{
	emit showPath();
}

void cScanPointsWidget::onClearPath()
{
	QMessageBox msgBox(this);

	emit clearPath();

	msgBox.setText("Clear Measurement Path");
	msgBox.setInformativeText("Do you wish clear path data?");
	msgBox.setStandardButtons(QMessageBox::Discard | QMessageBox::Cancel);
	msgBox.setDefaultButton(QMessageBox::Cancel);
	int ret = msgBox.exec();

	if (ret == QMessageBox::Cancel)
		return;

	mpStartLabel->setText("Start");
	mpStartX_mm->setText("");
	mpStartY_mm->setText("");
	mpStartType->setCurrentIndex(0);

	mpEndLabel->setText("End");
	mpEndX_mm->setText("");
	mpEndY_mm->setText("");
	mpEndType->setCurrentIndex(1);
}

void cScanPointsWidget::createControls_PointSelection()
{
	QStringList header;
	header.append(tr("Name"));
	header.append(tr("X (mm)"));
	header.append(tr("Y (mm)"));
	header.append(tr(""));

//	mpStartPosition = new QTableWidget(1, 4, this);
//	mpStartPosition->setHorizontalHeaderLabels(header);
//	mpStartPosition->verticalHeader()->hide();

	mpStartLabel = new QLineEdit("Start", this);

	mpStartX_mm = new QLineEdit(this);
	mpStartX_mm->setValidator(new QIntValidator(10000, 180000));

	mpStartY_mm = new QLineEdit(this);
	mpStartY_mm->setValidator(new QIntValidator(10000, 180000));

	mpStartType = new QComboBox(this);
	mpStartType->addItem("Start");
	mpStartType->addItem("End");
	mpStartType->addItem("Center");
	mpStartType->addItem("Point");

//	mpStartPosition->setCellWidget(0, 0, mpStartLabel);
//	mpStartPosition->setCellWidget(0, 1, mpStartX_mm);
//	mpStartPosition->setCellWidget(0, 2, mpStartY_mm);
//	mpStartPosition->setCellWidget(0, 3, mpStartType);

//	mpStartPosition->setMaximumHeight(50);


	mpMeasurementPoints = new QTableView(this);
	mpMeasurementPoints->horizontalHeader()->hide();
	mpMeasurementPoints->verticalHeader()->hide();

	mpModel = new QPointItemModel(4, mpMeasurementPoints);
	mpMeasurementPoints->setModel(mpModel);


//	mpEndPosition = new QTableWidget(1, 4, this);
//	mpEndPosition->horizontalHeader()->hide();
//	mpEndPosition->verticalHeader()->hide();

	mpEndLabel = new QLineEdit("End", this);

	mpEndX_mm = new QLineEdit(this);
	mpEndX_mm->setValidator(new QIntValidator(10000, 180000));

	mpEndY_mm = new QLineEdit(this);
	mpEndY_mm->setValidator(new QIntValidator(10000, 180000));

	mpEndType = new QComboBox(this);
	mpEndType->addItem("Start");
	mpEndType->addItem("End");
	mpEndType->addItem("Center");
	mpEndType->addItem("Point");
	mpEndType->setCurrentIndex(1);

//	mpEndPosition->setCellWidget(0, 0, mpEndLabel);
//	mpEndPosition->setCellWidget(0, 1, mpEndX_mm);
//	mpEndPosition->setCellWidget(0, 2, mpEndY_mm);
//	mpEndPosition->setCellWidget(0, 3, mpEndType);

//	mpEndPosition->setMaximumHeight(30);

	mpClearPath = new QPushButton("Clear", this);
	connect(mpClearPath, &QPushButton::pressed, this, &cScanPointsWidget::onClearPath);

	mpShowPath = new QPushButton("Show", this);
	connect(mpShowPath, &QPushButton::pressed, this, &cScanPointsWidget::onShowPath);

}

void cScanPointsWidget::createLayout_PointSelection()
{
	QLabel* pText = nullptr;
	QGridLayout* pGridLayout = nullptr;

	QVBoxLayout* pMainLayout = new QVBoxLayout();

	pGridLayout = new QGridLayout();
	pGridLayout->setColumnMinimumWidth(4, 10);

	pText = new QLabel("Name");
	pGridLayout->addWidget(pText, 0, 0, Qt::AlignHCenter);
	pText = new QLabel("X (mm)");
	pGridLayout->addWidget(pText, 0, 1, Qt::AlignHCenter);
	pText = new QLabel("Y (mm)");
	pGridLayout->addWidget(pText, 0, 2, Qt::AlignHCenter);
	pText = new QLabel(" ");
	pGridLayout->addWidget(pText, 0, 3, Qt::AlignHCenter);

	pGridLayout->addWidget(mpStartLabel, 1, 0);
	pGridLayout->addWidget(mpStartX_mm, 1, 1);
	pGridLayout->addWidget(mpStartY_mm, 1, 2);
	pGridLayout->addWidget(mpStartType, 1, 3);

	pMainLayout->addLayout(pGridLayout, 0);

//	pMainLayout->addWidget(mpStartPosition,1);

	pMainLayout->addSpacing(10);

	pMainLayout->addWidget(mpMeasurementPoints, 1);

	pMainLayout->addSpacing(10);

	pGridLayout = new QGridLayout();
	pGridLayout->setColumnMinimumWidth(4, 10);

	pGridLayout->addWidget(mpEndLabel, 0, 0);
	pGridLayout->addWidget(mpEndX_mm, 0, 1);
	pGridLayout->addWidget(mpEndY_mm, 0, 2);
	pGridLayout->addWidget(mpEndType, 0, 3);

/*
	pGridLayout->addWidget(mpEndLabel, 5, 0);
	pGridLayout->addWidget(mpEndX_mm, 5, 1);
	pGridLayout->addWidget(mpEndY_mm, 5, 2);
	pGridLayout->addWidget(mpEndType, 5, 3);
*/

	pMainLayout->addLayout(pGridLayout, 0);

	pMainLayout->addSpacing(10);

	QHBoxLayout* pButtonLayout = new QHBoxLayout();

	pButtonLayout->addStretch(1);
	pButtonLayout->addWidget(mpShowPath);
	pButtonLayout->addStretch(1);
	pButtonLayout->addWidget(mpClearPath);
	pButtonLayout->addStretch(1);

	pMainLayout->addLayout(pButtonLayout, 0);

	setLayout(pMainLayout);
}

