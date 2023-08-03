
#pragma once

#include <QStatusBar>

// Qt Forward Declaration
QT_BEGIN_NAMESPACE
class QLineEdit;
QT_END_NAMESPACE


class cWindTempRhPAR_StatusBar : public QStatusBar
{
    Q_OBJECT

public:
    cWindTempRhPAR_StatusBar(QWidget* parent = nullptr);
	virtual ~cWindTempRhPAR_StatusBar() = default;

public slots:
    void updateWindData(bool valid_wind_speed, double wind_speed_mps, double wind_dir_deg);
    void updateTemperature(double temp_C);
    void updateRelativeHumidity(double RH_pct);
    void updatePAR(double par_umole);

private:
    QLineEdit* mpWindSpeed_mph;
    QLineEdit* mpWindDirection_deg;
    QLineEdit* mpTemperature_C;
    QLineEdit* mpRelativeHumidity_pct;
    QLineEdit* mpPAR_umole;
};