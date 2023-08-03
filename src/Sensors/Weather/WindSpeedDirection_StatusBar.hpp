
#pragma once

#include <QStatusBar>

// Qt Forward Declaration
QT_BEGIN_NAMESPACE
class QLineEdit;
QT_END_NAMESPACE


class cWindSpeedDirection_StatusBar : public QStatusBar
{
    Q_OBJECT

public:
    cWindSpeedDirection_StatusBar(QWidget* parent = nullptr);
	virtual ~cWindSpeedDirection_StatusBar() = default;

public slots:
    void updateWindData(bool valid_wind_speed, double wind_speed_mps, double wind_dir_deg);

private:
    QLineEdit* mpWindSpeed_mph;
    QLineEdit* mpWindDirection_deg;
};