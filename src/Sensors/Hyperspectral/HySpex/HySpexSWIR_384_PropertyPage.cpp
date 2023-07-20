

#include "HySpexSWIR_384_PropertyPage.hpp"

#include <QtWidgets>
#include <QLayout>
#include <QComboBox>
#include <QLineEdit>

cHySpexSWIR_384_PropertyPage::cHySpexSWIR_384_PropertyPage(QWidget* parent)
	: cHySpexCamera_PropertyPage(parent)
{
}

cExperimentState* cHySpexSWIR_384_PropertyPage::createState(const std::string& type)
{
	return nullptr;
}

void cHySpexSWIR_384_PropertyPage::doLayout()
{
	auto* mainLayout = new QVBoxLayout(this);

	mainLayout->addWidget(getLensLayout());
	mainLayout->addWidget(getAcquisitionLayout());
	mainLayout->addWidget(getBackgroundLayout());

	mainLayout->addWidget(mpButtons);

	setLayout(mainLayout);
}
