
#include "FieldBoundaryDlg.hpp"

#include <QLabel>
#include <QLayout>
#include <QCheckBox>
#include <QPushButton>
#include <QGridLayout>
#include <QGroupBox>
#include <QDialogButtonBox>
#include <QLineEdit>
#include <QIntValidator>
#include <QPlainTextEdit>

#include <algorithm>


cFieldBoundaryDlg::cFieldBoundaryDlg(QWidget* parent)
:
	QDialog(parent)
{
	setWindowTitle("Field Boundary");

	createControls();
	createLayout();
}

cFieldBoundaryDlg::~cFieldBoundaryDlg()
{}

int cFieldBoundaryDlg::minX_mm() const
{
	auto minX = mpMinX_mm->text().toInt();
	auto maxX = mpMaxX_mm->text().toInt();

	if (maxX < minX)
		return maxX;

	return minX;
}

int cFieldBoundaryDlg::maxX_mm() const
{
	auto minX = mpMinX_mm->text().toInt();
	auto maxX = mpMaxX_mm->text().toInt();

	if (maxX < minX)
		return minX;

	return maxX;
}

int cFieldBoundaryDlg::minY_mm() const
{
	auto minY = mpMinY_mm->text().toInt();
	auto maxY = mpMaxY_mm->text().toInt();

	if (maxY < minY)
		return maxY;

	return minY;
}

int cFieldBoundaryDlg::maxY_mm() const
{
	auto minY = mpMinY_mm->text().toInt();
	auto maxY = mpMaxY_mm->text().toInt();

	if (maxY < minY)
		return minY;

	return maxY;
}

void cFieldBoundaryDlg::setMinX_mm(int limit)
{
	mpMinX_mm->setText(QString::number(limit));
}

void cFieldBoundaryDlg::setMaxX_mm(int limit)
{
	mpMaxX_mm->setText(QString::number(limit));
}

void cFieldBoundaryDlg::setMinY_mm(int limit)
{
	mpMinY_mm->setText(QString::number(limit));
}

void cFieldBoundaryDlg::setMaxY_mm(int limit)
{
	mpMaxY_mm->setText(QString::number(limit));
}

void cFieldBoundaryDlg::createControls()
{
	mpMinX_mm = new QLineEdit(this);
	mpMinX_mm->setValidator(new QIntValidator(0, 190000));

	mpMaxX_mm = new QLineEdit(this);
	mpMaxX_mm->setValidator(new QIntValidator(0, 190000));

	mpMinY_mm = new QLineEdit(this);
	mpMinY_mm->setValidator(new QIntValidator(0, 190000));

	mpMaxY_mm = new QLineEdit(this);
	mpMaxY_mm->setValidator(new QIntValidator(0, 190000));
}

void cFieldBoundaryDlg::createLayout()
{
	QLabel* pText = nullptr;
	QGridLayout* pGridLayout = nullptr;

	QVBoxLayout* pMainLayout = new QVBoxLayout();

	pGridLayout = new QGridLayout();
	pGridLayout->setColumnMinimumWidth(2, 10);

	pText = new QLabel("North Limit (mm)");
	pGridLayout->addWidget(pText, 0, 0);
	pGridLayout->addWidget(mpMinX_mm, 0, 1);

	pText = new QLabel("South Limit (mm)");
	pGridLayout->addWidget(pText, 0, 3);
	pGridLayout->addWidget(mpMaxX_mm, 0, 4);

	pText = new QLabel("West Limit (mm)");
	pGridLayout->addWidget(pText, 2, 0);
	pGridLayout->addWidget(mpMinY_mm, 2, 1);

	pText = new QLabel("East Limit (mm)");
	pGridLayout->addWidget(pText, 2, 3);
	pGridLayout->addWidget(mpMaxY_mm, 2, 4);

	pMainLayout->addLayout(pGridLayout);

	pMainLayout->addSpacing(10);

	QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
		| QDialogButtonBox::Cancel);

	connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
	connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

	pMainLayout->addWidget(buttonBox);

	setLayout(pMainLayout);
}



