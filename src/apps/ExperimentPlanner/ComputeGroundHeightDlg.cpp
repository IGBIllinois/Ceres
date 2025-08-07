
#include "ComputeGroundHeightDlg.hpp"

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


cComputeGroundHeightDlg::cComputeGroundHeightDlg(const cRappGroundModel& groundData, QWidget* parent)
:
	QDialog(parent), mGroundData(groundData)
{
	setWindowTitle("Compute Ground Height");

	setMinimumWidth(350);

	createControls();
	createLayout();
}

cComputeGroundHeightDlg::~cComputeGroundHeightDlg()
{}

int32_t cComputeGroundHeightDlg::getGroundHeight_mm() const
{
	if (mpGroundHeight_mm->text().isEmpty())
		return rfm::INVALID_HEIGHT;

	return mpGroundHeight_mm->text().toInt();
}

void cComputeGroundHeightDlg::setPosition(int32_t x_mm, int32_t y_mm)
{
	mpSpidercamX_mm->setText(QString::number(x_mm));
	mpSpidercamY_mm->setText(QString::number(y_mm));
	compute();
}

void cComputeGroundHeightDlg::createControls()
{
	mpSpidercamX_mm = new QLineEdit(this);
	mpSpidercamX_mm->setValidator(new QIntValidator(rfb::minMeasurementX_mm(), rfb::maxMeasurementX_mm()));

	mpSpidercamY_mm = new QLineEdit(this);
	mpSpidercamY_mm->setValidator(new QIntValidator(rfb::minMeasurementY_mm(), rfb::maxMeasurementY_mm()));

	mpGroundHeight_mm = new QLabel(this);
	mpGroundHeight_mm->setText("0");
}

void cComputeGroundHeightDlg::createLayout()
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

	pMainLayout->addLayout(pGridLayout);

	pMainLayout->addSpacing(10);

	QHBoxLayout* pAnswerLayout = new QHBoxLayout();

	pText = new QLabel("Ground Height (mm)");
	pAnswerLayout->addWidget(pText);
	pAnswerLayout->addWidget(mpGroundHeight_mm);

	pMainLayout->addLayout(pAnswerLayout);

	pMainLayout->addSpacing(10);

	QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
		| QDialogButtonBox::Close | QDialogButtonBox::Apply);

	buttonBox->button(QDialogButtonBox::Apply)->setText("Compute");

	connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
	connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
	connect(buttonBox->button(QDialogButtonBox::Apply), &QPushButton::clicked, this, &cComputeGroundHeightDlg::compute);

	pMainLayout->addWidget(buttonBox);

	setLayout(pMainLayout);
}

void cComputeGroundHeightDlg::compute()
{
	if (mpSpidercamX_mm->text().isEmpty())
		return;

	if (mpSpidercamY_mm->text().isEmpty())
		return;

	int32_t x_mm = mpSpidercamX_mm->text().toInt();
	int32_t y_mm = mpSpidercamY_mm->text().toInt();

	int32_t ground_height_mm = static_cast<int32_t>(mGroundData.getMeshHeight_mm(x_mm, y_mm)) + 1;

	if (ground_height_mm == rfm::INVALID_HEIGHT)
	{
		mpGroundHeight_mm->setText("INVALID HEIGHT");
		return;
	}

	mpGroundHeight_mm->setText(QString::number(ground_height_mm));
}


