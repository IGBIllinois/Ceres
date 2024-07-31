
#include "MovementStepInfoDlg.hpp"

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


cMovementStepInfoDlg::cMovementStepInfoDlg(QWidget* parent)
:
	QDialog(parent)
{
	setWindowTitle("Movement Parameters");

	createControls();
	createLayout();
}

cMovementStepInfoDlg::~cMovementStepInfoDlg()
{}

void cMovementStepInfoDlg::createControls()
{
	mpX_mm = new QLineEdit(this);
	mpX_mm->setValidator(new QIntValidator(0, 190000));

	mpY_mm = new QLineEdit(this);
	mpY_mm->setValidator(new QIntValidator(0, 190000));

	mpZ_mm = new QLineEdit(this);
	mpZ_mm->setValidator(new QIntValidator(-5000, 10000));


	mpSpeed_mmps = new QLineEdit(this);
	mpSpeed_mmps->setValidator(new QIntValidator(10, 2000));
	mpSpeed_mmps->setText("10");


	mpPan_deg = new QLineEdit(this);
	mpPan_deg->setValidator(new QDoubleValidator(0.0, 360.0, 1));

	mpTilt_deg = new QLineEdit(this);
	mpTilt_deg->setValidator(new QDoubleValidator(-90.0, 90.0, 1));

	mpRoll_deg = new QLineEdit(this);
	mpRoll_deg->setValidator(new QDoubleValidator(-180.0, 180.0, 1));

	mpRecord = new QCheckBox("Record Data During Movement", this);

	mpSampleXY = new QPushButton("Record X, Y", this);
	mpSampleXY->setEnabled(false);
	connect(mpSampleXY, &QPushButton::pressed, this, &cMovementStepInfoDlg::recordXY);

	mpSampleXYZ = new QPushButton("Record X, Y, and Z", this);
	mpSampleXYZ->setEnabled(false);
	connect(mpSampleXYZ, &QPushButton::pressed, this, &cMovementStepInfoDlg::recordXYZ);
}

void cMovementStepInfoDlg::createLayout()
{
	QLabel* pText = nullptr;
	QHBoxLayout* pTopLayout = nullptr;

	QVBoxLayout* pMainLayout = new QVBoxLayout();

	QGroupBox* pGroupBox = new QGroupBox(tr("Linear Information"));
	pGroupBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

	QGridLayout* pGridLayout = new QGridLayout();
	pGridLayout->setColumnMinimumWidth(2, 10);

	pText = new QLabel("X (mm)");
	pGridLayout->addWidget(pText, 0, 0);
	pGridLayout->addWidget(mpX_mm, 0, 1);

	pText = new QLabel("Y (mm)");
	pGridLayout->addWidget(pText, 0, 3);
	pGridLayout->addWidget(mpY_mm, 0, 4);

	pText = new QLabel("Z (mm)");
	pGridLayout->addWidget(pText, 0, 6);
	pGridLayout->addWidget(mpZ_mm, 0, 7);

	pText = new QLabel("Speed (mm/s)");
	pGridLayout->addWidget(pText, 0, 9);
	pGridLayout->addWidget(mpSpeed_mmps, 0, 10);

	pGroupBox->setLayout(pGridLayout);
	pMainLayout->addWidget(pGroupBox);

	pMainLayout->addSpacing(10);

	pGroupBox = new QGroupBox(tr("Orientation Information"));
	pGroupBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

	pGridLayout = new QGridLayout();
	pGridLayout->setColumnMinimumWidth(2, 10);

	pText = new QLabel("Pan (deg)");
	pGridLayout->addWidget(pText, 0, 0);
	pGridLayout->addWidget(mpPan_deg, 0, 1);

	pText = new QLabel("Title (deg)");
	pGridLayout->addWidget(pText, 0, 3);
	pGridLayout->addWidget(mpTilt_deg, 0, 4);

	pText = new QLabel("Roll (deg)");
	pGridLayout->addWidget(pText, 0, 6);
	pGridLayout->addWidget(mpRoll_deg, 0, 7);

	pGroupBox->setLayout(pGridLayout);
	pMainLayout->addWidget(pGroupBox);

	pMainLayout->addSpacing(10);

	pMainLayout->addWidget(mpRecord);

	pMainLayout->addSpacing(10);

	QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
		| QDialogButtonBox::Cancel);

	buttonBox->addButton(mpSampleXY, QDialogButtonBox::ButtonRole::ApplyRole);
	buttonBox->addButton(mpSampleXYZ, QDialogButtonBox::ButtonRole::ApplyRole);

	connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
	connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

	pMainLayout->addWidget(buttonBox);

	setLayout(pMainLayout);
}

void cMovementStepInfoDlg::positionUpdated(spidercam::sPosition_1_t pos)
{
	mpSampleXY->setEnabled(true);
	mpSampleXYZ->setEnabled(true);

	mSpidercamX_mm = pos.X_mm;
	mSpidercamY_mm = pos.Y_mm;
	mSpidercamZ_mm = pos.Z_mm;
}

bool cMovementStepInfoDlg::hasX() const
{
	return !mpX_mm->text().isEmpty();
}

int  cMovementStepInfoDlg::x_mm() const
{
	return mpX_mm->text().toInt();
}

bool cMovementStepInfoDlg::hasY() const
{
	return !mpY_mm->text().isEmpty();
}

int  cMovementStepInfoDlg::y_mm() const
{
	return mpY_mm->text().toInt();
}

bool cMovementStepInfoDlg::hasZ() const
{
	return !mpZ_mm->text().isEmpty();
}

int  cMovementStepInfoDlg::z_mm() const
{
	return mpZ_mm->text().toInt();
}

int  cMovementStepInfoDlg::speed_mmps() const
{
	return mpSpeed_mmps->text().toInt();
}

bool cMovementStepInfoDlg::hasPan() const
{
	return !mpPan_deg->text().isEmpty();
}

double cMovementStepInfoDlg::pan_deg() const
{
	return mpPan_deg->text().toDouble();
}

bool cMovementStepInfoDlg::hasTilt() const
{
	return !mpTilt_deg->text().isEmpty();
}

double cMovementStepInfoDlg::tilt_deg() const
{
	return mpTilt_deg->text().toDouble();
}

bool cMovementStepInfoDlg::hasRoll() const
{
	return !mpRoll_deg->text().isEmpty();
}

double cMovementStepInfoDlg::roll_deg() const
{
	return mpRoll_deg->text().toDouble();
}

bool cMovementStepInfoDlg::recording() const
{
	return mpRecord->isChecked();
}

void cMovementStepInfoDlg::setX_mm(int x_mm)
{
	mpX_mm->setText(QString::number(x_mm));
}

void cMovementStepInfoDlg::setY_mm(int y_mm)
{
	mpY_mm->setText(QString::number(y_mm));
}

void cMovementStepInfoDlg::setZ_mm(int z_mm)
{
	mpZ_mm->setText(QString::number(z_mm));
}

void cMovementStepInfoDlg::setSpeed_mmps(int speed_mmps)
{
	mpSpeed_mmps->setText(QString::number(speed_mmps));
}

void cMovementStepInfoDlg::setPan_deg(double pan_deg)
{
	mpPan_deg->setText(QString::number(pan_deg));
}

void cMovementStepInfoDlg::setTilt_deg(double tilt_deg)
{
	mpTilt_deg->setText(QString::number(tilt_deg));
}

void cMovementStepInfoDlg::setRoll_deg(double roll_deg)
{
	mpRoll_deg->setText(QString::number(roll_deg));
}

void cMovementStepInfoDlg::setRecording(bool recording)
{
	mpRecord->setChecked(recording);
}

void cMovementStepInfoDlg::recordXY()
{
	if ((mSpidercamX_mm > 0) && (mSpidercamY_mm > 0))
	{
		mpX_mm->setText(QString::number(mSpidercamX_mm));
		mpY_mm->setText(QString::number(mSpidercamY_mm));
	}
}

void cMovementStepInfoDlg::recordXYZ()
{
	if ((mSpidercamX_mm > 0) && (mSpidercamY_mm > 0))
	{
		mpX_mm->setText(QString::number(mSpidercamX_mm));
		mpY_mm->setText(QString::number(mSpidercamY_mm));
		mpZ_mm->setText(QString::number(mSpidercamZ_mm));
	}
}
