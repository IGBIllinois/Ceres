
#pragma once

#include <chrono>

#include <QStatusBar>

// Qt Forward Declaration
QT_BEGIN_NAMESPACE
class QLineEdit;
class QPixmap;
class QIcon;
QT_END_NAMESPACE


class cLoopTimeMeter : public QStatusBar
{
    Q_OBJECT

public:
    explicit cLoopTimeMeter(QWidget* parent = nullptr);
    ~cLoopTimeMeter();

public slots:
    void loopHeartbeatUpdated();
    void loopTimeUpdated(float avr_loop_time_ms);
    void loopTerminated();

protected:
    QLineEdit* mpAvgLoopTime_ms;
    QLabel* mpHeartBeat;

    QPixmap* mpRedHeart;
    QPixmap* mpEmptyHeart;
    QPixmap* mpBlackHeart;

    bool mShowRedHeart = false;
};

