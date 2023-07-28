
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
	cHySpexCamera_StatusView(pModel, parent)
{
}

cHySpexSWIR_384_StatusView::~cHySpexSWIR_384_StatusView()
{
}

void cHySpexSWIR_384_StatusView::createWidgets()
{
	cHySpexCamera_StatusView::createWidgets();
}

void cHySpexSWIR_384_StatusView::doLayout()
{
	auto* mainLayout = new QVBoxLayout(this);

	mainLayout->addWidget(getSensorStatusBox());

	doStatusLayout(mainLayout);


	auto* infoLayout = new QHBoxLayout(this);

	doAcqStatusLayout(infoLayout);
	doLensInfoLayout(infoLayout);

	mainLayout->addLayout(infoLayout);

	mainLayout->addStretch();

	setLayout(mainLayout);
}

void cHySpexSWIR_384_StatusView::onSensorTempChange(double temp_C)
{
	double temp_K = 273.15 + temp_C;
	QString label = QString::number(temp_K, 'f', 1);
	label += " K";
	mpCoolingStatus->setText(label);
}



