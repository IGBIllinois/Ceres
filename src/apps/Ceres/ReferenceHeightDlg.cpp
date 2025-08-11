
#include "ReferenceHeightDlg.hpp"

#include "RappFieldBoundary.hpp"
#include "RappFieldModel.hpp"

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


cReferenceHeightDlg::cReferenceHeightDlg(QWidget* parent)
:
	QDialog(parent)
{
	setWindowTitle("Compute Reference Height");

	setMinimumWidth(350);

	createControls();
	createLayout();
}

cReferenceHeightDlg::~cReferenceHeightDlg()
{}

int32_t cReferenceHeightDlg::getReferenceHeight_mm() const
{
	if (mpReferenceHeight_mm->text().isEmpty())
		return rfm::INVALID_HEIGHT;

	return mpReferenceHeight_mm->text().toInt();
}

void cReferenceHeightDlg::setReferenceHeight_mm(int32_t ref_height_mm)
{
	mpReferenceHeight_mm->setText(QString::number(ref_height_mm));
}

void cReferenceHeightDlg::setPosition(int32_t x_mm, int32_t y_mm, int32_t z_mm)
{
	mpSpidercamX_mm->setText(QString::number(x_mm));
	mpSpidercamY_mm->setText(QString::number(y_mm));
	mpSpidercamZ_mm->setText(QString::number(z_mm));
	compute();
}

void cReferenceHeightDlg::setGeometricHeight_mm(int32_t geometric_height_mm)
{
	mpGeometricHeight_mm->setText(QString::number(geometric_height_mm));
	compute();
}

void cReferenceHeightDlg::createControls()
{
	mpSpidercamX_mm = new QLineEdit(this);
	mpSpidercamX_mm->setValidator(new QIntValidator(rfb::minMeasurementX_mm(), rfb::maxMeasurementX_mm()));

	mpSpidercamY_mm = new QLineEdit(this);
	mpSpidercamY_mm->setValidator(new QIntValidator(rfb::minMeasurementY_mm(), rfb::maxMeasurementY_mm()));

	mpSpidercamZ_mm = new QLineEdit(this);
	mpSpidercamZ_mm->setValidator(new QIntValidator(-5000, 10000));

	mpGeometricHeight_mm = new QLineEdit(this);
	mpGeometricHeight_mm->setValidator(new QIntValidator(-2000, 3000));

	mpReferenceHeight_mm = new QLabel(this);
}

void cReferenceHeightDlg::createLayout()
{
	QLabel* pText = nullptr;
	QGridLayout* pGridLayout = nullptr;

	QVBoxLayout* pMainLayout = new QVBoxLayout();

	pGridLayout = new QGridLayout();
	pGridLayout->setColumnMinimumWidth(7, 10);

	pText = new QLabel("X position (mm)");
	pGridLayout->addWidget(pText, 0, 0);
	pGridLayout->addWidget(mpSpidercamX_mm, 0, 1);
	pText = new QLabel("Y position (mm)");
	pGridLayout->addWidget(pText, 0, 3);
	pGridLayout->addWidget(mpSpidercamY_mm, 0, 4);
	pText = new QLabel("Z position (mm)");
	pGridLayout->addWidget(pText, 0, 6);
	pGridLayout->addWidget(mpSpidercamZ_mm, 0, 7);


	pText = new QLabel("Geometric Height (mm)");
	pGridLayout->addWidget(pText, 2, 0);
	pGridLayout->addWidget(mpGeometricHeight_mm, 2, 1);

	pMainLayout->addLayout(pGridLayout);

	pMainLayout->addSpacing(10);

	QHBoxLayout* pAnswerLayout = new QHBoxLayout();

	pText = new QLabel("Reference Height (mm)");
	pAnswerLayout->addWidget(pText);
	pAnswerLayout->addWidget(mpReferenceHeight_mm);

	pMainLayout->addLayout(pAnswerLayout);

	pMainLayout->addSpacing(10);

	QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
		| QDialogButtonBox::Close | QDialogButtonBox::Apply);

	buttonBox->button(QDialogButtonBox::Apply)->setText("Compute");

	connect(buttonBox, &QDialogButtonBox::accepted, this, &cReferenceHeightDlg::accept);
	connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
	connect(buttonBox->button(QDialogButtonBox::Apply), &QPushButton::clicked, this, &cReferenceHeightDlg::compute);

	pMainLayout->addWidget(buttonBox);

	setLayout(pMainLayout);
}

void cReferenceHeightDlg::accept()
{
	if (!mpReferenceHeight_mm->text().isEmpty())
	{
		int32_t reference_height_mm = mpReferenceHeight_mm->text().toInt();
		nRFM::set_reference_height_mm(reference_height_mm);
	}

	QDialog::accept();
}

void cReferenceHeightDlg::compute()
{
	if (mpSpidercamX_mm->text().isEmpty())
		return;

	if (mpSpidercamY_mm->text().isEmpty())
		return;

	if (mpSpidercamZ_mm->text().isEmpty())
		return;

	if (mpGeometricHeight_mm->text().isEmpty())
		return;

	int32_t x_mm = mpSpidercamX_mm->text().toInt();
	int32_t y_mm = mpSpidercamY_mm->text().toInt();
	int32_t z_mm = mpSpidercamZ_mm->text().toInt();
	int32_t geometric_height_mm = mpGeometricHeight_mm->text().toInt();

	int32_t reference_height_mm = nRFM::compute_reference_height_mm(geometric_height_mm, x_mm, y_mm, z_mm);

	if (reference_height_mm != rfm::INVALID_HEIGHT)
		mpReferenceHeight_mm->setText(QString::number(reference_height_mm));
}


