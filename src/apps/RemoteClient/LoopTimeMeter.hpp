
#pragma once

#include <chrono>

#include <QStatusBar>

// Qt Forward Declaration
QT_BEGIN_NAMESPACE
class QLineEdit;
QT_END_NAMESPACE


class cLoopTimeMeter : public QStatusBar
{
    Q_OBJECT

public:
    explicit cLoopTimeMeter(QWidget* parent = nullptr);
    ~cLoopTimeMeter();

public slots:
    void loopTimeUpdated(float avr_loop_time_ms);

protected:
    QLineEdit* mpAvgLoopTime_ms;
};

