
#include "ExperimentShiftDlg.hpp"

#include <QLabel>
#include <QLayout>
#include <QCheckBox>
#include <QPushButton>
#include <QGridLayout>
#include <QGroupBox>
#include <QDialogButtonBox>
#include <QLineEdit>
#include <QIntValidator>

#include <algorithm>


cExperimentShiftDlg::cExperimentShiftDlg(QWidget* parent)
:
	QDialog(parent)
{
	setWindowTitle("Shift Parameters");

	createControls();
	createLayout();
}

cExperimentShiftDlg::~cExperimentShiftDlg()
{}

void cExperimentShiftDlg::createControls()
{
	mpX_Shift_mm = new QLineEdit(this);
	mpX_Shift_mm->setValidator(new QIntValidator(-10000, 10000));
	mpX_Shift_mm->setText("0");

	mpY_Shift_mm = new QLineEdit(this);
	mpY_Shift_mm->setValidator(new QIntValidator(-10000, 10000));
	mpY_Shift_mm->setText("0");

	mpZ_Shift_mm = new QLineEdit(this);
	mpZ_Shift_mm->setValidator(new QIntValidator(-2000, 2000));
	mpZ_Shift_mm->setText("0");
}

void cExperimentShiftDlg::createLayout()
{
	QLabel* pText = nullptr;
	QHBoxLayout* pTopLayout = nullptr;

	QVBoxLayout* pMainLayout = new QVBoxLayout();

	QGridLayout* pShiftInfo = new QGridLayout();
	pShiftInfo->setColumnMinimumWidth(2, 10);

	pText = new QLabel("Shift X (+ shifts south) Position (mm)");
	pShiftInfo->addWidget(pText, 0, 0);
	pShiftInfo->addWidget(mpX_Shift_mm, 0, 1);

	pText = new QLabel("Shift Y (+ shifts east) Position (mm)");
	pShiftInfo->addWidget(pText, 1, 0);
	pShiftInfo->addWidget(mpY_Shift_mm, 1, 1);

	pText = new QLabel("Shift Z (+ shifts up) Position (mm)");
	pShiftInfo->addWidget(pText, 2, 0);
	pShiftInfo->addWidget(mpZ_Shift_mm, 2, 1);

	pMainLayout->addLayout(pShiftInfo);

	pMainLayout->addSpacing(10);

	QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
		| QDialogButtonBox::Cancel);

	connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
	connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

	pMainLayout->addWidget(buttonBox);

	setLayout(pMainLayout);
}

int cExperimentShiftDlg::xShift_mm() const
{
	return mpX_Shift_mm->text().toInt();
}

int cExperimentShiftDlg::yShift_mm() const
{
	return mpY_Shift_mm->text().toInt();
}

int cExperimentShiftDlg::zShift_mm() const
{
	return mpZ_Shift_mm->text().toInt();
}


