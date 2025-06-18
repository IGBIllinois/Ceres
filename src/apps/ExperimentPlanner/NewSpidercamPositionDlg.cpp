
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

//
// Set a new X value for the whole experiment
//
cNewSpidercam_X_PositionDlg::cNewSpidercam_X_PositionDlg(int minX_mm, int maxX_mm, QWidget* parent)
:
	QDialog(parent)
{
	setWindowTitle("New Position");

	createControls(minX_mm, maxX_mm);
	createLayout();
}

cNewSpidercam_X_PositionDlg::~cNewSpidercam_X_PositionDlg()
{}

void cNewSpidercam_X_PositionDlg::createControls(int minX_mm, int maxX_mm)
{
	mpX_mm = new QLineEdit(this);
	mpX_mm->setValidator(new QIntValidator(minX_mm, maxX_mm));
	mpX_mm->setText("0");

	mpSampleX = new QPushButton("Record X", this);
	mpSampleX->setEnabled(false);
	connect(mpSampleX, &QPushButton::pressed, this, &cNewSpidercam_X_PositionDlg::recordX);
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

	buttonBox->addButton(mpSampleX, QDialogButtonBox::ButtonRole::ApplyRole);

	connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
	connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

	pMainLayout->addWidget(buttonBox);

	setLayout(pMainLayout);
}

int cNewSpidercam_X_PositionDlg::x_mm() const
{
	return mpX_mm->text().toInt();
}

void cNewSpidercam_X_PositionDlg::setX_mm(int x_mm)
{
	mpX_mm->setText(QString::number(x_mm));
}

void cNewSpidercam_X_PositionDlg::recordX()
{
	if (mSpidercamX_mm > 0)
		mpX_mm->setText(QString::number(mSpidercamX_mm));
}

void cNewSpidercam_X_PositionDlg::positionUpdated(spidercam::sPosition_1_t pos)
{
	mSpidercamX_mm = pos.X_mm;
	mpSampleX->setEnabled(true);
}


//
// Set a new Y value for the whole experiment
//
cNewSpidercam_Y_PositionDlg::cNewSpidercam_Y_PositionDlg(int minY_mm, int maxY_mm, QWidget* parent)
	:
	QDialog(parent)
{
	setWindowTitle("New Position");

	createControls(minY_mm, maxY_mm);
	createLayout();
}

cNewSpidercam_Y_PositionDlg::~cNewSpidercam_Y_PositionDlg()
{}

void cNewSpidercam_Y_PositionDlg::createControls(int minY_mm, int maxY_mm)
{
	mpY_mm = new QLineEdit(this);
	mpY_mm->setValidator(new QIntValidator(minY_mm, maxY_mm));
	mpY_mm->setText("0");

	mpSampleY = new QPushButton("Record Y", this);
	mpSampleY->setEnabled(false);
	connect(mpSampleY, &QPushButton::pressed, this, &cNewSpidercam_Y_PositionDlg::recordY);
}

void cNewSpidercam_Y_PositionDlg::createLayout()
{
	QLabel* pText = nullptr;
	QHBoxLayout* pTopLayout = nullptr;

	QVBoxLayout* pMainLayout = new QVBoxLayout();

	QGridLayout* pShiftInfo = new QGridLayout();
	pShiftInfo->setColumnMinimumWidth(2, 10);

	pText = new QLabel("Y Position (mm)");
	pShiftInfo->addWidget(pText, 0, 0);
	pShiftInfo->addWidget(mpY_mm, 0, 1);

	pMainLayout->addLayout(pShiftInfo);

	pMainLayout->addSpacing(10);

	QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
		| QDialogButtonBox::Cancel);

	buttonBox->addButton(mpSampleY, QDialogButtonBox::ButtonRole::ApplyRole);

	connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
	connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

	pMainLayout->addWidget(buttonBox);

	setLayout(pMainLayout);
}

int cNewSpidercam_Y_PositionDlg::y_mm() const
{
	return mpY_mm->text().toInt();
}

void cNewSpidercam_Y_PositionDlg::setY_mm(int y_mm)
{
	mpY_mm->setText(QString::number(y_mm));
}

void cNewSpidercam_Y_PositionDlg::recordY()
{
	if (mSpidercamY_mm > 0)
		mpY_mm->setText(QString::number(mSpidercamY_mm));
}

void cNewSpidercam_Y_PositionDlg::positionUpdated(spidercam::sPosition_1_t pos)
{
	mSpidercamY_mm = pos.Y_mm;
	mpSampleY->setEnabled(true);
}


//
// Set a new Z value for the whole experiment
//
cNewSpidercam_Z_PositionDlg::cNewSpidercam_Z_PositionDlg(int minZ_mm, int maxZ_mm, QWidget* parent)
	:
	QDialog(parent)
{
	setWindowTitle("New Position");

	createControls(minZ_mm, maxZ_mm);
	createLayout();
}

cNewSpidercam_Z_PositionDlg::~cNewSpidercam_Z_PositionDlg()
{}

void cNewSpidercam_Z_PositionDlg::createControls(int minZ_mm, int maxZ_mm)
{
	mpZ_mm = new QLineEdit(this);
	mpZ_mm->setValidator(new QIntValidator(minZ_mm, maxZ_mm));
	mpZ_mm->setText("0");

	mpSampleZ = new QPushButton("Record Z", this);
	mpSampleZ->setEnabled(false);
	connect(mpSampleZ, &QPushButton::pressed, this, &cNewSpidercam_Z_PositionDlg::recordZ);
}

void cNewSpidercam_Z_PositionDlg::createLayout()
{
	QLabel* pText = nullptr;
	QHBoxLayout* pTopLayout = nullptr;

	QVBoxLayout* pMainLayout = new QVBoxLayout();

	QGridLayout* pShiftInfo = new QGridLayout();
	pShiftInfo->setColumnMinimumWidth(2, 10);

	pText = new QLabel("Z Position (mm)");
	pShiftInfo->addWidget(pText, 0, 0);
	pShiftInfo->addWidget(mpZ_mm, 0, 1);

	pMainLayout->addLayout(pShiftInfo);

	pMainLayout->addSpacing(10);

	QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
		| QDialogButtonBox::Cancel);

	buttonBox->addButton(mpSampleZ, QDialogButtonBox::ButtonRole::ApplyRole);

	connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
	connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

	pMainLayout->addWidget(buttonBox);

	setLayout(pMainLayout);
}

int cNewSpidercam_Z_PositionDlg::z_mm() const
{
	return mpZ_mm->text().toInt();
}

void cNewSpidercam_Z_PositionDlg::setZ_mm(int z_mm)
{
	mpZ_mm->setText(QString::number(z_mm));
}

void cNewSpidercam_Z_PositionDlg::recordZ()
{
	if (mSpidercamZ_mm > 0)
		mpZ_mm->setText(QString::number(mSpidercamZ_mm));
}

void cNewSpidercam_Z_PositionDlg::positionUpdated(spidercam::sPosition_1_t pos)
{
	mSpidercamZ_mm = pos.Z_mm;
	mpSampleZ->setEnabled(true);
}

