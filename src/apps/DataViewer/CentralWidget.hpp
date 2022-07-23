
#pragma once

#include "CeresDataFile.hpp"

#include "ExperimentParser.hpp"
#include "SpidercamParser.hpp"
#include "WeatherParser.hpp"

#include <QWidget>

// Qt Forward Declaration
QT_BEGIN_NAMESPACE
class QLabel;
class QLineEdit;
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

    void onExperimentTitle(const std::string& title) override;
    void onExperimentCultivar(const std::string& cultivar) override;
    void onExperimentResearcher(const std::string& researcher) override;
    void onExperimentDoc(const std::string& doc) override;

    void onStartTime(sExperimentTime_t start_time) override;
    void onEndTime(sExperimentTime_t end_time) override;

    void onStartRecordingTimestamp(uint64_t timestamp) override;
    void onEndRecordingTimestamp(uint64_t timestamp) override;
    void onHeartbeatTimestamp(uint64_t timestamp) override;

    void onBeginSensorList() override;
    void onEndOfSensorList() override;
    void onSensorBlockInfo(unsigned int class_id, const std::string& name) override;

    void onUnknownDataID(BLOCK_DATA_ID_t data_id) override;

    //-----------------------------------------------------
    // Spidercam Data
    //-----------------------------------------------------
    void onPosition(const spidercam::sPosition_1_t& pos) override;

    //-----------------------------------------------------
    // Weather Data
    //-----------------------------------------------------
    void onWindSpeed_mps(bool valid, double speed_mps) override;
    void onWindSpeed_knots(bool valid, double speed_knots) override;
    void onWindDirection_deg(bool valid, double dir_deg) override;

private:
    cCeresDataFile mDataFile;

    bool mHasBeginHeader = false;
    bool mHeaderComplete = false;
    bool mHasBeginFooter = false;
    bool mFooterComplete = false;

    QLineEdit*   mpLoadSrcFile = nullptr;
    QPushButton* mpLoadSrcButton = nullptr;
    QPushButton* mpLoadButton = nullptr;
    QPushButton* mpPlayButton = nullptr;


    QGroupBox* mpExperimentInfo = nullptr;
    QLabel*    mpTitleLabel = nullptr;
    QLineEdit* mpTitle = nullptr;
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
    QLabel* mpDollyXLabel = nullptr;
    QLineEdit* mpDollyX_mm = nullptr;
    QLabel* mpDollyYLabel = nullptr;
    QLineEdit* mpDollyY_mm = nullptr;
    QLabel* mpDollyZLabel = nullptr;
    QLineEdit* mpDollyZ_mm = nullptr;
    QLabel* mpDollySpeedLabel = nullptr;
    QLineEdit* mpDollySpeed_mmps = nullptr;

    QGroupBox* mpWeatherInfo = nullptr;
    QLabel* mpWindSpeedLabel = nullptr;
    QLineEdit* mpWindSpeed_mps = nullptr;
    QLabel* mpWindDirectionLabel = nullptr;
    QLineEdit* mpWindDirection_deg = nullptr;
};

