
#include "ReferencePointDlg.hpp"

#include <QLabel>
#include <QLayout>
#include <QComboBox>
#include <QPushButton>
#include <QGridLayout>
#include <QGroupBox>
#include <QDialogButtonBox>
#include <QLineEdit>
#include <QIntValidator>

#include <algorithm>


cReferencePointDlg::cReferencePointDlg(QWidget* parent)
:
	QDialog(parent)
{
	setWindowTitle("SSNX Command");

	createControls();
	createLayout();
}

cReferencePointDlg::~cReferencePointDlg()
{}

void cReferencePointDlg::createControls()
{
	mpMinIntegrationTime_sec = new QLineEdit(this);
	mpMinIntegrationTime_sec->setValidator(new QDoubleValidator(0, 120, 1));

	mpMaxIntegrationTime_sec = new QLineEdit(this);
	mpMaxIntegrationTime_sec->setValidator(new QDoubleValidator(0, 120, 1));

	mpErrorThreshold_mm = new QLineEdit(this);
	mpErrorThreshold_mm->setValidator(new QIntValidator(1, 5000));
}

void cReferencePointDlg::createLayout()
{
	QLabel* pText = nullptr;

	QVBoxLayout* pMainLayout = new QVBoxLayout();

	QGroupBox* pGroupBox = new QGroupBox(tr("Integration Time"));
	pGroupBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

	QGridLayout* pWaitInfo = new QGridLayout();
	pWaitInfo->setColumnMinimumWidth(2, 10);

	pText = new QLabel("Minimum (sec)");
	pWaitInfo->addWidget(pText, 0, 0);
	pWaitInfo->addWidget(mpMinIntegrationTime_sec, 0, 1);

	pText = new QLabel("Maximum (sec)");
	pWaitInfo->addWidget(pText, 0, 3);
	pWaitInfo->addWidget(mpMaxIntegrationTime_sec, 0, 4);

	pGroupBox->setLayout(pWaitInfo);
	pMainLayout->addWidget(pGroupBox);

	pMainLayout->addSpacing(10);

	QHBoxLayout* pSubLayout = new QHBoxLayout();

	pText = new QLabel("Error Threshold (mm)");
	pSubLayout->addWidget(pText);
	pSubLayout->addSpacing(10);
	pSubLayout->addWidget(mpErrorThreshold_mm);

	pMainLayout->addLayout(pSubLayout);

	pMainLayout->addSpacing(10);

	QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
		| QDialogButtonBox::Cancel);

	connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
	connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

	pMainLayout->addWidget(buttonBox);

	setLayout(pMainLayout);
}


double cReferencePointDlg::minIntegrationTime_sec() const
{
	double min_time = mpMinIntegrationTime_sec->text().toDouble();
	double max_time = mpMaxIntegrationTime_sec->text().toDouble();

	if (max_time < min_time)
		return max_time;

	return min_time;
}

double cReferencePointDlg::maxIntegrationTime_sec() const
{
	double min_time = mpMinIntegrationTime_sec->text().toDouble();
	double max_time = mpMaxIntegrationTime_sec->text().toDouble();

	if (max_time < min_time)
		return min_time;

	return max_time;
}

int cReferencePointDlg::errorThreshold_mm() const
{
	return mpErrorThreshold_mm->text().toInt();
}

void cReferencePointDlg::setMinIntegrationTime_sec(double secs)
{
	mpMinIntegrationTime_sec->setText(QString::number(secs));
}

void cReferencePointDlg::setMaxIntegrationTime_sec(double secs)
{
	mpMaxIntegrationTime_sec->setText(QString::number(secs));
}

void cReferencePointDlg::setErrorThreshold_mm(int threshold_mm)
{
	mpErrorThreshold_mm->setText(QString::number(threshold_mm));
}

