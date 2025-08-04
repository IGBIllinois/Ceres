
#include "RecomputeSpidercamHeightDlg.hpp"

#include "RappFieldBoundary.hpp"

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
#include <QStackedLayout>

#include <algorithm>


cRecomputeSpidercamHeightDlg::cRecomputeSpidercamHeightDlg(QWidget* parent)
:
	QDialog(parent)
{
	setWindowTitle("Height Parameters");

	setMinimumWidth(350);

	createControls();
	createLayout();
}

cRecomputeSpidercamHeightDlg::~cRecomputeSpidercamHeightDlg()
{}

int32_t cRecomputeSpidercamHeightDlg::getHeight_mm() const
{
	return mHeight;
}

int32_t cRecomputeSpidercamHeightDlg::getReferenceHeight_mm() const
{
	if (mpReferenceHeight_mm->text().isEmpty())
		return rfm::INVALID_HEIGHT;

	return mpReferenceHeight_mm->text().toInt();
}

void cRecomputeSpidercamHeightDlg::setSensorOffset(int32_t offset_mm)
{
	mpSensorOffset_mm->setText(QString::number(offset_mm));
	compute();
}

void cRecomputeSpidercamHeightDlg::setCanopyHeight_agl(int32_t height_mm)
{
	mpCanopyHeight_mm->setText(QString::number(height_mm));
	compute();
}

void cRecomputeSpidercamHeightDlg::setDesiredHeight_agl(int32_t height_mm)
{
	mpDesiredHeight_mm->setText(QString::number(height_mm));
	compute();
}

void cRecomputeSpidercamHeightDlg::setReferenceHeight_mm(int32_t height_mm)
{
	mpReferenceHeight_mm->setText(QString::number(height_mm));
}

void cRecomputeSpidercamHeightDlg::createControls()
{
	mpSensorOffset_mm = new QLineEdit(this);
	mpSensorOffset_mm->setValidator(new QIntValidator(-2000, 3000));
	mpSensorOffset_mm->setText("1000");

	mpCanopyHeight_mm = new QLineEdit(this);
	mpCanopyHeight_mm->setValidator(new QIntValidator(-2000, 10000));
	mpCanopyHeight_mm->setText("0");

	mpDesiredHeight_mm = new QLineEdit(this);
	mpDesiredHeight_mm->setValidator(new QIntValidator(-2000, 10000));

	mpReferenceHeight_mm = new QLineEdit(this);
	mpReferenceHeight_mm->setValidator(new QIntValidator(2000, 10000));
}

void cRecomputeSpidercamHeightDlg::createLayout()
{
	QLabel* pText = nullptr;
	QGridLayout* pGridLayout = nullptr;

	QVBoxLayout* pMainLayout = new QVBoxLayout();

	pGridLayout = new QGridLayout();
	pGridLayout->setColumnMinimumWidth(4, 10);

	pText = new QLabel("Sensor Offset (mm)");
	pGridLayout->addWidget(pText, 0, 0);
	pGridLayout->addWidget(mpSensorOffset_mm, 0, 1);
	pText = new QLabel("Canopy Height (mm)");
	pGridLayout->addWidget(pText, 2, 0);
	pGridLayout->addWidget(mpCanopyHeight_mm, 2, 1);
	pText = new QLabel("Desired Height Above Plant Canopy (mm)");
	pGridLayout->addWidget(pText, 4, 0);
	pGridLayout->addWidget(mpDesiredHeight_mm, 4, 1);
	pText = new QLabel("Reference Height for Dolly Droop (mm)");
	pGridLayout->addWidget(pText, 6, 0);
	pGridLayout->addWidget(mpReferenceHeight_mm, 6, 1);

	pMainLayout->addLayout(pGridLayout);

	pMainLayout->addSpacing(10);

	QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
		| QDialogButtonBox::Cancel);

	connect(buttonBox, &QDialogButtonBox::accepted, this, &cRecomputeSpidercamHeightDlg::accept);
	connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

	pMainLayout->addWidget(buttonBox);

	setLayout(pMainLayout);
}

void cRecomputeSpidercamHeightDlg::accept()
{
	if (!compute())
	{
		return;
	}

	QDialog::accept();
}

bool cRecomputeSpidercamHeightDlg::compute()
{
	if (mpSensorOffset_mm->text().isEmpty())
		return false;

	if (mpCanopyHeight_mm->text().isEmpty())
		return false;

	if (mpDesiredHeight_mm->text().isEmpty())
		return false;

	int32_t offset_mm = mpSensorOffset_mm->text().toInt();
	int32_t canopy_mm = mpCanopyHeight_mm->text().toInt();
	int32_t height_mm = mpDesiredHeight_mm->text().toInt();

	mHeight = canopy_mm + height_mm + offset_mm;

	return true;
}


