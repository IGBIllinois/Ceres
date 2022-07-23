
#pragma once

#include "GpsView.hpp"
#include "GpsTypes.hpp"

// Qt Forward Declaration
QT_BEGIN_NAMESPACE
QT_END_NAMESPACE


class cSsnxView : public cGpsView
{
public:
    cSsnxView(QWidget* parent = nullptr);
	virtual ~cSsnxView();

public slots:
    void updatePVT(double timestamp_s,
        double lat_rad, double lng_rad, double height_m, 
        double northSpeed_mps, double eastSpeed_mps, double vertSpeed_mps, 
        double groundTrack_deg, gps::eDatum datum);

    void updateUTC(int hour, int min, int sec, int day, int month, int year);
};