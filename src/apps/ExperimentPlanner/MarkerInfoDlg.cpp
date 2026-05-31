
#include "MarkerInfoDlg.hpp"

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


cMarkerInfoDlg::cMarkerInfoDlg(QWidget* parent)
:
	QDialog(parent)
{
	setWindowTitle("Marker Parameters");

	createControls();
	createLayout();
}

cMarkerInfoDlg::~cMarkerInfoDlg()
{}

void cMarkerInfoDlg::createControls()
{
	mpType = new QComboBox(this);
	mpType->addItem("Custom");
	mpType->addItem("Start Position");
	mpType->addItem("End Position");

	mpLabel = new QLineEdit(this);
	mpLabel->setEnabled(false);

	connect(mpType, &QComboBox::currentIndexChanged, this, &cMarkerInfoDlg::onTypeChanged);
}

void cMarkerInfoDlg::createLayout()
{
	QLabel* pText = nullptr;

	QVBoxLayout* pMainLayout = new QVBoxLayout();

	QGridLayout* pInfo = new QGridLayout();
	pInfo->setColumnMinimumWidth(2, 10);

	pText = new QLabel("Type");
	pInfo->addWidget(pText, 0, 0);
	pInfo->addWidget(mpType, 0, 1);

	pText = new QLabel("Label");
	pInfo->addWidget(pText, 1, 0);
	pInfo->addWidget(mpLabel, 1, 1);

	pMainLayout->addLayout(pInfo);

	pMainLayout->addSpacing(10);

	QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
		| QDialogButtonBox::Cancel);

	connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
	connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

	pMainLayout->addWidget(buttonBox);

	setLayout(pMainLayout);
}

cMarkerInfoDlg::eType  cMarkerInfoDlg::type() const
{
	return static_cast<eType>(mpType->currentIndex());
}

bool cMarkerInfoDlg::hasLabel() const
{
	return !mpLabel->text().isEmpty();
}

std::string  cMarkerInfoDlg::label() const
{
	return mpLabel->text().toStdString();
}

void cMarkerInfoDlg::setType(eType type)
{
	mpType->setCurrentIndex(static_cast<int>(type));

	mpLabel->setEnabled(type == CUSTOM);
}

void cMarkerInfoDlg::setLabel(const std::string& label)
{
	mpLabel->setText(QString::fromStdString(label));
}

void cMarkerInfoDlg::onTypeChanged(int index)
{
	mpLabel->setEnabled(index == CUSTOM);
}



