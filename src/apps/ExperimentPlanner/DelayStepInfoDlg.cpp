
#include "DelayStepInfoDlg.hpp"

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


cDelayStepInfoDlg::cDelayStepInfoDlg(QWidget* parent)
:
	QDialog(parent)
{
	setWindowTitle("Delay Parameters");

	createControls();
	createLayout();
}

cDelayStepInfoDlg::~cDelayStepInfoDlg()
{}

void cDelayStepInfoDlg::createControls()
{
	mpHours = new QLineEdit(this);
	mpHours->setValidator(new QIntValidator(0, 24));

	mpMinutes = new QLineEdit(this);
	mpMinutes->setValidator(new QIntValidator(0, 59));

	mpSeconds = new QLineEdit(this);
	mpSeconds->setValidator(new QDoubleValidator(0, 59, 2));

	mpRecord = new QCheckBox("Record Data During Wait Period", this);
}

void cDelayStepInfoDlg::createLayout()
{
	QLabel* pText = nullptr;
	QHBoxLayout* pTopLayout = nullptr;

	QVBoxLayout* pMainLayout = new QVBoxLayout();

	QGroupBox* pGroupBox = new QGroupBox(tr("Wait Time"));
	pGroupBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

	QGridLayout* pWaitInfo = new QGridLayout();
	pWaitInfo->setColumnMinimumWidth(2, 10);

	pText = new QLabel("Hours");
	pWaitInfo->addWidget(pText, 0, 0);
	pWaitInfo->addWidget(mpHours, 0, 1);

	pText = new QLabel("Minutes");
	pWaitInfo->addWidget(pText, 0, 3);
	pWaitInfo->addWidget(mpMinutes, 0, 4);

	pText = new QLabel("Seconds");
	pWaitInfo->addWidget(pText, 0, 6);
	pWaitInfo->addWidget(mpSeconds, 0, 7);

	pGroupBox->setLayout(pWaitInfo);
	pMainLayout->addWidget(pGroupBox);

	pMainLayout->addSpacing(10);

	pMainLayout->addWidget(mpRecord);

	pMainLayout->addSpacing(10);

	QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
		| QDialogButtonBox::Cancel);

	connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
	connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

	pMainLayout->addWidget(buttonBox);

	setLayout(pMainLayout);
}

bool cDelayStepInfoDlg::hasHours() const
{
	return !mpHours->text().isEmpty();
}

int  cDelayStepInfoDlg::hours() const
{
	return mpHours->text().toInt();
}

bool cDelayStepInfoDlg::hasMinutes() const
{
	return !mpMinutes->text().isEmpty();
}

int  cDelayStepInfoDlg::minutes() const
{
	return mpMinutes->text().toInt();
}

double cDelayStepInfoDlg::seconds() const
{
	return mpSeconds->text().toDouble();
}

bool cDelayStepInfoDlg::recording() const
{
	return mpRecord->isChecked();
}

void cDelayStepInfoDlg::setHours(int hours)
{
	mpHours->setText(QString::number(hours));
}

void cDelayStepInfoDlg::setMinutes(int minutes)
{
	mpMinutes->setText(QString::number(minutes));
}

void cDelayStepInfoDlg::setSeconds(double secs)
{
	mpSeconds->setText(QString::number(secs));
}

void cDelayStepInfoDlg::setRecording(bool recording)
{
	mpRecord->setChecked(recording);
}




