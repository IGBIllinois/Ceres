
#include "NewSpidercamPositionDlg.hpp"

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


cNewSpidercam_X_PositionDlg::cNewSpidercam_X_PositionDlg(QWidget* parent)
:
	QDialog(parent)
{
	setWindowTitle("Shift Parameters");

	createControls();
	createLayout();
}

cNewSpidercam_X_PositionDlg::~cNewSpidercam_X_PositionDlg()
{}

void cNewSpidercam_X_PositionDlg::createControls()
{
	mpX_mm = new QLineEdit(this);
	mpX_mm->setValidator(new QIntValidator(10000, 190000));
	mpX_mm->setText("0");
}

void cNewSpidercam_X_PositionDlg::createLayout()
{
	QLabel* pText = nullptr;
	QHBoxLayout* pTopLayout = nullptr;

	QVBoxLayout* pMainLayout = new QVBoxLayout();

	QGridLayout* pShiftInfo = new QGridLayout();
	pShiftInfo->setColumnMinimumWidth(2, 10);

	pText = new QLabel("X Position (mm)");
	pShiftInfo->addWidget(pText, 0, 0);
	pShiftInfo->addWidget(mpX_mm, 0, 1);

	pMainLayout->addLayout(pShiftInfo);

	pMainLayout->addSpacing(10);

	QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
		| QDialogButtonBox::Cancel);

	connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
	connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

	pMainLayout->addWidget(buttonBox);

	setLayout(pMainLayout);
}

int cNewSpidercam_X_PositionDlg::x_mm() const
{
	return mpX_mm->text().toInt();
}

