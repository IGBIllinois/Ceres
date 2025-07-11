
#include "ComputeSensorRangeDlg.hpp"

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


cComputeSensorRangeDlg::cComputeSensorRangeDlg(const cRappGroundModel& groundData, QWidget* parent)
:
	QDialog(parent), mGroundData(groundData)
{
	setWindowTitle("Compute Range");

	setMinimumWidth(350);

	createControls();
	createLayout();
}

cComputeSensorRangeDlg::~cComputeSensorRangeDlg()
{}

int32_t cComputeSensorRangeDlg::getRange_mm() const
{
	if (mpRange_mm->text().isEmpty())
		return rfm::INVALID_HEIGHT;

	return mpRange_mm->text().toInt();
}

void cComputeSensorRangeDlg::setPosition(int32_t x_mm, int32_t y_mm, int32_t z_mm)
{
	mpSpidercamX_mm->setText(QString::number(x_mm));
	mpSpidercamY_mm->setText(QString::number(y_mm));
	mpSpidercamZ_mm->setText(QString::number(z_mm));
	compute();
}

void cComputeSensorRangeDlg::setSensorOffset(int32_t offset_mm)
{
	mpSensorOffset_mm->setText(QString::number(offset_mm));
	compute();
}

void cComputeSensorRangeDlg::createControls()
{
	mpSpidercamX_mm = new QLineEdit(this);
	mpSpidercamX_mm->setValidator(new QIntValidator(rfb::minMeasurementX_mm(), rfb::maxMeasurementX_mm()));

	mpSpidercamY_mm = new QLineEdit(this);
	mpSpidercamY_mm->setValidator(new QIntValidator(rfb::minMeasurementY_mm(), rfb::maxMeasurementY_mm()));

	mpSpidercamZ_mm = new QLineEdit(this);
	mpSpidercamZ_mm->setValidator(new QIntValidator(-5000, 10000));

	mpSensorOffset_mm = new QLineEdit(this);
	mpSensorOffset_mm->setValidator(new QIntValidator(-2000, 3000));
	mpSensorOffset_mm->setText("1000");

	mpRange_mm = new QLabel(this);
	mpRange_mm->setText("0");
}

void cComputeSensorRangeDlg::createLayout()
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


	pText = new QLabel("Sensor Offset (mm)");
	pGridLayout->addWidget(pText, 2, 0);
	pGridLayout->addWidget(mpSensorOffset_mm, 2, 1);

	pMainLayout->addLayout(pGridLayout);

	pMainLayout->addSpacing(10);

	QHBoxLayout* pAnswerLayout = new QHBoxLayout();

	pText = new QLabel("Sensor Range (mm)");
	pAnswerLayout->addWidget(pText);
	pAnswerLayout->addWidget(mpRange_mm);

	pMainLayout->addLayout(pAnswerLayout);

	pMainLayout->addSpacing(10);

	QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
		| QDialogButtonBox::Close);

	connect(buttonBox, &QDialogButtonBox::accepted, this, &cComputeSensorRangeDlg::compute);
	connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

	pMainLayout->addWidget(buttonBox);

	setLayout(pMainLayout);
}

void cComputeSensorRangeDlg::compute()
{
	if (mpSpidercamX_mm->text().isEmpty())
		return;

	if (mpSpidercamY_mm->text().isEmpty())
		return;

	if (mpSpidercamZ_mm->text().isEmpty())
		return;

	if (mpSensorOffset_mm->text().isEmpty())
		return;

	int32_t x_mm = mpSpidercamX_mm->text().toInt();
	int32_t y_mm = mpSpidercamY_mm->text().toInt();
	int32_t z_mm = mpSpidercamZ_mm->text().toInt();
	int32_t offset_mm = mpSensorOffset_mm->text().toInt();

	int32_t ground_height_mm = static_cast<int32_t>(mGroundData.getMeshHeight_mm(x_mm, y_mm)) + 1;

	int32_t range_mm = z_mm - ground_height_mm - offset_mm;

	mpRange_mm->setText(QString::number(range_mm));
}


