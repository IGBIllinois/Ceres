
#include "HySpexSWIR_384_StatusView.hpp"
#include "HySpexSWIR_384_Model.hpp"

#include <QLineEdit>
#include <QLabel>
#include <QGroupBox>
#include <QGridLayout>
#include <QFormLayout>

#include <string>


cHySpexSWIR_384_StatusView::cHySpexSWIR_384_StatusView(cHySpexSWIR_384_Model* pModel, QWidget* parent)
:
    cSensorStatusView(pModel, parent)
{
}

cHySpexSWIR_384_StatusView::~cHySpexSWIR_384_StatusView()
{
}

void cHySpexSWIR_384_StatusView::createWidgets()
{
	cSensorStatusView::createWidgets();


}

void cHySpexSWIR_384_StatusView::doLayout()
{
	auto* mainLayout = new QVBoxLayout(this);

	mainLayout->addWidget(getSensorStatusBox());


	mainLayout->addStretch();

	setLayout(mainLayout);
}



