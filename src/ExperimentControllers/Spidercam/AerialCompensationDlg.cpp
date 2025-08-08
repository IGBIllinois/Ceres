
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

	mHeight_mm = rfm::INVALID_HEIGHT;

	setMinimumWidth(350);

	createControls();
	createLayout();
}

cAboveGroundHeightDlg::~cAboveGroundHeightDlg()
{
}

int32_t cAboveGroundHeightDlg::getHeight_mm() const
{
	return mHeight_mm;
}

int32_t cAboveGroundHeightDlg::getSensorOffset_mm() const
{
	if (mpSensorOffset_mm->text().isEmpty())
		return 0;;

	return mpSensorOffset_mm->text().toInt();
}

int32_t cAboveGroundHeightDlg::getDesiredHeight_mm() const
{
	if (mpDesiredHeight_mm->text().isEmpty())
		return 0;

	return mpDesiredHeight_mm->text().toInt();
}

int32_t cAboveGroundHeightDlg::getReferenceHeight_mm() const
{
	if (mpReferenceHeight_mm->text().isEmpty())
		return -1; // rfm::INVALID_HEIGHT;

	return mpReferenceHeight_mm->text().toInt();
}

void cAboveGroundHeightDlg::setSensorOffset_mm(int32_t offset_mm)
{
	mpSensorOffset_mm->setText(QString::number(offset_mm));
	compute();
}

void cAboveGroundHeightDlg::setDesiredHeight_mm(int32_t height_mm)
{
	mpDesiredHeight_mm->setText(QString::number(height_mm));
	compute();
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

	mpDesiredHeight_mm = new QLineEdit(this);
	mpDesiredHeight_mm->setValidator(new QIntValidator(-2000, 10000));

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
	pText = new QLabel("Canopy Height (mm)");
	pGridLayout->addWidget(pText, 2, 0);
	pText = new QLabel("Reference Height for Dolly Droop (mm)");
	pGridLayout->addWidget(pText, 4, 0);
	pGridLayout->addWidget(mpReferenceHeight_mm, 4, 1);

	pMainLayout->addLayout(pGridLayout);

	pMainLayout->addSpacing(10);

	QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
		| QDialogButtonBox::Cancel);

	connect(buttonBox, &QDialogButtonBox::accepted, this, &cAboveGroundHeightDlg::accept);
	connect(buttonBox, &QDialogButtonBox::rejected, this, &cAboveGroundHeightDlg::reject);

	pMainLayout->addWidget(buttonBox);

	setLayout(pMainLayout);
}

void cAboveGroundHeightDlg::accept()
{
	if (!compute())
		QDialog::reject();

	QDialog::accept();
}

void cAboveGroundHeightDlg::reject()
{
	QDialog::reject();
}

bool cAboveGroundHeightDlg::compute()
{
	if (mpSensorOffset_mm->text().isEmpty())
		return false;

	if (mpDesiredHeight_mm->text().isEmpty())
		return false;

	int32_t offset_mm = mpSensorOffset_mm->text().toInt();
	int32_t height_mm = mpDesiredHeight_mm->text().toInt();

	mHeight_mm = height_mm + offset_mm;

	return true;
}



/*****************************************************************************
 * Above CANOPY Height Dialog
 ****************************************************************************/

cAboveCanopyHeightDlg::cAboveCanopyHeightDlg()
	: QDialog(), mResult(eRESULT::NONE)
{
	setWindowTitle("Above Canopy Height");

	mpContinue = new QPushButton("Continue", this);
	mpContinue->setDefault(true);

	mpAbort = new QPushButton("Abort", this);;

	connect(mpContinue, &QPushButton::pressed, this, &cAboveCanopyHeightDlg::accept);
	connect(mpAbort, &QPushButton::pressed, this, &cAboveCanopyHeightDlg::reject);

	QVBoxLayout* pMainLayout = new QVBoxLayout();

	pMainLayout->addWidget(new QLabel("Waiting for Continue or Abort button to be selected."));

	QDialogButtonBox* buttonBox = new QDialogButtonBox(this);
	buttonBox->addButton(mpContinue, QDialogButtonBox::AcceptRole);
	buttonBox->addButton(mpAbort, QDialogButtonBox::RejectRole);

	pMainLayout->addWidget(buttonBox);

	setLayout(pMainLayout);
}

cAboveCanopyHeightDlg::~cAboveCanopyHeightDlg()
{
}


cAboveCanopyHeightDlg::eRESULT cAboveCanopyHeightDlg::result()
{
	return mResult;
}

void cAboveCanopyHeightDlg::showDlg()
{
	show();
	raise();
	activateWindow();
}


void cAboveCanopyHeightDlg::accept()
{
	mResult = eRESULT::CONTINUE;
	QDialog::accept();
}

void cAboveCanopyHeightDlg::reject()
{
	mResult = eRESULT::ABORT;
	QDialog::reject();
}

bool cAboveCanopyHeightDlg::compute()
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
