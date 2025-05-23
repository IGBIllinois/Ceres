
#pragma once

#include <QAbstractScrollArea>
#include "CtrlDataModelRemote.hpp"

#include <vector>

// Qt Forward Declaration
QT_BEGIN_NAMESPACE
class QLabel;
class QLineEdit;
class QTextEdit;
class QPushButton;
class QStackedLayout;
class QGroupBox;
QT_END_NAMESPACE


class cRemoteClientView : public QAbstractScrollArea
{
public:
    cRemoteClientView(cCtrlDataModelRemote* pModel, QWidget* parent = nullptr);
	virtual ~cRemoteClientView();

    void setConnectionStatus(const QString& status);
    void enableReconnectButton(bool enable);

    void updateSensorStatus(const QString& sensor, const QString& status);
    void updateSensorStatus(const QString& sensor, const QString& instance, const QString& status);

    void sensorNameChange(const QString& old_name, const QString& new_name);
    void sensorNameChange(const QString& old_name, const QString& new_name, const QString& instance);

    void removeAllSensors();

    void updateStatusMsg(const QString& msg);
    void updateLogMsg(int msg_type, const QString& device, const QString& msg);

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

    QLabel*      mpConnectionStatusLabel;
    QLineEdit*   mpConnectionStatus;
    QPushButton* mpTryReconnect;

    struct sSensorStatus_t
    {
        QLabel* mpSensorLabel;
        QLineEdit* mpSensorStatus;
    };

    std::vector<sSensorStatus_t> mSensorStatus;

    QGroupBox* mpStatusMessageBox;
    QTextEdit* mpStatusMessage;

    QGroupBox* mpLogMessageBox;
    QLabel*    mpLogTypeLabel;
    QLineEdit* mpLogType;
    QLabel*    mpLogDeviceLabel;
    QLineEdit* mpLogDevice;
    QTextEdit* mpLogMessage;
};