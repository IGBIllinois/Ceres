
#pragma once

#include <spidercam_connect/spidercam_types.hpp>

#include <QAbstractScrollArea>
#include <QStatusBar>

// Qt Forward Declaration
QT_BEGIN_NAMESPACE
class QLabel;
class QLineEdit;
QT_END_NAMESPACE


class cSpidercamDollyStatus : public QAbstractScrollArea
{
    Q_OBJECT

public:
    explicit cSpidercamDollyStatus(QWidget* parent = nullptr);
	~cSpidercamDollyStatus();

public slots:
	void updateLimits(spidercam::sWorkingDimensions limits);
	void updatePosition(spidercam::sPosition_1_t pos);
    void updateBatteryLevel(float level_pct);

public slots:
    void dockLocationChanged(Qt::DockWidgetArea area);
    void topLevelChanged(bool topLevel);


private:
    void createWidgets();
    void horizontalLayout();
    void verticalLayout();

    void setFloatingSize();
    void setHorzDockSize();
    void setVertDockSize();

private:
    QLabel* mpX_Label;
    QLineEdit* mpX_m;

    QLabel* mpY_Label;
    QLineEdit* mpY_m;

    QLabel* mpZ_Label;
    QLineEdit* mpZ_m;

    QLabel* mpHeightLabel;
    QLineEdit* mpHeight_m;

    QLabel* mpPanLabel;
    QLineEdit* mpPan_deg;

    QLabel* mpTiltLabel;
    QLineEdit* mpTilt_deg;

    QLabel* mpSpeedLabel;
    QLineEdit* mpSpeed_mps;

    QLabel* mpBatteryLabel;
    QLineEdit* mpBatteryLevel_pct;

private:
    double mMinX_mm = 0;
    double mMaxX_mm = 0;
    double mMinY_mm = 0;
    double mMaxY_mm = 0;
    double mMinHeight_mm = 0;
    double mMaxHeight_mm = 0;
};

class cBatteryStatus : public QStatusBar
{
    Q_OBJECT

public:
    cBatteryStatus(QWidget* parent = nullptr);
    virtual ~cBatteryStatus() = default;

public slots:
    void updateBatteryLevel(float level_pct);

private:
    QLineEdit* mpBatteryLevel_pct;
};

