
#include "RappFieldBoundary.hpp"

#include "Constants.hpp"

#include <array>


namespace
{
	uint32_t measurementOffset_mm = 10000;

	constexpr double Re_lat_m = 6361721.0810512137;
	constexpr double Re_lat_ft = Re_lat_m * nConstants::M_TO_FT;

	constexpr double Re_lng_m = 4888165.4030188089;
	constexpr double Re_lng_ft = Re_lng_m * nConstants::M_TO_FT;

	const std::array<rfm::planePoint_t, 4> mUIUC_TowerLocations = {
		rfm::planePoint_t(1237093.164, 1019287.146, 710.708),
		rfm::planePoint_t(1236468.119, 1019286.889, 710.774),
		rfm::planePoint_t(1236468.156, 1019912.027, 710.853),
		rfm::planePoint_t(1237093.252, 1019911.889, 710.752) };

	constexpr double mUIUC_ReferenceHeight_ft = 708.045;
	constexpr double mUIUC_ReferenceHeight_m = mUIUC_ReferenceHeight_ft * nConstants::FT_TO_M;

	const std::array<rfm::rappPoint_t, 4> mRappTowerLocations = {
		rfm::rappPoint_t(0, 
			0, 
			static_cast<std::int32_t>((mUIUC_TowerLocations[0].elevation_ft - mUIUC_ReferenceHeight_ft) * nConstants::FT_TO_MM)),

		rfm::rappPoint_t(static_cast<std::int32_t>((mUIUC_TowerLocations[0].northing_ft - mUIUC_TowerLocations[1].northing_ft) * nConstants::FT_TO_MM),
			static_cast<std::int32_t>((mUIUC_TowerLocations[1].easting_ft - mUIUC_TowerLocations[0].easting_ft) * nConstants::FT_TO_MM),
			static_cast<std::int32_t>((mUIUC_TowerLocations[1].elevation_ft - mUIUC_ReferenceHeight_ft) * nConstants::FT_TO_MM)),

		rfm::rappPoint_t(static_cast<std::int32_t>((mUIUC_TowerLocations[0].northing_ft - mUIUC_TowerLocations[2].northing_ft) * nConstants::FT_TO_MM),
			static_cast<std::int32_t>((mUIUC_TowerLocations[2].easting_ft - mUIUC_TowerLocations[0].easting_ft) * nConstants::FT_TO_MM),
			static_cast<std::int32_t>((mUIUC_TowerLocations[2].elevation_ft - mUIUC_ReferenceHeight_ft) * nConstants::FT_TO_MM)),

		rfm::rappPoint_t(static_cast<std::int32_t>((mUIUC_TowerLocations[0].northing_ft - mUIUC_TowerLocations[3].northing_ft) * nConstants::FT_TO_MM),
			static_cast<std::int32_t>((mUIUC_TowerLocations[3].easting_ft - mUIUC_TowerLocations[0].easting_ft) * nConstants::FT_TO_MM),
			static_cast<std::int32_t>((mUIUC_TowerLocations[3].elevation_ft - mUIUC_ReferenceHeight_ft) * nConstants::FT_TO_MM)) };

	/*
	* 40.0635686 deg, -88.2081615 deg
	* 40.0618528 deg, -88.2081656 deg
	* 40.0618504 deg, -88.2059322 deg
	* 40.0635664 deg, -88.2059295 deg
	* 
	*/
	/*
	 * Tower 1 : lat = 40.0635686 deg, lng = -88.2081615 deg
	 * Tower 2 : lat = 40.0618528 deg, lng = -88.2081656 deg
	 * Tower 3 : lat = 40.0618504 deg, lng = -88.2059322 deg
	 * Tower 4 : lat = 40.0635664 deg, lng = -88.2059295 deg
	 */

	struct sLatLon_WGS84_t
	{
		double mLatitude_rad = 0.0;
		double mLongitude_rad = 0.0;

		sLatLon_WGS84_t(double lat, double lng) : mLatitude_rad(lat), mLongitude_rad(lng)
		{}
	};

	const std::array<sLatLon_WGS84_t, 4> mRappTowerLocations_WGS84 = {
		sLatLon_WGS84_t(40.0635686 * nConstants::DEG_TO_RAD, -88.2081615 * nConstants::DEG_TO_RAD),
		sLatLon_WGS84_t(40.0618528 * nConstants::DEG_TO_RAD, -88.2081656 * nConstants::DEG_TO_RAD),
		sLatLon_WGS84_t(40.0618504 * nConstants::DEG_TO_RAD, -88.2059322 * nConstants::DEG_TO_RAD),
		sLatLon_WGS84_t(40.0635664 * nConstants::DEG_TO_RAD, -88.2059295 * nConstants::DEG_TO_RAD)
	};

}

uint32_t rfb::getMeasureOffset_mm()
{
	return measurementOffset_mm;
}

void rfb::setMeasureOffset_mm(uint32_t measure_offset_mm)
{
	measurementOffset_mm = measure_offset_mm;
}

uint32_t rfb::minX_mm()
{
	return mRappTowerLocations[0].x_mm;
}

uint32_t rfb::minY_mm()
{
	return mRappTowerLocations[0].y_mm;
}

uint32_t rfb::minZ_mm()
{
	return 0;
}

uint32_t rfb::maxX_mm()
{
	return mRappTowerLocations[2].x_mm;
}

uint32_t rfb::maxY_mm()
{
	return mRappTowerLocations[2].y_mm;
}

uint32_t rfb::maxZ_mm()
{
	return 10000;
}


uint32_t rfb::minMeasurementX_mm()
{
	return mRappTowerLocations[0].x_mm + measurementOffset_mm;
}

uint32_t rfb::maxMeasurementX_mm()
{
	return mRappTowerLocations[2].x_mm - measurementOffset_mm;
}

uint32_t rfb::minMeasurementY_mm()
{
	return mRappTowerLocations[0].y_mm + measurementOffset_mm;
}

uint32_t rfb::maxMeasurementY_mm()
{
	return mRappTowerLocations[2].y_mm - measurementOffset_mm;
}

uint32_t rfb::minMeasurementZ_mm()
{
	return 1000;
}

uint32_t rfb::maxMeasurementZ_mm()
{
	return 9000;
}


bool rfb::withinBoundary(const rfm::planePoint_t& point)
{
	rfm::rappPoint_t p = toRappCoordinates(point);
	return withinBoundary(p);
}

bool rfb::withinBoundary(const rfm::rappPoint_t& point)
{
	return withinBoundary(point.x_mm, point.y_mm);
}

bool rfb::withinBoundary(const std::int32_t x_mm, const std::int32_t y_mm)
{
	if ((x_mm < minX_mm()) || (x_mm > maxX_mm()))
		return false;

	if ((y_mm < minY_mm()) || (y_mm > maxY_mm()))
		return false;

	return true;
}

bool rfb::withinMeasurementBoundary(const rfm::planePoint_t& point)
{
	rfm::rappPoint_t p = toRappCoordinates(point);
	return withinMeasurementBoundary(p);
}

bool rfb::withinMeasurementBoundary(const rfm::rappPoint_t& point)
{
	return withinMeasurementBoundary(point.x_mm, point.y_mm);
}

bool rfb::withinMeasurementBoundary(const std::int32_t x_mm, const std::int32_t y_mm)
{
	if ((x_mm < minMeasurementX_mm()) || (x_mm > maxMeasurementX_mm()))
		return false;

	if ((y_mm < minMeasurementY_mm()) || (y_mm > maxMeasurementY_mm()))
		return false;

	return true;
}

bool rfb::withinMeasurementBoundary(const std::int32_t x_mm, const std::int32_t y_mm, const std::int32_t z_mm)
{
	if ((x_mm < minMeasurementX_mm()) || (x_mm > maxMeasurementX_mm()))
		return false;

	if ((y_mm < minMeasurementY_mm()) || (y_mm > maxMeasurementY_mm()))
		return false;

	if ((z_mm < minMeasurementZ_mm()) || (z_mm > maxMeasurementZ_mm()))
		return false;

	return true;
}

bool rfb::withinMeasurementHeight(const std::int32_t z_mm)
{
	if ((z_mm < minMeasurementZ_mm()) || (z_mm > maxMeasurementZ_mm()))
		return false;

	return true;
}

rfm::rappPoint_t rfb::toRappCoordinates(const rfm::planePoint_t& point)
{
	rfm::rappPoint_t result;

	result = { static_cast<std::int32_t>((mUIUC_TowerLocations[0].northing_ft - point.northing_ft) * nConstants::FT_TO_MM),
		static_cast<std::int32_t>((point.easting_ft - mUIUC_TowerLocations[0].easting_ft) * nConstants::FT_TO_MM),
		static_cast<std::int32_t>((point.elevation_ft - mUIUC_ReferenceHeight_ft) * nConstants::FT_TO_MM) };

	return result;
}

rfm::rappPoint_t rfb::fromStatePlane(const double northing_ft, const double easting_ft, const double height_ft)
{
	rfm::rappPoint_t result;

	result.x_mm = static_cast<std::int32_t>((mUIUC_TowerLocations[0].northing_ft - northing_ft) * nConstants::FT_TO_MM);
	result.y_mm = static_cast<std::int32_t>((easting_ft - mUIUC_TowerLocations[0].easting_ft) * nConstants::FT_TO_MM);
	result.z_mm = static_cast<std::int32_t>((height_ft - mUIUC_ReferenceHeight_ft) * nConstants::FT_TO_MM);

	return result;
}

rfm::rappPoint_t rfb::fromGPS(const double lat_rad, const double lng_rad, const double height_m)
{
	double theta = mRappTowerLocations_WGS84[0].mLatitude_rad - lat_rad;

	double northing_m = theta * Re_lat_m;

	theta = lng_rad - mRappTowerLocations_WGS84[0].mLongitude_rad;

	double easting_m = theta * Re_lng_m;

	rfm::rappPoint_t result;

	result.x_mm = static_cast<std::int32_t>(northing_m * nConstants::M_TO_MM);
	result.y_mm = static_cast<std::int32_t>(easting_m * nConstants::M_TO_MM);
	result.z_mm = static_cast<std::int32_t>((height_m - mUIUC_ReferenceHeight_m) * nConstants::M_TO_MM);

	return result;
}
