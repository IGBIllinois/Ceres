

#include "CentralWidget.hpp"

#include <QtWidgets>
#include <QLayout>
#include <QStackedLayout>
#include <QLineEdit>
#include <QPushButton>

#include <memory>


//-----------------------------------------------------------------------------
cCentralWidget::cCentralWidget(QWidget* parent) :
    QWidget(parent),
    mpLoadSrcFile(nullptr), mpLoadSrcButton(nullptr)
{
    initialize();
}

//-----------------------------------------------------------------------------
cCentralWidget::~cCentralWidget()
{
}

//-----------------------------------------------------------------------------
void cCentralWidget::initialize()
{
    mpLoadSrcFile = new QLineEdit();
    mpLoadSrcFile->setReadOnly(true);
    mpLoadSrcFile->setMinimumWidth(500);

    mpLoadSrcButton = new QPushButton(this);
    mpLoadSrcButton->setText("Browse");
    mpLoadSrcButton->setEnabled(true);
    connect(mpLoadSrcButton, &QPushButton::pressed, this, &cCentralWidget::browseSourceFile);

    mpLoadButton = new QPushButton(this);
    mpLoadButton->setText("Load");
    mpLoadButton->setEnabled(true);
    connect(mpLoadButton, &QPushButton::pressed, this, &cCentralWidget::loadSourceFile);

    mpTitleLabel = new QLabel();
    mpTitleLabel->setText("Title:");
    mpTitle = new QLineEdit();
    mpTitle->setReadOnly(true);

    mpCultivarLabel = new QLabel();
    mpCultivarLabel->setText("Cultivar:");
    mpCultivar = new QLineEdit();
    mpCultivar->setReadOnly(true);

    mpResearcherLabel = new QLabel();
    mpResearcherLabel->setText("Researcher:");
    mpResearcher = new QLineEdit();
    mpResearcher->setReadOnly(true);

    mpExperimentInfo = new QGroupBox("Experiment Info");

    auto* expInfoLayout = new QFormLayout();
    expInfoLayout->addRow(mpTitleLabel, mpTitle);
    expInfoLayout->addRow(mpCultivarLabel, mpCultivar);
    expInfoLayout->addRow(mpResearcherLabel, mpResearcher);

    mpExperimentInfo->setLayout(expInfoLayout);


    // Layout the dialog...

    QVBoxLayout* pMainLayout = new QVBoxLayout();
    pMainLayout->setSizeConstraint(QLayout::SetFixedSize);

    auto* srcLayout = new QHBoxLayout();

    QLabel* pSourceLabel = new QLabel();
    pSourceLabel->setText("Source:");

    srcLayout->addWidget(pSourceLabel);
    srcLayout->addSpacing(24);
    srcLayout->addWidget(mpLoadSrcFile);
    srcLayout->addSpacing(3);
    srcLayout->addWidget(mpLoadSrcButton);

    pMainLayout->addLayout(srcLayout);

    pMainLayout->addWidget(mpLoadButton);

    pMainLayout->addWidget(mpExperimentInfo);

    setLayout(pMainLayout);
}

//-----------------------------------------------------------------------------
void cCentralWidget::browseSourceFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QString(), tr("Ceres data (*.ceres);;All Files (*.*)"));

    if (fileName.isEmpty())
        return;

    mpLoadSrcFile->setText(fileName);
}

//-----------------------------------------------------------------------------
void cCentralWidget::loadSourceFile()
{
    std::string filename = mpLoadSrcFile->text().toStdString();

    if (!open(filename))
    {
        return;
    }

    readHeaderData();
}

bool cCentralWidget::readHeaderData()
{
    while (!mHeaderComplete)
    {
        auto result = updateData();
        if (!result)
        {
            return false;
        }
    }

    return true;
}

//-----------------------------------------------------------------------------
void cCentralWidget::onBeginHeader()
{
    mHasBeginHeader = true;
}

void cCentralWidget::onEndOfHeader()
{
    mHeaderComplete = true;
}

void cCentralWidget::onBeginFooter()
{
    mHasBeginFooter = true;
}

void cCentralWidget::onEndOfFooter()
{
    mFooterComplete = true;
}

void cCentralWidget::onExperimentTitle(const std::string& title)
{
    mpTitle->setText(QString::fromStdString(title));
}

void cCentralWidget::onExperimentCultivar(const std::string& cultivar)
{
    mpCultivar->setText(QString::fromStdString(cultivar));
}

void cCentralWidget::onExperimentResearcher(const std::string& researcher)
{
    mpResearcher->setText(QString::fromStdString(researcher));
}

void cCentralWidget::onExperimentDoc(const std::string& doc)
{

}
