
#pragma once

#include <chrono>

#include <QStatusBar>
#include <QTimer>
#include <QSettings>

// Qt Forward Declaration
QT_BEGIN_NAMESPACE
class QLineEdit;
QT_END_NAMESPACE

namespace experiment
{
    enum class eState : uint8_t;
}


class cHobbsMeter : public QStatusBar
{
    Q_OBJECT

public:
    explicit cHobbsMeter(QWidget* parent = nullptr);
    ~cHobbsMeter();

signals:

public slots:
    void updateControllerConnection(bool connected);
    void onExperimentStateChange(experiment::eState state);

protected slots:

private slots:
    void updateTime();
    void updateExpTime();

private:
    void updateTimeData();

protected:
    bool  mRecordingExperimentTime = false;
    float mExperimentTime_min = 0.0;
    float mElapseTime_hr = 0.0;
    float mTotalTime_hr = 0.0;

protected:
    std::chrono::time_point<std::chrono::steady_clock> mStartTime;
    std::chrono::time_point<std::chrono::steady_clock> mExpStartTime;

    QLineEdit* mpExperimentTime_min;
    QLineEdit* mpElapseTime_hr;
    QLineEdit* mpTotalTime_hr;

private:
    QTimer      mTimer;
    QTimer      mExperimentTimer;
    QSettings   mSettings;
};

