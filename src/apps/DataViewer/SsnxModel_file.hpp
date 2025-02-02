
#pragma once

#include "SsnxTypes.hpp"
#include "GPS/GpsTypes.hpp"

#include <cbdf/SsnxParser.hpp>

#include <QObject>

class cSsnxModel_file : public QObject, public cSsnxParser
{
    Q_OBJECT

public:
    explicit cSsnxModel_file(QObject* parent = nullptr);
    ~cSsnxModel_file();

signals:
	void updateSolutionType(int solution_type);

    void updatePVT(double timestamp_s,
        double lat_rad, double lng_rad, double height_m,
        double northSpeed_mps, double eastSpeed_mps, double vertSpeed_mps,
        double groundTrack_deg, int datum, int num_sv, int num_bases);

    void updateUTC(int hour, int min, int sec, int day, int month, int year);

protected:
	void onPVT_Cartesian(ssnx::gps::PVT_Cartesian_1_t data) override {};
	void onPVT_Cartesian(ssnx::gps::PVT_Cartesian_2_t data) override;
	void onPVT_Geodetic(ssnx::gps::PVT_Geodetic_1_t data) override;
	void onPVT_Geodetic(ssnx::gps::PVT_Geodetic_2_t data) override;
	void onPosCovGeodetic(ssnx::gps::PosCovGeodetic_1_t data) override {};
	void onVelCovGeodetic(ssnx::gps::VelCovGeodetic_1_t data) override {};
	void onDOP(ssnx::gps::DOP_1_t data) override {};
	void onPVT_Residuals(ssnx::gps::PVT_Residuals_1_t data) override {};
	void onRAIMStatistics(ssnx::gps::RAIMStatistics_1_t data) override {};
	void onPOS_Local(ssnx::gps::POS_Local_1_t pos) override {};
	void onPOS_Projected(ssnx::gps::POS_Projected_1_t data) override {};
	void onReceiverTime(ssnx::gps::ReceiverTime_1_t data) override;
	void onRtcmDatum(ssnx::gps::RtcmDatum_1_t data) override {};

	void onReferencePoint(double avgLat_rad, double avgLng_rad, double avgHeight_m,
		double stdLat_rad, double stdLng_rad, double stdHeight_m, bool heightComputed) override
	{};

private:
	::gps::eSolutionType mSolutionType = ::gps::eSolutionType::NONE;

	double mX_m = 0.0;
	double mY_m = 0.0;
	double mZ_m = 0.0;
	double mVx_mps = 0.0;
	double mVy_mps = 0.0;
	double mVz_mps = 0.0;
	double mDollyGroundTrack_deg = 0.0;

	double mTimestamp_s = 0.0;
	double mLatitude_rad = 0.0;
	double mLongitude_rad = 0.0;
	double mHeight_m = 0.0;
	double mNorthSpeed_mps = 0.0;
	double mEastSpeed_mps = 0.0;
	double mVertSpeed_mps = 0.0;
	double mGroundTrack_deg = 0.0;
	double mUndulation_m = 0.0;
	::gps::eDatum mDatum = ::gps::eDatum::WGS84;

	int mNumOfSv = 0;
	int mNumOfBases = 0;
};

