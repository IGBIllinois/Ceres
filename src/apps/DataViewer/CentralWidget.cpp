

#include "CentralWidget.hpp"
#include "SsnxModel_file.hpp"

#include <QtWidgets>
#include <QLayout>
#include <QStackedLayout>
#include <QLineEdit>
#include <QPushButton>

#include <memory>
#include <iostream>


//-----------------------------------------------------------------------------
cCentralWidget::cCentralWidget(QWidget* parent) :
    QWidget(parent), mTimer(this)
{
    initialize();
    connect(&mTimer, &QTimer::timeout, this, &cCentralWidget::updateFrame);
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

    mpPlayButton = new QPushButton(this);
    mpPlayButton->setText("Play");
    mpPlayButton->setEnabled(false);
    connect(mpPlayButton, &QPushButton::pressed, this, &cCentralWidget::playSourceFile);

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

    mpDollyXLabel = new QLabel("X (mm):");
    mpDollyX_mm = new QLineEdit();
    mpDollyX_mm->setReadOnly(true);
    mpDollyYLabel = new QLabel("Y (mm):");
    mpDollyY_mm = new QLineEdit();
    mpDollyY_mm->setReadOnly(true);
    mpDollyZLabel = new QLabel("Z (mm):");
    mpDollyZ_mm = new QLineEdit();
    mpDollyZ_mm->setReadOnly(true);
    mpDollySpeedLabel = new QLabel("Speed (mm/sec):");
    mpDollySpeed_mmps = new QLineEdit();
    mpDollySpeed_mmps->setReadOnly(true);

    mpSpidercamInfo = new QGroupBox("Dolly Position");
    auto* posLayout = new QFormLayout();
    posLayout->addRow(mpDollyXLabel, mpDollyX_mm);
    posLayout->addRow(mpDollyYLabel, mpDollyY_mm);
    posLayout->addRow(mpDollyZLabel, mpDollyZ_mm);
    auto* speedLayout = new QFormLayout();
    speedLayout->addRow(mpDollySpeedLabel, mpDollySpeed_mmps);
    auto* dollyInfoLayout = new QHBoxLayout();
    dollyInfoLayout->addLayout(posLayout);
    dollyInfoLayout->addLayout(speedLayout);
    mpSpidercamInfo->setLayout(dollyInfoLayout);


    mpWindSpeedLabel = new QLabel("Speed (m/s):");
    mpWindSpeed_mps = new QLineEdit();
    mpWindSpeed_mps->setReadOnly(true);
    mpWindDirectionLabel = new QLabel("Direction (deg):");
    mpWindDirection_deg = new QLineEdit();
    mpWindDirection_deg->setReadOnly(true);

    QGroupBox* mpWeatherInfo = new QGroupBox("Wind Data");
    auto* windInfoLayout = new QHBoxLayout();
    windInfoLayout->addWidget(mpWindSpeedLabel);
    windInfoLayout->addWidget(mpWindSpeed_mps);
    windInfoLayout->addWidget(mpWindDirectionLabel);
    windInfoLayout->addWidget(mpWindDirection_deg);
    mpWeatherInfo->setLayout(windInfoLayout);


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
    pMainLayout->addWidget(mpSpidercamInfo);
    pMainLayout->addWidget(mpWeatherInfo);
    
    pMainLayout->addWidget(mpPlayButton);

    setLayout(pMainLayout);

    mDataFile.attach(static_cast<cExperimentParser*>(this));
    mDataFile.attach(static_cast<cSpidercamParser*>(this));
    mDataFile.attach(static_cast<cWeatherParser*>(this));
}

void cCentralWidget::attach(cBlockParser* pParser)
{
    mDataFile.attach(pParser);
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

    if (mDataFile.is_open())
        mDataFile.close();

    if (!mDataFile.open(filename))
    {
        return;
    }

    if (readHeaderData())
        mpPlayButton->setEnabled(true);
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
void cCentralWidget::playSourceFile()
{
    mpPlayButton->setEnabled(false);
    mTimer.start(100);
}


//-----------------------------------------------------------------------------
void cCentralWidget::updateFrame()
{
    auto result = mDataFile.updateData();

    if (!result)
        mTimer.stop();
}

//-----------------------------------------------------------------------------
// Experiment Parser Data
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

void cCentralWidget::onHeartbeatTimestamp(uint64_t timestamp)
{}

void cCentralWidget::onBeginSensorList()
{}

void cCentralWidget::onEndOfSensorList()
{}

void cCentralWidget::onSensorBlockInfo(unsigned int class_id, const std::string& name)
{}

void cCentralWidget::onUnknownDataID(BLOCK_DATA_ID_t data_id)
{
    std::cerr << data_id << "\n";
}

//-----------------------------------------------------------------------------
// Spidercam Parser Data
//-----------------------------------------------------------------------------

void cCentralWidget::onPosition(const spidercam::sPosition_1_t& pos)
{
    mpDollyX_mm->setText(QString::number(pos.X_mm));
    mpDollyY_mm->setText(QString::number(pos.Y_mm));
    mpDollyZ_mm->setText(QString::number(pos.Z_mm));
    mpDollySpeed_mmps->setText(QString::number(pos.speed_mmps));
}

//-----------------------------------------------------------------------------
// Weather Parser Data
//-----------------------------------------------------------------------------

void cCentralWidget::onWindSpeed_mps(bool valid, double speed_mps)
{
    if (valid)
        mpWindSpeed_mps->setText(QString::number(speed_mps, 'g', 1));
    else
        mpWindSpeed_mps->setText("Calm");
}

void cCentralWidget::onWindSpeed_knots(bool valid, double speed_knots)
{}

void cCentralWidget::onWindDirection_deg(bool valid, double dir_deg)
{
    if (valid)
        mpWindDirection_deg->setText(QString::number(dir_deg, 'g', 1));
    else
        mpWindDirection_deg->setText("");
}
