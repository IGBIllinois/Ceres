
#pragma once

#include <QStatusBar>

// Qt Forward Declaration
QT_BEGIN_NAMESPACE
class QLineEdit;
QT_END_NAMESPACE


class cWindSpeedAndDirection : public QStatusBar
{
public:
    cWindSpeedAndDirection(QWidget* parent = nullptr);
	virtual ~cWindSpeedAndDirection() = default;

public slots:
    void update(bool valid_wind_speed, double wind_speed_mps, double wind_dir_deg);

private:
    QLineEdit* mpWindSpeed_mph;
    QLineEdit* myWindDirection_deg;
};