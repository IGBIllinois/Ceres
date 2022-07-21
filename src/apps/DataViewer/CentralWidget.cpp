

#include "CentralWidget.hpp"

#include <QtWidgets>
#include <QLayout>
#include <QStackedLayout>
#include <QLineEdit>
#include <QPushButton>

#include <memory>
#include <iostream>

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

    mpTitleLabel = new QLabel("Title:");
    mpTitle = new QLineEdit();
    mpTitle->setReadOnly(true);

    mpCultivarLabel = new QLabel("Cultivar:");
    mpCultivar = new QLineEdit();
    mpCultivar->setReadOnly(true);

    mpResearcherLabel = new QLabel("Researcher:");
    mpResearcher = new QLineEdit();
    mpResearcher->setReadOnly(true);

    mpExperimentInfo = new QGroupBox("Experiment Info");
    auto* expInfoLayout = new QFormLayout();
    expInfoLayout->addRow(mpTitleLabel, mpTitle);
    expInfoLayout->addRow(mpCultivarLabel, mpCultivar);
    expInfoLayout->addRow(mpResearcherLabel, mpResearcher);
    mpExperimentInfo->setLayout(expInfoLayout);

    mpStartTimeLabel = new QLabel("Time:");
    mpStartTime = new QLineEdit();
    mpStartTime->setReadOnly(true);
    mpStartDateLabel = new QLabel("Date:");
    mpStartDate = new QLineEdit();
    mpStartDate->setReadOnly(true);

    mpStartTimeInfo = new QGroupBox("Experiment Start Time");
    auto* startInfoLayout = new QHBoxLayout();
    startInfoLayout->addWidget(mpStartTimeLabel);
    startInfoLayout->addWidget(mpStartTime);
    startInfoLayout->addWidget(mpStartDateLabel);
    startInfoLayout->addWidget(mpStartDate);
    mpStartTimeInfo->setLayout(startInfoLayout);

    mpEndTimeLabel = new QLabel("Time:");
    mpEndTime = new QLineEdit();
    mpEndTime->setReadOnly(true);
    mpEndDateLabel = new QLabel("Date:");
    mpEndDate = new QLineEdit();
    mpEndDate->setReadOnly(true);

    mpEndTimeInfo = new QGroupBox("Experiment End Time");
    auto* endInfoLayout = new QHBoxLayout();
    endInfoLayout->addWidget(mpEndTimeLabel);
    endInfoLayout->addWidget(mpEndTime);
    endInfoLayout->addWidget(mpEndDateLabel);
    endInfoLayout->addWidget(mpEndDate);
    mpEndTimeInfo->setLayout(endInfoLayout);

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

    auto* infoLayout = new QHBoxLayout();
    infoLayout->addWidget(mpExperimentInfo);

    auto* timeInfoLayout = new QVBoxLayout();
    timeInfoLayout->addWidget(mpStartTimeInfo);
    timeInfoLayout->addWidget(mpEndTimeInfo);

    infoLayout->addLayout(timeInfoLayout);

    pMainLayout->addLayout(infoLayout);

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

    if (!mDataFile.open(filename))
    {
        return;
    }

    readHeaderData();
}

bool cCentralWidget::readHeaderData()
{
    while (!mHeaderComplete)
    {
        auto result = mDataFile.updateData();
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

void cCentralWidget::onStartTime(sExperimentTime_t start_time)
{
    if (!mHasBeginHeader)
        mHeaderComplete = true;

/*
    QString date = QString::number(start_time.day);
    date += "/";
    date += QString::number(start_time.month);
    date += "/";
    date += QString::number(start_time.year);
    mpStartDate->setText(date);
*/
    QString date = QString::number(start_time.month);
    date += "/";
    date += QString::number(start_time.day);
    date += "/";
    date += QString::number(start_time.year);
    mpStartDate->setText(date);

    QString time = QString::number(start_time.hour);
    time += ":";
    time += QString::number(start_time.minutes);
    time += ":";
    time += QString::number(start_time.seconds);
    mpStartTime->setText(time);
}

void cCentralWidget::onEndTime(sExperimentTime_t end_time)
{
    QString date = QString::number(end_time.month);
    date += "/";
    date += QString::number(end_time.day);
    date += "/";
    date += QString::number(end_time.year);
    mpEndDate->setText(date);

    QString time = QString::number(end_time.hour);
    time += ":";
    time += QString::number(end_time.minutes);
    time += ":";
    time += QString::number(end_time.seconds);
    mpEndTime->setText(time);
}

void cCentralWidget::onStartRecordingTimestamp(uint64_t timestamp)
{}

void cCentralWidget::onEndRecordingTimestamp(uint64_t timestamp)
{}

void cCentralWidget::onBeginSensorList()
{}

void cCentralWidget::onEndOfSensorList()
{}

void cCentralWidget::onSensorBlockInfo(unsigned int class_id, const std::string& name)
{
    std::cerr << class_id;
}
