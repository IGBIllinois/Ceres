
#include "ComputeReferenceHeightDlg.hpp"

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


cComputeReferenceHeightDlg::cComputeReferenceHeightDlg(const cRappGroundModel& groundData, const cRappAerialModel& aerialData, QWidget* parent)
:
	QDialog(parent), mGroundData(groundData), mAerialData(aerialData)
{
	setWindowTitle("Compute Reference Height");

	setMinimumWidth(350);

	createControls();
	createLayout();
}

cComputeReferenceHeightDlg::~cComputeReferenceHeightDlg()
{}

int32_t cComputeReferenceHeightDlg::getReferenceHeight_mm() const
{
	if (mpReferenceHeight_mm->text().isEmpty())
		return rfm::INVALID_HEIGHT;

	return mpReferenceHeight_mm->text().toInt();
}

void cComputeReferenceHeightDlg::setReferenceHeight_mm(int32_t ref_height_mm)
{
	mpReferenceHeight_mm->setText(QString::number(ref_height_mm));
}

void cComputeReferenceHeightDlg::setPosition(int32_t x_mm, int32_t y_mm, int32_t z_mm)
{
	mpSpidercamX_mm->setText(QString::number(x_mm));
	mpSpidercamY_mm->setText(QString::number(y_mm));
	mpSpidercamZ_mm->setText(QString::number(z_mm));
	compute();
}

void cComputeReferenceHeightDlg::setGeometricHeight_mm(int32_t geometric_height_mm)
{
	mpGeometricHeight_mm->setText(QString::number(geometric_height_mm));
	compute();
}

void cComputeReferenceHeightDlg::createControls()
{
	mpSpidercamX_mm = new QLineEdit(this);
	mpSpidercamX_mm->setValidator(new QIntValidator(rfb::minMeasurementX_mm(), rfb::maxMeasurementX_mm()));

	mpSpidercamY_mm = new QLineEdit(this);
	mpSpidercamY_mm->setValidator(new QIntValidator(rfb::minMeasurementY_mm(), rfb::maxMeasurementY_mm()));

	mpSpidercamZ_mm = new QLineEdit(this);
	mpSpidercamZ_mm->setValidator(new QIntValidator(-5000, 10000));

	mpGeometricHeight_mm = new QLineEdit(this);
	mpGeometricHeight_mm->setValidator(new QIntValidator(-2000, 3000));
	mpGeometricHeight_mm->setText("1000");

	mpReferenceHeight_mm = new QLabel(this);
	mpReferenceHeight_mm->setText("0");
}

void cComputeReferenceHeightDlg::createLayout()
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

	connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
	connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
	connect(buttonBox->button(QDialogButtonBox::Apply), &QPushButton::clicked, this, &cComputeReferenceHeightDlg::compute);

	pMainLayout->addWidget(buttonBox);

	setLayout(pMainLayout);
}

void cComputeReferenceHeightDlg::compute()
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

	int32_t ground_height_mm = static_cast<int32_t>(mGroundData.getMeshHeight_mm(x_mm, y_mm)) + 1;
	int32_t aerial_height_mm = static_cast<int32_t>(mAerialData.getMeshHeight_mm(x_mm, y_mm));

	if (aerial_height_mm == rfm::INVALID_HEIGHT)
	{
		return;
	}

	auto diff_height_mm = z_mm - (ground_height_mm + geometric_height_mm);

	int32_t reference_height_mm = aerial_height_mm - diff_height_mm;

	mpReferenceHeight_mm->setText(QString::number(reference_height_mm));
}


