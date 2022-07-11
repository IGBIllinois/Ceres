
#pragma once

#include <QAbstractScrollArea>
#include "CtrlDataModelRemote.hpp"

// Qt Forward Declaration
QT_BEGIN_NAMESPACE
class QLabel;
class QLineEdit;
class QPushButton;
class QStackedLayout;
QT_END_NAMESPACE


class cRemoteClientView : public QAbstractScrollArea
{
public:
    cRemoteClientView(cCtrlDataModelRemote* pModel, QWidget* parent = nullptr);
	virtual ~cRemoteClientView();

    void setConnectionStatus(const QString& status);
    void enableReconnectButton(bool enable);

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

private slots:
    void tryReconnectPressed();

protected:
    cCtrlDataModelRemote* mpModel;

    QStackedLayout* mpStatusReconnectLayout;

    QLabel*      mpConnectionStatusLabel;
    QLineEdit*   mpConnectionStatus;
    QPushButton* mpTryReconnect;

    QLabel* mpSensorLabel;
    QLineEdit* mpSensorStatus;

/*
    QLabel* mpHeightLabel;
    QLineEdit* mpHeight_m;

    QLabel* mpNorthSpeedLabel;
    QLineEdit* mpNorthVelocity_mps;

    QLabel* mpEastSpeedLabel;
    QLineEdit* mpEastVelocity_mps;

    QLabel* mpVerticalSpeedLabel;
    QLineEdit* mpUpVelocity_mps;

    QLabel* mpGroundTrackLabel;
    QLineEdit* mpGroundTrack_deg;

    QLabel* mpDatumLabel;
    QLineEdit* mpDatum;

    QLabel* mpTimestampLabel;
    QLineEdit* mpTimestamp_s;

    QLabel* mpDateLabel;
    QLineEdit* mpDate;

    QLabel* mpTimeLabel;
    QLineEdit* mpTime;
*/
};