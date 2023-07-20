

#include "HySpexVNIR_3000N_PropertyPage.hpp"

#include <QtWidgets>
#include <QLayout>
#include <QComboBox>
#include <QLineEdit>

cHySpexVNIR_3000N_PropertyPage::cHySpexVNIR_3000N_PropertyPage(QWidget* parent)
	: cHySpexCamera_PropertyPage(parent)
{
}

cExperimentState* cHySpexVNIR_3000N_PropertyPage::createState(const std::string& type)
{
	return nullptr;
}

void cHySpexVNIR_3000N_PropertyPage::doLayout()
{
	auto* mainLayout = new QVBoxLayout(this);

	mainLayout->addWidget(getLensLayout());
	mainLayout->addWidget(getAcquisitionLayout());
	mainLayout->addWidget(getBackgroundLayout());

	mainLayout->addWidget(mpButtons);

	setLayout(mainLayout);
}
