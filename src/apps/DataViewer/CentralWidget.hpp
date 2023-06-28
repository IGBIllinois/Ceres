
#pragma once

#include "CeresDataFile.hpp"

#include <cbdf/ExperimentParser.hpp>
#include <cbdf/SpidercamParser.hpp>
#include <cbdf/WeatherParser.hpp>

#include <QWidget>
#include <QTimer>

// Qt Forward Declaration
QT_BEGIN_NAMESPACE
class QLabel;
class QLineEdit;
class QStackedLayout;
class QPushButton;
class QGroupBox;
QT_END_NAMESPACE


// Forward Declarations


class cCentralWidget : public QWidget, protected cExperimentParser, 
                        protected cSpidercamParser, protected cWeatherParser
{
    Q_OBJECT

public:
    explicit cCentralWidget(QWidget* parent = nullptr);
    ~cCentralWidget();

    void initialize();
    
    void attach(cBlockParser* pParser);

signals:
    void statusMessage(QString msg);
    void errorMessage(QString title, QString msg);

private slots:
    void browseSourceFile();
    void loadSourceFile();
    void playSourceFile();
    void pauseSourceFile();
    void updateFrame();

private:
    bool readHeaderData();

private:
    //-----------------------------------------------------
    // Experiment Data
    //-----------------------------------------------------
    void onBeginHeader() override;
    void onEndOfHeader() override;

    void onBeginFooter() override;
    void onEndOfFooter() override;

    void onTitle(const std::string& title) override;
    void onSpecies(const std::string& species) override;
    void onCultivar(const std::string& cultivar) override;
    void onPrincipalInvestigator(const std::string& investigator) override;
    
    void onBeginResearcherList() override;
    void onEndOfResearcherList() override;
    void onResearcher(const std::string& researcher) override;
    
    void onExperimentDoc(const std::string& doc) override;
    void onPermitInfo(const std::string& permit) override;

    void onBeginTreatmentList() override;
    void onEndOfTreatmentList() override;
    void onTreatment(const std::string& treatment) override;

    void onConstructName(const std::string& name) override;

    void onBeginEventNumberList() override;
    void onEndOfEventNumberList() override;
    void onEventNumber(const std::string& event) override;

    void onFieldDesign(const std::string& design) override;
    void onPlantingDate(std::uint16_t year, std::uint8_t month, std::uint8_t day, std::uint16_t doy) override;
    void onHarvestDate(std::uint16_t year, std::uint8_t month, std::uint8_t day, std::uint16_t doy) override;

    void onBeginCommentList() override;
    void onEndOfCommentList() override;
    void onComment(const std::string& comments) override;

    void onFileDate(std::uint16_t year, std::uint8_t month, std::uint8_t day) override;
    void onFileTime(std::uint8_t hour, std::uint8_t minute, std::uint8_t seconds) override;

    void onDayOfYear(std::uint16_t day_of_year) override;

    void onBeginSensorList() override;
    void onEndOfSensorList() override;
    void onSensorBlockInfo(uint16_t class_id, const std::string& name) override;

    void onStartTime(sExperimentTime_t start_time) override;
    void onEndTime(sExperimentTime_t end_time) override;

    void onStartRecordingTimestamp(uint64_t timestamp_ns) override;
    void onEndRecordingTimestamp(uint64_t timestamp_ns) override;
    void onHeartbeatTimestamp(uint64_t timestamp_ns) override;


    //-----------------------------------------------------
    // Spidercam Data
    //-----------------------------------------------------
    void onPosition(spidercam::sPosition_1_t position) override;

    //-----------------------------------------------------
    // Weather Data
    //-----------------------------------------------------
    void onConfigInfo(const std::string& info) override;
    void onWindData_mps(bool valid, double speed_mps, double dir_deg) override;
    void onWindData_knots(bool valid, double speed_knots, double dir_deg) override;

private:
    cCeresDataFile mDataFile;
    std::streampos mStartOfData = 0;

    bool mHasBeginHeader = false;
    bool mHeaderComplete = false;
    bool mHasBeginFooter = false;
    bool mFooterComplete = false;

    QLineEdit*   mpLoadSrcFile = nullptr;
    QPushButton* mpLoadSrcButton = nullptr;
    QPushButton* mpLoadButton = nullptr;

    QStackedLayout* mpPlayPauseLayout = nullptr;
    QPushButton* mpPlayButton = nullptr;
    QPushButton* mpPauseButton = nullptr;


    QGroupBox* mpExperimentInfo = nullptr;
    QLabel*    mpTitleLabel = nullptr;
    QLineEdit* mpTitle = nullptr;
    QLabel*    mpSpeciesLabel = nullptr;
    QLineEdit* mpSpecies = nullptr;
    QLabel*    mpCultivarLabel = nullptr;
    QLineEdit* mpCultivar = nullptr;
    QLabel*    mpResearcherLabel = nullptr;
    QLineEdit* mpResearcher = nullptr;

    QGroupBox* mpStartTimeInfo = nullptr;
    QLabel*    mpStartTimeLabel = nullptr;
    QLineEdit* mpStartTime = nullptr;
    QLabel*    mpStartDateLabel = nullptr;
    QLineEdit* mpStartDate = nullptr;

    QGroupBox* mpEndTimeInfo = nullptr;
    QLabel*    mpEndTimeLabel = nullptr;
    QLineEdit* mpEndTime = nullptr;
    QLabel*    mpEndDateLabel = nullptr;
    QLineEdit* mpEndDate = nullptr;

    QGroupBox* mpSpidercamInfo = nullptr;
    QLabel*    mpDollyXLabel = nullptr;
    QLineEdit* mpDollyX_mm = nullptr;
    QLabel*    mpDollyYLabel = nullptr;
    QLineEdit* mpDollyY_mm = nullptr;
    QLabel*    mpDollyZLabel = nullptr;
    QLineEdit* mpDollyZ_mm = nullptr;
    QLabel*    mpDollySpeedLabel = nullptr;
    QLineEdit* mpDollySpeed_mmps = nullptr;

    QGroupBox* mpWeatherInfo = nullptr;
    QLabel*    mpWindSpeedLabel = nullptr;
    QLineEdit* mpWindSpeed_mps = nullptr;
    QLabel*    mpWindDirectionLabel = nullptr;
    QLineEdit* mpWindDirection_deg = nullptr;

    QTimer mTimer;

    std::uint64_t mRecordingStartTime_ns = 0;
};

