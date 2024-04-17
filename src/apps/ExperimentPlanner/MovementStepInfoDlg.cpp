
#include "MovementStepInfoDlg.hpp"

#include <QLabel>
#include <QLayout>
#include <QCheckBox>
#include <QPushButton>
#include <QGridLayout>
#include <QGroupBox>
#include <QDialogButtonBox>
#include <QLineEdit>
#include <QIntValidator>

#include <algorithm>


cMovementStepInfoDlg::cMovementStepInfoDlg(QWidget* parent)
:
	QDialog(parent)
{
	setWindowTitle("Movement Parameters");

	createControls();
	createLayout();
}

cMovementStepInfoDlg::~cMovementStepInfoDlg()
{}

void cMovementStepInfoDlg::createControls()
{
	mpPlotName = new QLineEdit(this);
	mpDescription = new QLineEdit(this);
	mpSpecies = new QLineEdit(this);
	mpCultivar = new QLineEdit(this);

	mpEvent = new QLineEdit(this);
	mpConstructName = new QLineEdit(this);
	mpPotLabel = new QLineEdit(this);
	mpSeedGeneration = new QLineEdit(this);
	mpCopyNumber = new QLineEdit(this);
}

void cMovementStepInfoDlg::createLayout()
{
	QLabel* pText = nullptr;
	QHBoxLayout* pTopLayout = nullptr;

	QVBoxLayout* pMainLayout = new QVBoxLayout();

	QGridLayout* pPlotInfo = new QGridLayout();
	pPlotInfo->setColumnMinimumWidth(2, 10);

	pText = new QLabel("Name");
	pPlotInfo->addWidget(pText, 1, 0);
	pPlotInfo->addWidget(mpPlotName, 1, 1);

	pText = new QLabel("Description");
	pPlotInfo->addWidget(pText, 2, 0);
	pPlotInfo->addWidget(mpDescription, 2, 1);

	pText = new QLabel("Cultivar");
	pPlotInfo->addWidget(pText, 3, 0);
	pPlotInfo->addWidget(mpCultivar, 3, 1);

	pText = new QLabel("Species");
	pPlotInfo->addWidget(pText, 4, 0);
	pPlotInfo->addWidget(mpSpecies, 4, 1);

	pText = new QLabel("Event");
	pPlotInfo->addWidget(pText, 5, 0);
	pPlotInfo->addWidget(mpEvent, 5, 1);

	pText = new QLabel("Construct Name");
	pPlotInfo->addWidget(pText, 6, 0);
	pPlotInfo->addWidget(mpConstructName, 6, 1);

	pText = new QLabel("Pot Label");
	pPlotInfo->addWidget(pText, 7, 0);
	pPlotInfo->addWidget(mpPotLabel, 7, 1);

	pText = new QLabel("Seed Generation");
	pPlotInfo->addWidget(pText, 8, 0);
	pPlotInfo->addWidget(mpSeedGeneration, 8, 1);

	pText = new QLabel("Copy Number");
	pPlotInfo->addWidget(pText, 9, 0);
	pPlotInfo->addWidget(mpCopyNumber, 9, 1);

	pMainLayout->addLayout(pPlotInfo);

	pMainLayout->addSpacing(10);

	QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
		| QDialogButtonBox::Cancel);

	connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
	connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

	pMainLayout->addWidget(buttonBox);

	setLayout(pMainLayout);
}


