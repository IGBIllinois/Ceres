
#pragma once

#include <QTabWidget>


// Forward Declarations
class cLogWidget;


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
    void updateSensorName(QString old_name, QString new_name);

private:
    cLogWidget* mpLogWindow;
};

