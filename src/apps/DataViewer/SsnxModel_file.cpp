
#include "SsnxModel_file.hpp"

#include "../Sensors/GPS/GpsUtils.hpp"

#include <functional>


using namespace ssnx;

cSsnxModel_file::cSsnxModel_file(QObject* parent)
:
    QObject(parent)
{
}

cSsnxModel_file::~cSsnxModel_file()
{
}

void cSsnxModel_file::onPVT_Cartesian(uint8_t device_id, ssnx::gps::PVT_Cartesian_2_t data)
{
	mX_m = data.X_m;
	mY_m = data.Y_m;
	mZ_m = data.Z_m;
	mVx_mps = data.Vx_mps;
	mVy_mps = data.Vy_mps;
	mVz_mps = data.Vz_mps;
	mDollyGroundTrack_deg = data.GroundTrack_deg;
}

void cSsnxModel_file::onPVT_Geodetic(uint8_t device_id, ssnx::gps::PVT_Geodetic_1_t data)
{
	mTimestamp_s = data.timestamp_s;
	mLatitude_rad = data.Lat_rad;
	mLongitude_rad = data.Lon_rad;
	mHeight_m = data.Alt_m;
	mUndulation_m = data.GeoidHeight_m;
	mNorthSpeed_mps = data.Vn_mps;
	mEastSpeed_mps = data.Ve_mps;
	mVertSpeed_mps = data.Vu_mps;
	mGroundTrack_deg = data.GroundTrack_deg;
	mDatum = ::gps::eDatum::WGS84;

	mNumOfSv = data.NrSV;

	double height_m = mHeight_m - mUndulation_m;

	emit updatePVT(mTimestamp_s, mLatitude_rad, mLongitude_rad, height_m,
		mNorthSpeed_mps, mEastSpeed_mps, mVertSpeed_mps, mGroundTrack_deg, ::gps::to_int(mDatum), mNumOfSv, 0);
}

void cSsnxModel_file::onPVT_Geodetic(uint8_t device_id, ssnx::gps::PVT_Geodetic_2_t data)
{
	mSolutionType = static_cast<::gps::eSolutionType>(data.Mode);

	emit updateSolutionType(::gps::to_int(mSolutionType));

	mTimestamp_s = data.timestamp_s;
	mLatitude_rad = data.Lat_rad;
	mLongitude_rad = data.Lon_rad;
	mHeight_m = data.Height_m;
	mUndulation_m = data.Undulation_m;
	mNorthSpeed_mps = data.Vn_mps;
	mEastSpeed_mps = data.Ve_mps;
	mVertSpeed_mps = data.Vu_mps;
	mGroundTrack_deg = data.GroundTrack_deg;
	mDatum = static_cast<::gps::eDatum>(data.Datum);

	mNumOfSv = data.NrSV;
	mNumOfBases = data.NrBases.value_or(0);

	double height_m = mHeight_m - mUndulation_m;

	emit updatePVT(mTimestamp_s, mLatitude_rad, mLongitude_rad, height_m,
		mNorthSpeed_mps, mEastSpeed_mps, mVertSpeed_mps, mGroundTrack_deg, ::gps::to_int(mDatum), mNumOfSv, mNumOfBases);
}

void cSsnxModel_file::onReceiverTime(uint8_t device_id, ssnx::gps::ReceiverTime_1_t data)
{
	emit updateUTC(data.utcHour, data.utcMinute, data.utcSecond,
                    data.utcDay, data.utcMonth, data.utcYear);
}
