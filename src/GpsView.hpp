
#pragma once

#include <QAbstractScrollArea>


// Qt Forward Declaration
QT_BEGIN_NAMESPACE
class QLabel;
class QLineEdit;
QT_END_NAMESPACE


class cGpsView : public QAbstractScrollArea
{
public:
	cGpsView(QWidget* parent = nullptr);
	virtual ~cGpsView();

public slots:
    void updatePVT(double timestamp_s,
        double lat_rad, double lng_rad, double height_m, 
        double northSpeed_mps, double eastSpeed_mps, double vertSpeed_mps, 
        double groundTrack_deg);

    void updateUTC(int hour, int min, int sec, int day, int month, int year);

    void dockLocationChanged(Qt::DockWidgetArea area);
    void topLevelChanged(bool topLevel);

private:
    void createWidgets();
    void horizontalLayout();
    void verticalLayout();

    void setFixedSize();
    void setVariableSize();

protected:
    QLabel*     mpLatitudeLabel;
    QLineEdit*  mpLatitude_deg;

    QLabel* mpLongitudeLabel;
    QLineEdit* mpLongitude_deg;

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
};