
#include "HySpexSWIR_384_StatusView.hpp"
#include "HySpexSWIR_384_Model.hpp"
#include "QIndicator.hpp"

#include <QLineEdit>
#include <QLabel>
#include <QGroupBox>
#include <QGridLayout>
#include <QFormLayout>

#include <string>


cHySpexSWIR_384_StatusView::cHySpexSWIR_384_StatusView(cHySpexSWIR_384_Model* pModel, QWidget* parent)
:
	cHySpexStatusView(pModel, parent)
{
}

cHySpexSWIR_384_StatusView::~cHySpexSWIR_384_StatusView()
{
}

void cHySpexSWIR_384_StatusView::createWidgets()
{
	cHySpexStatusView::createWidgets();

	mpAvgFramesLabel = new QLabel("Avg Frames", this);
	mpAvgFrames = new QLineEdit(this);
	mpAvgFrames->setReadOnly(true);

	mpFramePeriodLabel = new QLabel("Frame Period (us) :", this);
	mpFramePeriod_us = new QLineEdit(this);
	mpFramePeriod_us->setReadOnly(true);

	mpMinFramePeriodLabel = new QLabel("Min Frame Period (us) :", this);
	mpMinFramePeriod_us = new QLineEdit(this);
	mpMinFramePeriod_us->setReadOnly(true);

	mpIntegrationTimeLabel = new QLabel("Integration Time (us) :", this);
	mpIntegrationTime_us = new QLineEdit(this);
	mpIntegrationTime_us->setReadOnly(true);

	mpMaxIntegrationTimeLabel = new QLabel("Max Integration Time (us) :", this);
	mpMaxIntegrationTime_us = new QLineEdit(this);
	mpMaxIntegrationTime_us->setReadOnly(true);

	mpAmbientTempLabel = new QLabel("Ambient Temp (C) :", this);
	mpAmbientTemp_C = new QLineEdit(this);
	mpAmbientTemp_C->setReadOnly(true);
}

void cHySpexSWIR_384_StatusView::doLayout()
{
	auto* mainLayout = new QVBoxLayout(this);

	mainLayout->addWidget(getSensorStatusBox());

	doStatusLayout(mainLayout);

	QGroupBox* acqBox = new QGroupBox("Acquisition Status");

	auto* acqLayout = new QHBoxLayout(this);

	acqLayout->addWidget(mpAvgFramesLabel);
	acqLayout->addWidget(mpAvgFrames);

	acqLayout->addWidget(mpFramePeriodLabel);
	acqLayout->addWidget(mpFramePeriod_us);

	acqLayout->addWidget(mpMinFramePeriodLabel);
	acqLayout->addWidget(mpMinFramePeriod_us);

	acqLayout->addWidget(mpIntegrationTimeLabel);
	acqLayout->addWidget(mpIntegrationTime_us);

	acqLayout->addWidget(mpMaxIntegrationTimeLabel);
	acqLayout->addWidget(mpMaxIntegrationTime_us);

	acqLayout->addWidget(mpAmbientTempLabel);
	acqLayout->addWidget(mpAmbientTemp_C);

	acqBox->setLayout(acqLayout);
	mainLayout->addWidget(acqBox);

	mainLayout->addStretch();

	setLayout(mainLayout);
}

void cHySpexSWIR_384_StatusView::onAvgFramesChange(std::uint16_t avgFrames)
{
	mpAvgFrames->setText(QString::number(avgFrames));
}

void cHySpexSWIR_384_StatusView::onFramePeriodChange(std::uint32_t period_us)
{
	mpFramePeriod_us->setText(QString::number(period_us));
}

void cHySpexSWIR_384_StatusView::onMinFramePeriodChange(std::uint32_t period_us)
{
	mpMinFramePeriod_us->setText(QString::number(period_us));
}

void cHySpexSWIR_384_StatusView::onIntegrationTimeChange(std::uint32_t time_us)
{
	mpIntegrationTime_us->setText(QString::number(time_us));
}

void cHySpexSWIR_384_StatusView::onMaxIntegrationTimeChange(std::uint32_t time_us)
{
	mpMaxIntegrationTime_us->setText(QString::number(time_us));
}

void cHySpexSWIR_384_StatusView::onAmbientTempChange(double temp_C)
{
	mpAmbientTemp_C->setText(QString::number(temp_C, 'f', 1));
}

void cHySpexSWIR_384_StatusView::onSensorTempChange(double temp_C)
{
	double temp_K = 273.15 + temp_C;
	QString label = QString::number(temp_K, 'f', 1);
	label += " K";
	mpCoolingStatus->setText(label);
}



