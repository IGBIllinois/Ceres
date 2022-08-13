
#pragma once

#include "SsnxTypes.hpp"
#include "SsnxParser.hpp"
#include "GPS/GpsTypes.hpp"

#include <QObject>

class cSsnxModel_file : public QObject, public cSsnxParser
{
    Q_OBJECT

public:
    explicit cSsnxModel_file(QObject* parent = nullptr);
    ~cSsnxModel_file();

signals:
    void updatePVT(double timestamp_s,
        double lat_rad, double lng_rad, double height_m,
        double northSpeed_mps, double eastSpeed_mps, double vertSpeed_mps,
        double groundTrack_deg, gps::eDatum datum);

    void updateUTC(int hour, int min, int sec, int day, int month, int year);

protected:
	void onNewData(const ssnx::PVT_Cartesian_1_t& data) override {};
	void onNewData(const ssnx::PVT_Cartesian_2_t& data) override;
	void onNewData(const ssnx::PVT_Cartesian_2_1_t& data) override;
	void onNewData(const ssnx::PVT_Cartesian_2_2_t& data) override;
	void onNewData(const ssnx::PVT_Geodetic_1_t& data) override;
	void onNewData(const ssnx::PVT_Geodetic_2_t& data) override;
	void onNewData(const ssnx::PVT_Geodetic_2_1_t& data) override;
	void onNewData(const ssnx::PVT_Geodetic_2_2_t& data) override;
	void onNewData(const ssnx::PosCovGeodetic_1_t& data) override {};
	void onNewData(const ssnx::VelCovGeodetic_1_t& data) override {};
	void onNewData(const ssnx::DOP_1_t& data) override {};
	void onNewData(const ssnx::PVT_Residuals_1_t& data) override {};
	void onNewData(const ssnx::RAIMStatistics_1_t& data) override {};
	void onNewData(const ssnx::POS_Projected_1_t& data) override {};
	void onNewData(const ssnx::ReceiverTime_1_t& data) override;
	void onNewData(const ssnx::RtcmDatum_1_t& data) override {};

private:
	double mX_m;
	double mY_m;
	double mZ_m;
	double mVx_mps;
	double mVy_mps;
	double mVz_mps;
	double mCartGroundTrack_deg;

	double mTimestamp_s;
	double mLatitude_rad;
	double mLongitude_rad;
	double mHeight_m;
	double mNorthSpeed_mps;
	double mEastSpeed_mps;
	double mVertSpeed_mps;
	double mGroundTrack_deg;
	gps::eDatum mDatum;
};

