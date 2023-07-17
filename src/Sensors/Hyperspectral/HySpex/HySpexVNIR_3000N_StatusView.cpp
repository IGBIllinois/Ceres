
#include "HySpexVNIR_3000N_StatusView.hpp"
#include "HySpexVNIR_3000N_Model.hpp"

#include <QLineEdit>
#include <QLabel>
#include <QGroupBox>
#include <QGridLayout>
#include <QFormLayout>

#include <string>


cHySpexVNIR_3000N_StatusView::cHySpexVNIR_3000N_StatusView(cHySpexVNIR_3000N_Model* pModel, QWidget* parent)
:
    cSensorStatusView(pModel, parent)
{
}

cHySpexVNIR_3000N_StatusView::~cHySpexVNIR_3000N_StatusView()
{
}

void cHySpexVNIR_3000N_StatusView::createWidgets()
{
	cSensorStatusView::createWidgets();


}

void cHySpexVNIR_3000N_StatusView::doLayout()
{
	auto* mainLayout = new QVBoxLayout(this);

	mainLayout->addWidget(getSensorStatusBox());


	mainLayout->addStretch();

	setLayout(mainLayout);
}

void cHySpexVNIR_3000N_StatusView::onAvgFramesChange(std::uint16_t avgFrames)
{}

void cHySpexVNIR_3000N_StatusView::onFramePeriodChange(std::uint32_t period_us)
{}

void cHySpexVNIR_3000N_StatusView::onMinFramePeriodChange(std::uint32_t period_us)
{}

void cHySpexVNIR_3000N_StatusView::onIntegrationTimeChange(std::uint32_t time_us)
{}

void cHySpexVNIR_3000N_StatusView::onMaxIntegrationTimeChange(std::uint32_t time_us)
{}

void cHySpexVNIR_3000N_StatusView::onAmbientTempChange(double temp_C)
{}

void cHySpexVNIR_3000N_StatusView::onSensorTempChange(double temp_C)
{}

