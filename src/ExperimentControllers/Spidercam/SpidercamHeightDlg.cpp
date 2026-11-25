
#include "SpidercamHeightDlg.hpp"

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


cSpidercamHeightDlg::cSpidercamHeightDlg(const cRappGroundModel& groundData, QWidget* parent)
:
	QDialog(parent), mGroundData(groundData)
{
	setWindowTitle("Find Spidercam Height");

	setMinimumWidth(350);

	createControls();
	createLayout();
}

cSpidercamHeightDlg::~cSpidercamHeightDlg()
{}

int32_t cSpidercamHeightDlg::getHeight_mm() const
{
	if (mpSpidercamZ_mm->text().isEmpty())
		return rfm::INVALID_HEIGHT;

	return mpSpidercamZ_mm->text().toInt();
}

void cSpidercamHeightDlg::setPosition(int32_t x_mm, int32_t y_mm)
{
	mpSpidercamX_mm->setText(QString::number(x_mm));
	mpSpidercamY_mm->setText(QString::number(y_mm));
	compute();
}

void cSpidercamHeightDlg::setSensorOffset(int32_t offset_mm)
{
	mpSensorOffset_mm->setText(QString::number(offset_mm));
	compute();
}

void cSpidercamHeightDlg::setDesiredHeight_agl(int32_t height_mm)
{
	mpHeight_mm->setText(QString::number(height_mm));
	compute();
}

void cSpidercamHeightDlg::createControls()
{
	mpSpidercamX_mm = new QLineEdit(this);
	mpSpidercamX_mm->setValidator(new QIntValidator(rfb::minMeasurementX_mm(), rfb::maxMeasurementX_mm()));

	mpSpidercamY_mm = new QLineEdit(this);
	mpSpidercamY_mm->setValidator(new QIntValidator(rfb::minMeasurementY_mm(), rfb::maxMeasurementY_mm()));

	mpSensorOffset_mm = new QLineEdit(this);
	mpSensorOffset_mm->setValidator(new QIntValidator(-2000, 3000));
	mpSensorOffset_mm->setText("1000");

	mpHeight_mm = new QLineEdit(this);
	mpHeight_mm->setValidator(new QIntValidator(-2000, 10000));

	mpSpidercamZ_mm = new QLabel(this);
	mpSpidercamZ_mm->setText("0");
}

void cSpidercamHeightDlg::createLayout()
{
	QLabel* pText = nullptr;
	QGridLayout* pGridLayout = nullptr;

	QVBoxLayout* pMainLayout = new QVBoxLayout();

	pGridLayout = new QGridLayout();
	pGridLayout->setColumnMinimumWidth(4, 10);

	pText = new QLabel("X position (mm)");
	pGridLayout->addWidget(pText, 0, 0);
	pGridLayout->addWidget(mpSpidercamX_mm, 0, 1);
	pText = new QLabel("Y position (mm)");
	pGridLayout->addWidget(pText, 0, 3);
	pGridLayout->addWidget(mpSpidercamY_mm, 0, 4);


	pText = new QLabel("Sensor Offset (mm)");
	pGridLayout->addWidget(pText, 2, 0);
	pGridLayout->addWidget(mpSensorOffset_mm, 2, 1);
	pText = new QLabel("Height AGL (mm)");
	pGridLayout->addWidget(pText, 2, 3);
	pGridLayout->addWidget(mpHeight_mm, 2, 4);

	pMainLayout->addLayout(pGridLayout);

	pMainLayout->addSpacing(10);

	QHBoxLayout* pAnswerLayout = new QHBoxLayout();

	pText = new QLabel("Spidercam Height (mm)");
	pAnswerLayout->addWidget(pText);
	pAnswerLayout->addWidget(mpSpidercamZ_mm);

	pMainLayout->addLayout(pAnswerLayout);

	pMainLayout->addSpacing(10);

	QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
		| QDialogButtonBox::Close | QDialogButtonBox::Apply);

	buttonBox->button(QDialogButtonBox::Apply)->setText("Compute");

	connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
	connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
	connect(buttonBox->button(QDialogButtonBox::Apply), &QPushButton::clicked, this, &cSpidercamHeightDlg::compute);

	pMainLayout->addWidget(buttonBox);

	setLayout(pMainLayout);
}

void cSpidercamHeightDlg::compute()
{
	if (mpSpidercamX_mm->text().isEmpty())
		return;

	if (mpSpidercamY_mm->text().isEmpty())
		return;

	if (mpSensorOffset_mm->text().isEmpty())
		return;

	if (mpHeight_mm->text().isEmpty())
		return;

	int32_t x_mm = mpSpidercamX_mm->text().toInt();
	int32_t y_mm = mpSpidercamY_mm->text().toInt();
	int32_t offset_mm = mpSensorOffset_mm->text().toInt();
	int32_t h_mm = mpHeight_mm->text().toInt();

	int32_t ground_height_mm = static_cast<int32_t>(mGroundData.getMeshHeight_mm(x_mm, y_mm)) + 1;

	int32_t z_mm = ground_height_mm + h_mm + offset_mm;

	mpSpidercamZ_mm->setText(QString::number(z_mm));
}


