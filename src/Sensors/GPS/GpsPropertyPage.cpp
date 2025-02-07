

#include "GpsPropertyPage.hpp"

#include <QtWidgets>
#include <QLayout>
#include <QGridLayout>
#include <QComboBox>
#include <QLineEdit>
#include <QTimer>

cGpsPropertyPage::cGpsPropertyPage(QWidget* parent)
	: cSensorPropertyPage(parent)
{}

void cGpsPropertyPage::createWidgets()
{
	/** Acquisition Info */
	mpMinIntegrationTimeLabel = new QLabel("Min Integration Time (sec) :", this);
	mpMinIntegrationTime_sec = new QLineEdit(this);

	mpMaxIntegrationTimeLabel = new QLabel("Max Integration Time (sec) :", this);
	mpMaxIntegrationTime_sec = new QLineEdit(this);

	mpErrorThresholdLabel = new QLabel("Error Threshold (mm) :", this);
	mpErrorThreshold_mm = new QLineEdit(this);

	mpDoReference = new QPushButton("Calc Reference");
//	mpDoBackground->setCheckable(true);
//	connect(mpDoBackground, &QPushButton::clicked, this, &cGpsPropertyPage::doCalcBackground);
	connect(mpDoReference, &QPushButton::pressed, this, &cGpsPropertyPage::doCalcReference);

	mpRef_X_mm = new QLineEdit(this);
	mpRef_X_mm->setReadOnly(true);

	mpRef_Y_mm = new QLineEdit(this);
	mpRef_Y_mm->setReadOnly(true);

	mpRef_Z_mm = new QLineEdit(this);
	mpRef_Z_mm->setReadOnly(true);

	mpRef_Error_mm = new QLineEdit(this);
	mpRef_Error_mm->setReadOnly(true);
}

void cGpsPropertyPage::doLayout()
{
	QLabel* text = nullptr;

	QVBoxLayout* pMainLayout = new QVBoxLayout();

	QGridLayout* pGridLayout = new QGridLayout();
	pGridLayout->setColumnMinimumWidth(2, 10);

	pGridLayout->addWidget(mpMinIntegrationTimeLabel, 0, 0);
	pGridLayout->addWidget(mpMinIntegrationTime_sec, 0, 1);

	pGridLayout->addWidget(mpMaxIntegrationTimeLabel, 1, 0);
	pGridLayout->addWidget(mpMaxIntegrationTime_sec, 1, 1);

	pGridLayout->addWidget(mpErrorThresholdLabel, 2, 0);
	pGridLayout->addWidget(mpErrorThreshold_mm, 2, 1);

	pMainLayout->addLayout(pGridLayout);

	pMainLayout->addSpacing(10);
	
	pMainLayout->addWidget(mpDoReference, 1);

	pMainLayout->addSpacing(10);

	QGroupBox* refBox = new QGroupBox("Reference Position");

	auto* pRefLayout = new QHBoxLayout();
	text = new QLabel("X (mm)", this);
	pRefLayout->addWidget(text);
	pRefLayout->addWidget(mpRef_X_mm, 1);
	pRefLayout->addSpacing(10);

	text = new QLabel("Y (mm)", this);
	pRefLayout->addWidget(text);
	pRefLayout->addWidget(mpRef_Y_mm, 1);
	pRefLayout->addSpacing(10);

	text = new QLabel("Z (mm)", this);
	pRefLayout->addWidget(text);
	pRefLayout->addWidget(mpRef_Z_mm, 1);
	pRefLayout->addSpacing(10);

	text = new QLabel("Error (mm)", this);
	pRefLayout->addWidget(text);
	pRefLayout->addWidget(mpRef_Error_mm, 1);

	refBox->setLayout(pRefLayout);

	pMainLayout->addWidget(refBox);

	pMainLayout->addSpacing(10);

	pMainLayout->addWidget(mpButtons);

	setLayout(pMainLayout);
}

QGroupBox* cGpsPropertyPage::getReferenceLayout()
{
	QGroupBox* acqBox = new QGroupBox("Reference Parameters");

	auto* acqLayout = new QFormLayout();

	acqLayout->addRow(mpMinIntegrationTimeLabel, mpMinIntegrationTime_sec);
	acqLayout->addRow(mpMaxIntegrationTimeLabel, mpMaxIntegrationTime_sec);
	acqLayout->addRow(mpErrorThresholdLabel,	 mpErrorThreshold_mm);

	acqLayout->addWidget(mpDoReference);

	acqBox->setLayout(acqLayout);

	return acqBox;
}

cExperimentState* cGpsPropertyPage::createState(const std::string& type, const nlohmann::json& entry, QObject* parent)
{
	return nullptr;
}
