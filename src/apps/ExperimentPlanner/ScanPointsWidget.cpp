
#include "ScanPointsWidget.hpp"
#include "../../Utilities/Constants.hpp"

#include <QTableWidget>
#include <QVBoxLayout>
#include <QStringList>
#include <QComboBox>
#include <QLineEdit>
#include <QLabel>
#include <QIntValidator>


cScanPointsWidget::cScanPointsWidget(QWidget* parent)
:
	QWidget(parent)
{
	createControls_PointSelection();
	createLayout_PointSelection();
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

	mpStartPosition = new QTableWidget(1, 4, this);
	mpStartPosition->setHorizontalHeaderLabels(header);

	mpStartLabel = new QLabel("Start", this);

	mpStartX_mm = new QLineEdit(this);
	mpStartX_mm->setValidator(new QIntValidator(10000, 180000));

	mpStartY_mm = new QLineEdit(this);
	mpStartY_mm->setValidator(new QIntValidator(10000, 180000));

	mpStartType = new QComboBox(this);
	mpStartType->addItem("Start");
	mpStartType->addItem("End");
	mpStartType->addItem("Center");
	mpStartType->addItem("Point");

	mpStartPosition->setCellWidget(0, 0, mpStartLabel);
	mpStartPosition->setCellWidget(0, 1, mpStartX_mm);
	mpStartPosition->setCellWidget(0, 2, mpStartY_mm);
	mpStartPosition->setCellWidget(0, 3, mpStartType);

}

void cScanPointsWidget::createLayout_PointSelection()
{
	QVBoxLayout* pMainLayout = new QVBoxLayout();

	pMainLayout->addWidget(mpStartPosition);

	pMainLayout->addSpacing(10);

	setLayout(pMainLayout);
}

