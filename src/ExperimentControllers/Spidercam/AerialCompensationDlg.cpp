
#include "AerialCompensationDlg.hpp"
#include "../../Utilities/RappFieldBoundary.hpp"
#include "../../Utilities/RappFieldModel.hpp"

#include <QLayout>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QIntValidator>
#include <QDialogButtonBox>


/*****************************************************************************
 * Above GROUND Height Dialog
 ****************************************************************************/

cAboveGroundHeightDlg::cAboveGroundHeightDlg()
	: QDialog()
{
	setWindowTitle("Above Ground Height Parameters");

	setMinimumWidth(350);

	createControls();
	createLayout();
}

cAboveGroundHeightDlg::~cAboveGroundHeightDlg()
{
}

int32_t cAboveGroundHeightDlg::getSensorOffset_mm() const
{
	if (mpSensorOffset_mm->text().isEmpty())
		return 0;

	return mpSensorOffset_mm->text().toInt();
}

int32_t cAboveGroundHeightDlg::getReferenceHeight_mm() const
{
	if (mpReferenceHeight_mm->text().isEmpty())
		return rfm::INVALID_HEIGHT;

	return mpReferenceHeight_mm->text().toInt();
}

void cAboveGroundHeightDlg::setSensorOffset_mm(int32_t offset_mm)
{
	mpSensorOffset_mm->setText(QString::number(offset_mm));
}

void cAboveGroundHeightDlg::setReferenceHeight_mm(int32_t height_mm)
{
	mpReferenceHeight_mm->setText(QString::number(height_mm));
}

void cAboveGroundHeightDlg::createControls()
{
	mpSensorOffset_mm = new QLineEdit(this);
	mpSensorOffset_mm->setValidator(new QIntValidator(-2000, 3000));
	mpSensorOffset_mm->setText("1000");

	mpReferenceHeight_mm = new QLineEdit(this);
	mpReferenceHeight_mm->setValidator(new QIntValidator(2000, 10000));
}

void cAboveGroundHeightDlg::createLayout()
{
	QLabel* pText = nullptr;
	QGridLayout* pGridLayout = nullptr;

	QVBoxLayout* pMainLayout = new QVBoxLayout();

	pGridLayout = new QGridLayout();
	pGridLayout->setColumnMinimumWidth(4, 10);

	pText = new QLabel("Sensor Offset (mm)");
	pGridLayout->addWidget(pText, 0, 0);
	pGridLayout->addWidget(mpSensorOffset_mm, 0, 1);
	pText = new QLabel("Reference Height for Dolly Droop (mm)");
	pGridLayout->addWidget(pText, 2, 0);
	pGridLayout->addWidget(mpReferenceHeight_mm, 2, 1);

	pMainLayout->addLayout(pGridLayout);

	pMainLayout->addSpacing(10);

	QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
		| QDialogButtonBox::Cancel);

	connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
	connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

	pMainLayout->addWidget(buttonBox);

	setLayout(pMainLayout);
}


/*****************************************************************************
 * Above CANOPY Height Dialog
 ****************************************************************************/
cAboveCanopyHeightDlg::cAboveCanopyHeightDlg()
	: QDialog()
{
	setWindowTitle("Above Canopy Height Parameters");

	setMinimumWidth(350);

	createControls();
	createLayout();
}

cAboveCanopyHeightDlg::~cAboveCanopyHeightDlg()
{
}

int32_t cAboveCanopyHeightDlg::getCanopyHeight_mm() const
{
	if (mpCanopyHeight_mm->text().isEmpty())
		return 0;

	return mpCanopyHeight_mm->text().toInt();
}

int32_t cAboveCanopyHeightDlg::getSensorOffset_mm() const
{
	if (mpSensorOffset_mm->text().isEmpty())
		return 0;

	return mpSensorOffset_mm->text().toInt();
}

int32_t cAboveCanopyHeightDlg::getReferenceHeight_mm() const
{
	if (mpReferenceHeight_mm->text().isEmpty())
		return rfm::INVALID_HEIGHT;

	return mpReferenceHeight_mm->text().toInt();
}

void cAboveCanopyHeightDlg::setCanopyHeight_mm(int32_t offset_mm)
{
	mpCanopyHeight_mm->setText(QString::number(offset_mm));
}

void cAboveCanopyHeightDlg::setSensorOffset_mm(int32_t offset_mm)
{
	mpSensorOffset_mm->setText(QString::number(offset_mm));
}

void cAboveCanopyHeightDlg::setReferenceHeight_mm(int32_t height_mm)
{
	mpReferenceHeight_mm->setText(QString::number(height_mm));
}

void cAboveCanopyHeightDlg::createControls()
{
	mpSensorOffset_mm = new QLineEdit(this);
	mpSensorOffset_mm->setValidator(new QIntValidator(-2000, 3000));
	mpSensorOffset_mm->setText("1000");

	mpCanopyHeight_mm = new QLineEdit(this);
	mpCanopyHeight_mm->setValidator(new QIntValidator(0, 10000));
	mpCanopyHeight_mm->setText("0");

	mpReferenceHeight_mm = new QLineEdit(this);
	mpReferenceHeight_mm->setValidator(new QIntValidator(2000, 10000));
}

void cAboveCanopyHeightDlg::createLayout()
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
	pText = new QLabel("Reference Height for Dolly Droop (mm)");
	pGridLayout->addWidget(pText, 4, 0);
	pGridLayout->addWidget(mpReferenceHeight_mm, 4, 1);

	pMainLayout->addLayout(pGridLayout);

	pMainLayout->addSpacing(10);

	QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
		| QDialogButtonBox::Cancel);

	connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
	connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

	pMainLayout->addWidget(buttonBox);

	setLayout(pMainLayout);
}
