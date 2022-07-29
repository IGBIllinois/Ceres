
#include "SsnxModel_file.hpp"
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

void cSsnxModel_file::onNewData(const ssnx::PVT_Geodetic_1_t& data)
{
	mTimestamp_s = data.timestamp_s;
	mLatitude_rad = data.Lat_rad;
	mLongitude_rad = data.Lon_rad;
	mHeight_m = data.GeoidHeight_m;
	mNorthSpeed_mps = data.Vn_mps;
	mEastSpeed_mps = data.Ve_mps;
	mVertSpeed_mps = data.Vu_mps;
	mGroundTrack_deg = data.GroundTrack_deg;
	mDatum = gps::eDatum::WGS84;

	emit updatePVT(mTimestamp_s, mLatitude_rad, mLongitude_rad, mHeight_m,
		mNorthSpeed_mps, mEastSpeed_mps, mVertSpeed_mps, mGroundTrack_deg, mDatum);
}

void cSsnxModel_file::onNewData(const ssnx::PVT_Geodetic_2_t& data)
{
	mTimestamp_s = data.timestamp_s;
	mLatitude_rad = data.Lat_rad;
	mLongitude_rad = data.Lon_rad;
	mHeight_m = data.Height_m;
	mNorthSpeed_mps = data.Vn_mps;
	mEastSpeed_mps = data.Ve_mps;
	mVertSpeed_mps = data.Vu_mps;
	mGroundTrack_deg = data.GroundTrack_deg;
	mDatum = static_cast<gps::eDatum>(data.Datum);

	emit updatePVT(mTimestamp_s, mLatitude_rad, mLongitude_rad, mHeight_m,
		mNorthSpeed_mps, mEastSpeed_mps, mVertSpeed_mps, mGroundTrack_deg, mDatum);
}

void cSsnxModel_file::onNewData(const ssnx::PVT_Geodetic_2_1_t& data)
{
	mTimestamp_s = data.timestamp_s;
	mLatitude_rad = data.Lat_rad;
	mLongitude_rad = data.Lon_rad;
	mHeight_m = data.Height_m;
	mNorthSpeed_mps = data.Vn_mps;
	mEastSpeed_mps = data.Ve_mps;
	mVertSpeed_mps = data.Vu_mps;
	mGroundTrack_deg = data.GroundTrack_deg;
	mDatum = static_cast<gps::eDatum>(data.Datum);

	emit updatePVT(mTimestamp_s, mLatitude_rad, mLongitude_rad, mHeight_m,
		mNorthSpeed_mps, mEastSpeed_mps, mVertSpeed_mps, mGroundTrack_deg, mDatum);
}

void cSsnxModel_file::onNewData(const ssnx::PVT_Geodetic_2_2_t& data)
{
	mTimestamp_s = data.timestamp_s;
	mLatitude_rad = data.Lat_rad;
	mLongitude_rad = data.Lon_rad;
	mHeight_m = data.Height_m;
	mNorthSpeed_mps = data.Vn_mps;
	mEastSpeed_mps = data.Ve_mps;
	mVertSpeed_mps = data.Vu_mps;
	mGroundTrack_deg = data.GroundTrack_deg;
	mDatum = static_cast<gps::eDatum>(data.Datum);

	emit updatePVT(mTimestamp_s, mLatitude_rad, mLongitude_rad, mHeight_m,
		mNorthSpeed_mps, mEastSpeed_mps, mVertSpeed_mps, mGroundTrack_deg, mDatum);
}

void cSsnxModel_file::onNewData(const ssnx::ReceiverTime_1_t& data)
{
	emit updateUTC(data.utcHour, data.utcMinute, data.utcSecond,
                    data.utcDay, data.utcMonth, data.utcYear);
}
