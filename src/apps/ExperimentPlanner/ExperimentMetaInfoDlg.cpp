
#include "ExperimentMetaInfoDlg.hpp"

#include <QLabel>
#include <QLayout>
#include <QCheckBox>
#include <QPushButton>
#include <QGridLayout>
#include <QGroupBox>
#include <QDialogButtonBox>
#include <QLineEdit>
#include <QIntValidator>
#include <QPlainTextEdit>

#include <algorithm>


cExperimentMetaInfoDlg::cExperimentMetaInfoDlg(QWidget* parent)
:
	QDialog(parent)
{
	setWindowTitle("Experiment Information");

	createControls();
	createLayout();
}

cExperimentMetaInfoDlg::~cExperimentMetaInfoDlg()
{}

void cExperimentMetaInfoDlg::createControls()
{
	QFont font;
	QFontMetrics fm(font);
	int pixelsHigh = fm.height();

	mpPrincipalInvestigator = new QLineEdit(this);

	mpResearchers = new QPlainTextEdit(this);
	mpResearchers->setMaximumHeight(pixelsHigh * 4);

	mpComments = new QPlainTextEdit(this);
	mpComments->setMaximumHeight(pixelsHigh * 4);

	mpSpecies = new QLineEdit(this);
	mpCultivar = new QLineEdit(this);
	mpEvents = new QLineEdit(this);
	mpConstructName = new QLineEdit(this);
	mpTreatments = new QPlainTextEdit(this);
	mpTreatments->setMaximumHeight(pixelsHigh * 4);

	mpFieldDesign = new QLineEdit(this);

	mpPlantingDate = new QLineEdit(this);
	mpTargetHarvestDate = new QLineEdit(this);

	mpPermitInfo = new QLineEdit(this);
}

void cExperimentMetaInfoDlg::createLayout()
{
	QLabel* pText = nullptr;
	QGroupBox* pGroupBox = nullptr;
	QGridLayout* pGridLayout = nullptr;

	QVBoxLayout* pMainLayout = new QVBoxLayout();

	pGroupBox = new QGroupBox(tr("General Information"));
	pGroupBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

	pGridLayout = new QGridLayout();
	pText = new QLabel("Principal Investigator");
	pGridLayout->addWidget(pText, 0, 0);
	pGridLayout->addWidget(mpPrincipalInvestigator, 0, 1);

	pText = new QLabel("Researcher(s)");
	pGridLayout->addWidget(pText, 1, 0);
	pGridLayout->addWidget(mpResearchers, 1, 1);

	pText = new QLabel("Comment(s)");
	pGridLayout->addWidget(pText, 2, 0);
	pGridLayout->addWidget(mpComments, 2, 1);

	pGroupBox->setLayout(pGridLayout);
	pMainLayout->addWidget(pGroupBox);

	pMainLayout->addSpacing(10);

	pGroupBox = new QGroupBox(tr("Crop Information"));
	pGroupBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

	pGridLayout = new QGridLayout();
	pText = new QLabel("Species");
	pGridLayout->addWidget(pText, 0, 0);
	pGridLayout->addWidget(mpSpecies, 0, 1);

	pText = new QLabel("Cultivar");
	pGridLayout->addWidget(pText, 1, 0);
	pGridLayout->addWidget(mpCultivar, 1, 1);

	pText = new QLabel("Events");
	pGridLayout->addWidget(pText, 2, 0);
	pGridLayout->addWidget(mpEvents, 2, 1);

	pText = new QLabel("Construct Name");
	pGridLayout->addWidget(pText, 3, 0);
	pGridLayout->addWidget(mpConstructName, 3, 1);

	pText = new QLabel("Treatments");
	pGridLayout->addWidget(pText, 4, 0);
	pGridLayout->addWidget(mpTreatments, 4, 1);

	pText = new QLabel("Field Design");
	pGridLayout->addWidget(pText, 5, 0);
	pGridLayout->addWidget(mpFieldDesign, 5, 1);

	pText = new QLabel("Planting Date");
	pGridLayout->addWidget(pText, 6, 0);
	pGridLayout->addWidget(mpPlantingDate, 6, 1);

	pText = new QLabel("Target Harvest Date");
	pGridLayout->addWidget(pText, 7, 0);
	pGridLayout->addWidget(mpTargetHarvestDate, 7, 1);

	pText = new QLabel("Permit Info");
	pGridLayout->addWidget(pText, 8, 0);
	pGridLayout->addWidget(mpPermitInfo, 8, 1);

	pGroupBox->setLayout(pGridLayout);
	pMainLayout->addWidget(pGroupBox);

	pMainLayout->addSpacing(10);

	QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
		| QDialogButtonBox::Cancel | QDialogButtonBox::Apply);

	connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
	connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

	pMainLayout->addWidget(buttonBox);

	setLayout(pMainLayout);
}

void cExperimentMetaInfoDlg::accept()
{
	QDialog::accept();
}

void cExperimentMetaInfoDlg::reject()
{
	QDialog::reject();
}

void cExperimentMetaInfoDlg::apply()
{
}


