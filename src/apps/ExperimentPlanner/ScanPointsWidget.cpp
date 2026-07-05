
#include "ScanPointsWidget.hpp"
#include "../../Utilities/Constants.hpp"

#include <QTableWidget>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QStringList>
#include <QComboBox>
#include <QLineEdit>
#include <QLabel>
#include <QIntValidator>
#include <QHeaderView>
#include <QAbstractItemModel>


class QPointItemModel : public QAbstractItemModel
{
public:
};


cScanPointsWidget::cScanPointsWidget(QWidget* parent)
:
	QWidget(parent)
{
	createControls_PointSelection();
	createLayout_PointSelection();

//	setMinimumWidth(420);
	setMinimumWidth(435);
}

cScanPointsWidget::~cScanPointsWidget()
{
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


	mpMeasurementPoints = new QTableWidget(8, 4, this);
	mpMeasurementPoints->horizontalHeader()->hide();
	mpMeasurementPoints->verticalHeader()->hide();


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

	setLayout(pMainLayout);
}

