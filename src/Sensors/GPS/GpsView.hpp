
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
    void dockLocationChanged(Qt::DockWidgetArea area);
    void topLevelChanged(bool topLevel);

private:
    void createWidgets();
    void horizontalLayout();
    void verticalLayout();

    void setFloatingSize();
    void setHorzDockSize();
    void setVertDockSize();

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

    QLabel* mpNumSvLabel;
    QLineEdit* mpNumSv;

    QLabel* mpNumBasesLabel;
    QLineEdit* mpNumBases;

    QLabel* mpSolutionTypeLabel;
    QLineEdit* mpSolutionType;
};