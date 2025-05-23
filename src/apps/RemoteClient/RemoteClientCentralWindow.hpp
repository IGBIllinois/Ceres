
#pragma once

#include <QTabWidget>


// Forward Declarations
class QLogWidget;


class cRemoteClientCentalWindow : public QTabWidget
{
    Q_OBJECT

public:
    explicit cRemoteClientCentalWindow(QWidget* parent = nullptr);
    ~cRemoteClientCentalWindow();

/*
 * Signals handlers from the sensors
 */
public slots:
    void updateSensorName(QString old_name, QString new_name, QString instance);
    void logMessage(uint8_t type, QString device, QString msg);

private:
    QLogWidget* mpLogWindow;
};

