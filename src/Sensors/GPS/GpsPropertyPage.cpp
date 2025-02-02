

#include "GpsPropertyPage.hpp"

#include <QtWidgets>
#include <QLayout>
#include <QComboBox>
#include <QLineEdit>
#include <QTimer>

cGpsPropertyPage::cGpsPropertyPage(QWidget* parent)
	: cSensorPropertyPage(parent)
{}

void cGpsPropertyPage::createWidgets()
{
	/** Acquisition Info */
	mpIntegrationTimeLabel = new QLabel("Integration Time (sec) :", this);
	mpIntegrationTime_sec = new QLineEdit(this);

	mpMaxIntegrationTimeLabel = new QLabel("Max Integration Time (sec) :", this);
	mpMaxIntegrationTime_sec = new QLineEdit(this);

	mpDoReference = new QPushButton("Calc Reference");
//	mpDoBackground->setCheckable(true);
//	connect(mpDoBackground, &QPushButton::clicked, this, &cGpsPropertyPage::doCalcBackground);
	connect(mpDoReference, &QPushButton::pressed, this, &cGpsPropertyPage::doCalcReference);
}

QGroupBox* cGpsPropertyPage::getReferenceLayout()
{
	QGroupBox* acqBox = new QGroupBox("Reference Parameters");

	auto* acqLayout = new QFormLayout();

	acqLayout->addRow(mpIntegrationTimeLabel, mpIntegrationTime_sec);
	acqLayout->addRow(mpMaxIntegrationTimeLabel, mpMaxIntegrationTime_sec);

	acqLayout->addWidget(mpDoReference);

	acqBox->setLayout(acqLayout);

	return acqBox;
}

cExperimentState* cGpsPropertyPage::createState(const std::string& type, const nlohmann::json& entry, QObject* parent)
{
	return nullptr;
}
