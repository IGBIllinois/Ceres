

#include "CentralWidget.hpp"
#include "SsnxModel_file.hpp"

#include <QtWidgets>
#include <QLayout>
#include <QStackedLayout>
#include <QLineEdit>
#include <QPushButton>

#include <memory>


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

    mpPauseButton = new QPushButton(this);
    mpPauseButton->setText("Pause");
    mpPauseButton->setEnabled(true);
    connect(mpPauseButton, &QPushButton::pressed, this, &cCentralWidget::pauseSourceFile);

    mpPlayButton = new QPushButton(this);
    mpPlayButton->setText("Play");
    mpPlayButton->setEnabled(true);
    connect(mpPlayButton, &QPushButton::pressed, this, &cCentralWidget::playSourceFile);

    mpTitleLabel = new QLabel("Title:");
    mpTitle = new QLineEdit();
    mpTitle->setReadOnly(true);

    mpSpeciesLabel = new QLabel("Species:");
    mpSpecies = new QLineEdit();
    mpSpecies->setReadOnly(true);

    mpCultivarLabel = new QLabel("Cultivar:");
    mpCultivar = new QLineEdit();
    mpCultivar->setReadOnly(true);

    mpResearcherLabel = new QLabel("Researcher:");
    mpResearcher = new QLineEdit();
    mpResearcher->setReadOnly(true);

    mpExperimentInfo = new QGroupBox("Experiment Info");
    auto* expInfoLayout = new QFormLayout();
    expInfoLayout->addRow(mpTitleLabel, mpTitle);
    expInfoLayout->addRow(mpSpeciesLabel, mpSpecies);
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
    mpWindDirection_deg->setAlignment(Qt::AlignCenter);

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
    
    mpPlayPauseLayout = new QStackedLayout;
    mpPlayPauseLayout->addWidget(mpPlayButton);
    mpPlayPauseLayout->addWidget(mpPauseButton);
    mpPlayPauseLayout->setEnabled(false);
    mpPlayPauseLayout->setCurrentWidget(mpPlayButton);
    pMainLayout->addLayout(mpPlayPauseLayout);

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

    mpTitle->clear();
    mpCultivar->clear();
    mpResearcher->clear();

    mpStartTime->clear();
    mpStartDate->clear();
    mpEndTime->clear();
    mpEndDate->clear();

    mpDollyX_mm->clear();
    mpDollyY_mm->clear();
    mpDollyZ_mm->clear();
    mpDollySpeed_mmps->clear();

    mpWindSpeed_mps->clear();
    mpWindDirection_deg->clear();

    mHeaderComplete = false;
    mStartOfData = 0;
    mpLoadButton->setEnabled(true);
    mpPlayButton->setText("Play");
    mpPlayPauseLayout->setEnabled(false);

    emit statusMessage("");
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
    {
        mStartOfData = mDataFile.filePosition();

        mpLoadButton->setEnabled(false);
        mpPlayPauseLayout->setEnabled(true);
    }
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
    mpPlayPauseLayout->setCurrentWidget(mpPauseButton);
    mTimer.start(10);
}

//-----------------------------------------------------------------------------
void cCentralWidget::pauseSourceFile()
{
    mTimer.stop();
    mpPlayPauseLayout->setCurrentWidget(mpPlayButton);
}

//-----------------------------------------------------------------------------
void cCentralWidget::updateFrame()
{
    try
    {
        auto result = mDataFile.updateData();

        if (!result)
        {
            mTimer.stop();
            mpPlayButton->setText("Replay");
            mpPlayButton->setEnabled(true);
            mpPlayPauseLayout->setCurrentWidget(mpPlayButton);
            mDataFile.gotoPosition(mStartOfData);
        }
    }
    catch (const std::runtime_error& e)
    {
        if (mDataFile.eof())
        {
            mTimer.stop();
        }
        else
        {
            QString msg = e.what();
            emit statusMessage(msg);
        }
    }
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

void cCentralWidget::onTitle(const std::string& title)
{
    mpTitle->setText(QString::fromStdString(title));
}

void cCentralWidget::onSpecies(const std::string& species)
{
    mpSpecies->setText(QString::fromStdString(species));
}

void cCentralWidget::onCultivar(const std::string& cultivar)
{
    mpCultivar->setText(QString::fromStdString(cultivar));
}

void cCentralWidget::onResearcher(const std::string& researcher)
{
    mpResearcher->setText(QString::fromStdString(researcher));
}

void cCentralWidget::onExperimentDoc(const std::string& doc) {}

void cCentralWidget::onBeginTreatmentList() {}
void cCentralWidget::onEndOfTreatmentList() {}
void cCentralWidget::onTreatment(const std::string& treatment) {}

void cCentralWidget::onFileDate(std::uint16_t year, std::uint8_t month, std::uint8_t day) {};
void cCentralWidget::onFileTime(std::uint8_t hour, std::uint8_t minute, std::uint8_t seconds) {};
void cCentralWidget::onDayOfYear(std::uint16_t day_of_year) {};

void cCentralWidget::onStartTime(sExperimentTime_t start_time)
{
    if (!mHasBeginHeader)
        mHeaderComplete = true;

    QString date = QString::number(start_time.month);
    date += "/";
    if (start_time.day < 10)
        date += "0";
    date += QString::number(start_time.day);
    date += "/";
    date += QString::number(start_time.year);
    mpStartDate->setText(date);

    QString time;
    if (start_time.hour < 10)
        time += "0";
    time += QString::number(start_time.hour);
    time += ":";
    if (start_time.minutes < 10)
        time += "0";
    time += QString::number(start_time.minutes);
    time += ":";
    if (start_time.seconds < 10)
        time += "0";
    time += QString::number(start_time.seconds);

    // Due to a copy/paste error, the end time was written as
    // a second start time.  This checks for that.
    if (mpStartTime->text().isEmpty())
        mpStartTime->setText(time);
}

void cCentralWidget::onEndTime(sExperimentTime_t end_time)
{
    QString date = QString::number(end_time.month);
    date += "/";
    if (end_time.day < 10)
        date += "0";
    date += QString::number(end_time.day);
    date += "/";
    date += QString::number(end_time.year);
    mpEndDate->setText(date);

    QString time;
    if (end_time.hour < 10)
        time += "0";
    time += QString::number(end_time.hour);
    time += ":";
    if (end_time.minutes < 10)
        time += "0";
    time += QString::number(end_time.minutes);
    time += ":";
    if (end_time.seconds < 10)
        time += "0";
    time += QString::number(end_time.seconds);
    mpEndTime->setText(time);
}

void cCentralWidget::onStartRecordingTimestamp(uint64_t timestamp_ns)
{
    mRecordingStartTime_ns = timestamp_ns;
}

void cCentralWidget::onEndRecordingTimestamp(uint64_t timestamp_ns)
{
    std::uint64_t diffTime_ns = timestamp_ns - mRecordingStartTime_ns;
    double diffTime_sec = static_cast<double>(diffTime_ns) / 1000000000.0;

    QString msg = "Total recording time: ";
    msg += QString::number(diffTime_sec);
    emit statusMessage(msg);
}

void cCentralWidget::onHeartbeatTimestamp(uint64_t timestamp_ns)
{
    std::uint64_t diffTime_ns = timestamp_ns - mRecordingStartTime_ns;
    double diffTime_sec = static_cast<double>(diffTime_ns) / 1000000000.0;

    QString msg = "Record time: ";
    msg += QString::number(diffTime_sec);
    emit statusMessage(msg);
}

void cCentralWidget::onBeginSensorList()
{}

void cCentralWidget::onEndOfSensorList()
{}

void cCentralWidget::onSensorBlockInfo(uint16_t class_id, const std::string& name)
{}


//-----------------------------------------------------------------------------
// Spidercam Parser Data
//-----------------------------------------------------------------------------

void cCentralWidget::onPosition(spidercam::sPosition_1_t pos)
{
    mpDollyX_mm->setText(QString::number(pos.X_mm));
    mpDollyY_mm->setText(QString::number(pos.Y_mm));
    mpDollyZ_mm->setText(QString::number(pos.Z_mm));
    mpDollySpeed_mmps->setText(QString::number(pos.speed_mmps));
}

//-----------------------------------------------------------------------------
// Weather Parser Data
//-----------------------------------------------------------------------------

void cCentralWidget::onConfigInfo(const std::string& info) {}

void cCentralWidget::onWindData_mps(bool valid, double speed_mps, double dir_deg)
{
    if (valid)
    {
        mpWindSpeed_mps->setText(QString::number(speed_mps, 'g', 1));

        QString dir = QString::number(static_cast<int>(dir_deg));

        if (dir_deg < 10.0)
            dir.insert(0, "00");
        else if (dir_deg < 100.0)
            dir.insert(0, "0");

        mpWindDirection_deg->setText(dir);
    }
    else
    {
        mpWindSpeed_mps->setText("Calm");
        mpWindDirection_deg->setText("");
    }
}

void cCentralWidget::onWindData_knots(bool valid, double speed_knots, double dir_deg)
{
    if (valid)
    {
        mpWindSpeed_mps->setText(QString::number(speed_knots, 'g', 1));

        QString dir = QString::number(static_cast<int>(dir_deg));

        if (dir_deg < 10.0)
            dir.insert(0, "00");
        else if (dir_deg < 100.0)
            dir.insert(0, "0");

        mpWindDirection_deg->setText(dir);
    }
    else
    {
        mpWindSpeed_mps->setText("Calm");
        mpWindDirection_deg->setText("");
    }
}

