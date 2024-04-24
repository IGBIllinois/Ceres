
#include "CreateExperimentFromGpsDlg.hpp"
#include "GpsFileReader.hpp"
#include "Constants.hpp"

#include "ExperimentSteps.hpp"

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
#include <QTableView>
#include <QStringList>
#include <QStandardItemModel>
#include <QHeaderView>
#include <QMessageBox>


#include <algorithm>
#include <memory>


cCreateExperimentFromGpsDlg::cCreateExperimentFromGpsDlg(cExperimentFile& info, const QString& filename, QWidget* parent)
:
	mInfo(info), QDialog(parent)
{
	setWindowTitle("Create Experiment");

//	mInfo.clearSteps();

	setMinimumWidth(550);

	mpModel = new QStandardItemModel(10, 4, this);
	mpModel->setHeaderData(0, Qt::Horizontal, tr("Name"));
	mpModel->setHeaderData(1, Qt::Horizontal, tr("X (m)"));
	mpModel->setHeaderData(2, Qt::Horizontal, tr("Y (m)"));
	mpModel->setHeaderData(3, Qt::Horizontal, tr("Z (m)"));

	{
		cGpsFileReader reader;
		reader.loadFromFile(filename.toStdString());
		auto points = reader.GetPoints();

		auto n = points.size();

		for (int i = 0; i < n; ++i)
		{
			const auto& pos = points[i];

			mpModel->insertRows(i, 1, QModelIndex());
			mpModel->setData(mpModel->index(i, 0, QModelIndex()), QString::fromStdString(pos.label));
			mpModel->setData(mpModel->index(i, 1, QModelIndex()), pos.x_m);
			mpModel->setData(mpModel->index(i, 2, QModelIndex()), pos.y_m);
			mpModel->setData(mpModel->index(i, 3, QModelIndex()), pos.z_m);
		}
	}

	createControls();
	createLayout();
}

cCreateExperimentFromGpsDlg::~cCreateExperimentFromGpsDlg()
{}

void cCreateExperimentFromGpsDlg::createControls()
{
	mpTitle = new QLineEdit(this);

	mpStartPosition = new QTableView(this);
	mpStartPosition->setModel(mpModel);

	mpStartPosition->verticalHeader()->hide();

	QHeaderView* headerView = mpStartPosition->horizontalHeader();
	headerView->setDefaultAlignment(Qt::AlignHCenter);
	headerView->setStretchLastSection(false);

	mpStartPosition->setHorizontalScrollMode(QAbstractItemView::ScrollPerItem);
	mpStartPosition->setSelectionBehavior(QAbstractItemView::SelectRows);
	mpStartPosition->setSelectionMode(QAbstractItemView::SingleSelection);
	mpStartPosition->setSortingEnabled(false);
	mpStartPosition->setFixedWidth(419);

	mpEndPosition = new QTableView(this);
	mpEndPosition->setModel(mpModel);

	mpEndPosition->verticalHeader()->hide();

	headerView = mpEndPosition->horizontalHeader();
	headerView->setDefaultAlignment(Qt::AlignHCenter);
	headerView->setStretchLastSection(false);

	mpEndPosition->setHorizontalScrollMode(QAbstractItemView::ScrollPerItem);
	mpEndPosition->setSelectionBehavior(QAbstractItemView::SelectRows);
	mpEndPosition->setSelectionMode(QAbstractItemView::SingleSelection);
	mpEndPosition->setSortingEnabled(false);
	mpEndPosition->setFixedWidth(419);


	mpClearPath = new QPushButton("Clear Path", this);
	connect(mpClearPath, &QPushButton::pressed, this, &cCreateExperimentFromGpsDlg::clearPaths);

	mpShowPath = new QPushButton("Show Path", this);
	connect(mpShowPath, &QPushButton::pressed, this, &cCreateExperimentFromGpsDlg::onShowPath);
}

void cCreateExperimentFromGpsDlg::createLayout()
{
	QLabel* pText = nullptr;
	QGroupBox* pGroupBox = nullptr;
	QGridLayout* pGridLayout = nullptr;

	QVBoxLayout* pMainLayout = new QVBoxLayout();

	QHBoxLayout* pTitleLayout = new QHBoxLayout();
	pText = new QLabel("Experiment Title");
	pTitleLayout->addWidget(pText);
	pTitleLayout->addWidget(mpTitle, 1);
	pMainLayout->addLayout(pTitleLayout);

	pMainLayout->addSpacing(10);

	QHBoxLayout* pPosLayout = new QHBoxLayout();
	pPosLayout->addWidget(mpStartPosition);
	pPosLayout->addWidget(mpEndPosition);

	QVBoxLayout* pVSubLayout = new QVBoxLayout();
	pVSubLayout->addWidget(mpClearPath);
	pVSubLayout->addWidget(mpShowPath);
	pPosLayout->addLayout(pVSubLayout);

	pMainLayout->addLayout(pPosLayout);

	pMainLayout->addSpacing(10);

#if 0
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
#endif

	QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
		| QDialogButtonBox::Cancel | QDialogButtonBox::Apply);

	buttonBox->button(QDialogButtonBox::Apply)->setText("Generate");

	connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
	connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
	connect(buttonBox->button(QDialogButtonBox::Apply), &QPushButton::clicked, this, &cCreateExperimentFromGpsDlg::generate);

	pMainLayout->addWidget(buttonBox);

	setLayout(pMainLayout);
}

void cCreateExperimentFromGpsDlg::accept()
{
	QDialog::accept();
}

void cCreateExperimentFromGpsDlg::generate()
{
	std::string str;
	QString text;
	std::vector<std::string> list;

	str = mpTitle->text().toStdString();
	if (str.empty())
	{
		QString msg = "The \"Experiment Title\" can not be blank.";
		QMessageBox msg_box(QMessageBox::Critical, "Invalid Parameter", msg);
		msg_box.exec();
		return;
	}

	mInfo.setExperimentName(str);

	QModelIndex startIndex = mpStartPosition->currentIndex();
	QModelIndex endIndex = mpEndPosition->currentIndex();

	if ((startIndex.row() < 0) || (endIndex.row() < 0))
	{
		QString msg = "Please select start and end positions.";
		QMessageBox msg_box(QMessageBox::Critical, "Invalid Parameter", msg);
		msg_box.exec();
		return;
	}

	mInfo.clearSteps();

	// Add preamble...
	std::unique_ptr<cExperimentStep> step = std::make_unique<cExperimentStep_Movement>();

}

void cCreateExperimentFromGpsDlg::onShowPath()
{
	QModelIndex startIndex = mpStartPosition->currentIndex();
	if (startIndex.row() < 0)
		return;

	QModelIndex endIndex   = mpEndPosition->currentIndex();
	if (endIndex.row() < 0)
		return;

	auto x1 = mpModel->data(startIndex.siblingAtColumn(1)).toFloat();
	auto y1 = mpModel->data(startIndex.siblingAtColumn(2)).toFloat();

	auto x2 = mpModel->data(endIndex.siblingAtColumn(1)).toFloat();
	auto y2 = mpModel->data(endIndex.siblingAtColumn(2)).toFloat();

	int x1_mm = static_cast<int>(x1 * nConstants::M_TO_MM);
	int y1_mm = static_cast<int>(y1 * nConstants::M_TO_MM);
	int x2_mm = static_cast<int>(x2 * nConstants::M_TO_MM);
	int y2_mm = static_cast<int>(y2 * nConstants::M_TO_MM);

	emit drawPath(x1_mm, y1_mm, x2_mm, y2_mm);
}
