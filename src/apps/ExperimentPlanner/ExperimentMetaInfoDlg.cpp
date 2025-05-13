
#include "ExperimentMetaInfoDlg.hpp"

#include <QLabel>
#include <QLayout>
#include <QCheckBox>
#include <QComboBox>
#include <QPushButton>
#include <QGridLayout>
#include <QGroupBox>
#include <QDialogButtonBox>
#include <QLineEdit>
#include <QIntValidator>
#include <QPlainTextEdit>

#include <algorithm>


cExperimentMetaInfoDlg::cExperimentMetaInfoDlg(cExperimentMetaInfo& info, QWidget* parent)
:
	mInfo(info), QDialog(parent)
{
	setWindowTitle("Experiment Information");

	setMinimumWidth(550);

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

	mpExperimentTitle = new QLineEdit(this);
	mpMeasurementTitle = new QLineEdit(this);

	mpPrincipalInvestigator = new QLineEdit(this);
	if (!mInfo.getPrincipalInvestigator().empty())
		mpPrincipalInvestigator->setText(QString::fromStdString(mInfo.getPrincipalInvestigator()));

	mpResearchers = new QPlainTextEdit(this);
	mpResearchers->setMaximumHeight(pixelsHigh * 4);
	if (mInfo.getResearchers().size() > 0)
	{
		QString text;
		auto researchers = mInfo.getResearchers();

		text = QString::fromStdString(researchers.front());

		for (std::size_t i = 1; i < researchers.size(); ++i)
		{
			text += "\n";
			text += QString::fromStdString(researchers[i]);
		}

		mpResearchers->setPlainText(text);
	}

	mpComments = new QPlainTextEdit(this);
	mpComments->setMaximumHeight(pixelsHigh * 4);
	if (mInfo.getComments().size() > 0)
	{
		QString text;
		auto comments = mInfo.getComments();

		text = QString::fromStdString(comments.front());

		for (std::size_t i = 1; i < comments.size(); ++i)
		{
			text += "\n";
			text += QString::fromStdString(comments[i]);
		}

		mpComments->setPlainText(text);
	}

	mpSpecies = new QLineEdit(this);
	if (!mInfo.getSpecies().empty())
		mpSpecies->setText(QString::fromStdString(mInfo.getSpecies()));

	mpCultivar = new QLineEdit(this);
	if (!mInfo.getCultivar().empty())
		mpCultivar->setText(QString::fromStdString(mInfo.getCultivar()));

	mpEvents = new QPlainTextEdit(this);
	mpEvents->setMaximumHeight(pixelsHigh * 4);
	if (mInfo.getEvents().size() > 0)
	{
		QString text;
		auto events = mInfo.getEvents();

		text = QString::fromStdString(events.front());

		for (std::size_t i = 1; i < events.size(); ++i)
		{
			text += "\n ";
			text += QString::fromStdString(events[i]);
		}

		mpEvents->setPlainText(text);
	}

	mpConstructName = new QLineEdit(this);
	if (!mInfo.getConstructName().empty())
		mpConstructName->setText(QString::fromStdString(mInfo.getConstructName()));

	mpTreatments = new QPlainTextEdit(this);
	mpTreatments->setMaximumHeight(pixelsHigh * 4);
	if (mInfo.getTreatments().size() > 0)
	{
		QString text;
		auto treatments = mInfo.getTreatments();

		text = QString::fromStdString(treatments.front());

		for (std::size_t i = 1; i < treatments.size(); ++i)
		{
			text += "\n ";
			text += QString::fromStdString(treatments[i]);
		}

		mpTreatments->setPlainText(text);
	}

	mpFieldDesign = new QLineEdit(this);
	if (!mInfo.getFieldDesign().empty())
		mpFieldDesign->setText(QString::fromStdString(mInfo.getFieldDesign()));

	mpPlantingDay = new QLineEdit(this);
	mpPlantingDay->setValidator(new QIntValidator(1,31));
	mpPlantingDay->setText(QString::fromStdString(mInfo.getPlantingDay()));

	mpPlantingMonth = new QComboBox(this);
	mpPlantingMonth->addItem("April");
	mpPlantingMonth->addItem("May");
	mpPlantingMonth->addItem("June");
	mpPlantingMonth->addItem("July");
	mpPlantingMonth->addItem("August");
	mpPlantingMonth->addItem("Sept");
	mpPlantingMonth->setEditable(false);


	if (!mInfo.getPlantingMonth().empty())
	{
		int item = std::stoi(mInfo.getPlantingMonth()) - 4;
		mpPlantingMonth->setCurrentIndex(item);
	}

	mpPlantingYear = new QComboBox(this);
	mpPlantingYear->addItem("2024");
	mpPlantingYear->addItem("2025");


	mpTargetHarvestDay = new QLineEdit(this);
	mpTargetHarvestDay->setValidator(new QIntValidator(1, 31));
	mpTargetHarvestDay->setText(QString::fromStdString(mInfo.getTargetHarvestDay()));

	mpTargetHarvestMonth = new QComboBox(this);
	mpTargetHarvestMonth->addItem("May");
	mpTargetHarvestMonth->addItem("June");
	mpTargetHarvestMonth->addItem("July");
	mpTargetHarvestMonth->addItem("August");
	mpTargetHarvestMonth->addItem("Sept");
	mpTargetHarvestMonth->addItem("Oct");
	mpTargetHarvestMonth->setEditable(false);

	if (!mInfo.getTargetHarvestMonth().empty())
	{
		int item = std::stoi(mInfo.getTargetHarvestMonth()) - 5;
		mpTargetHarvestMonth->setCurrentIndex(item);
	}

	mpTargetHarvestYear = new QComboBox(this);
	mpTargetHarvestYear->addItem("2024");
	mpTargetHarvestYear->addItem("2025");

	mpPermitInfo = new QLineEdit(this);
	if (!mInfo.getPermitInfo().empty())
		mpPermitInfo->setText(QString::fromStdString(mInfo.getPermitInfo()));
}

void cExperimentMetaInfoDlg::createLayout()
{
	QLabel* pText = nullptr;
	QGroupBox* pGroupBox = nullptr;
	QGridLayout* pGridLayout = nullptr;

	QVBoxLayout* pMainLayout = new QVBoxLayout();

	QHBoxLayout* pTitleLayout = new QHBoxLayout();
	pText = new QLabel("Measurement Title");
	pTitleLayout->addWidget(pText);
	pTitleLayout->addWidget(mpMeasurementTitle, 1);

	pMainLayout->addLayout(pTitleLayout);

	pMainLayout->addSpacing(10);

	pGroupBox = new QGroupBox(tr("General Information"));
	pGroupBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

	pGridLayout = new QGridLayout();
	pText = new QLabel("Experiment Title");
	pGridLayout->addWidget(pText, 0, 0);
	pGridLayout->addWidget(mpExperimentTitle, 0, 1);

	pText = new QLabel("Principal Investigator");
	pGridLayout->addWidget(pText, 1, 0);
	pGridLayout->addWidget(mpPrincipalInvestigator, 1, 1);

	pText = new QLabel("Researcher(s)");
	pGridLayout->addWidget(pText, 2, 0);
	pGridLayout->addWidget(mpResearchers, 2, 1);

	pText = new QLabel("Comment(s)");
	pGridLayout->addWidget(pText, 3, 0);
	pGridLayout->addWidget(mpComments, 3, 1);

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

	QHBoxLayout* pDateLayout = new QHBoxLayout();

	pText = new QLabel("Month");
	pDateLayout->addWidget(pText);
	pDateLayout->addWidget(mpPlantingMonth, 1);

	pText = new QLabel("Day");
	pDateLayout->addWidget(pText);
	pDateLayout->addWidget(mpPlantingDay, 1);

	pText = new QLabel("Year");
	pDateLayout->addWidget(pText);
	pDateLayout->addWidget(mpPlantingYear, 1);

	pGridLayout->addLayout(pDateLayout, 6, 1);

	pText = new QLabel("Target Harvest Date");
	pGridLayout->addWidget(pText, 7, 0);

	pDateLayout = new QHBoxLayout();

	pText = new QLabel("Month");
	pDateLayout->addWidget(pText);
	pDateLayout->addWidget(mpTargetHarvestMonth, 1);

	pText = new QLabel("Day");
	pDateLayout->addWidget(pText);
	pDateLayout->addWidget(mpTargetHarvestDay, 1);

	pText = new QLabel("Year");
	pDateLayout->addWidget(pText);
	pDateLayout->addWidget(mpTargetHarvestYear, 1);

	pGridLayout->addLayout(pDateLayout, 7, 1);

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

std::string cExperimentMetaInfoDlg::getExperimentTitle() const
{
	return mpExperimentTitle->text().toStdString();
}

void cExperimentMetaInfoDlg::setExperimentTitle(const std::string& title)
{
	mpExperimentTitle->setText(QString::fromStdString(title));
}

std::string cExperimentMetaInfoDlg::getMeasurementTitle() const
{
	return mpMeasurementTitle->text().toStdString();
}

void cExperimentMetaInfoDlg::setMeasurementTitle(const std::string& title)
{
	mpMeasurementTitle->setText(QString::fromStdString(title));
}

void cExperimentMetaInfoDlg::accept()
{
	apply();
	QDialog::accept();
}

void cExperimentMetaInfoDlg::apply()
{
	std::string str;
	QString text;
	std::vector<std::string> list;

	str = mpPrincipalInvestigator->text().toStdString();
	mInfo.setPrincipalInvestigator(str);

	text = mpResearchers->toPlainText();
	auto text_list = text.split(QRegExp("[,\n]+"), Qt::SkipEmptyParts);

	list.clear();
	for (auto entry : text_list)
	{
		list.push_back(entry.trimmed().toStdString());
	}
	mInfo.setResearchers(list);

	text = mpComments->toPlainText();
	text_list = text.split(QRegExp("[,\n]+"), Qt::SkipEmptyParts);

	list.clear();
	for (auto entry : text_list)
	{
		list.push_back(entry.trimmed().toStdString());
	}
	mInfo.setComments(list);

	str = mpSpecies->text().toStdString();
	mInfo.setSpecies(str);

	str = mpCultivar->text().toStdString();
	mInfo.setCultivar(str);

	text = mpEvents->toPlainText();
	text_list = text.split(QRegExp("[,\n]+"), Qt::SkipEmptyParts);

	list.clear();
	for (auto entry : text_list)
	{
		list.push_back(entry.trimmed().toStdString());
	}
	mInfo.setEvents(list);

	str = mpConstructName->text().toStdString();
	mInfo.setConstructName(str);

	text = mpTreatments->toPlainText();
	text_list = text.split(QRegExp("[,\n]+"), Qt::SkipEmptyParts);

	list.clear();
	for (auto entry : text_list)
	{
		list.push_back(entry.trimmed().toStdString());
	}
	mInfo.setTreatments(list);

	str = mpFieldDesign->text().toStdString();
	mInfo.setFieldDesign(str);

	int m = mpPlantingMonth->currentIndex() + 4;
	mInfo.setPlantingMonth(std::to_string(m));

	str = mpPlantingDay->text().toStdString();
	mInfo.setPlantingDay(str);
	
	str = mpPlantingYear->currentText().toStdString();
	mInfo.setPlantingYear(str);

	m = mpTargetHarvestMonth->currentIndex() + 5;
	mInfo.setTargetHarvestMonth(std::to_string(m));

	str = mpTargetHarvestDay->text().toStdString();
	mInfo.setTargetHarvestDay(str);

	str = mpTargetHarvestYear->currentText().toStdString();
	mInfo.setTargetHarvestYear(str);

	str = mpPermitInfo->text().toStdString();
	mInfo.setPermitInfo(str);
}


