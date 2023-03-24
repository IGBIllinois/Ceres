
#pragma once

#include <chrono>

#include <QStatusBar>
#include <QTimer>
#include <QSettings>

// Qt Forward Declaration
QT_BEGIN_NAMESPACE
class QLineEdit;
QT_END_NAMESPACE


class cHobbsMeter : public QStatusBar
{
    Q_OBJECT

public:
    explicit cHobbsMeter(QWidget* parent = nullptr);
    ~cHobbsMeter();

signals:

public slots:
    void updateControllerConnection(bool connected);

protected slots:

private slots:
    void updateTime();

private:
    void updateTimeData();

protected:
    float mElapseTime_hr = 0.0;
    float mTotalTime_hr = 0.0;

protected:
    std::chrono::time_point<std::chrono::steady_clock> mStartTime;

    QLineEdit* mpElapseTime_hr;
    QLineEdit* mpTotalTime_hr;

private:
    QTimer      mTimer;
    QSettings   mSettings;
};

