
#include "RappFieldBoundary.hpp"

#include "Constants.hpp"

#include <proj.h>

#include <array>
#include <stdexcept>

#if 0

Lambert Conformal Conic (LCC)
	Used when the zone is wider east–west(e.g., Illinois East / West, California zones).
	Requires:

		φ₁ = first standard parallel

		φ₂ = second standard parallel

		φ₀ = latitude of origin

		λ₀ = central meridian

		E₀ = false easting

		N₀ = false northing

		a = ellipsoid semi-major axis

		e = ellipsoid eccentricity

	1. Precompute constants

			𝑚(𝜙) = cos𝜙 / sqrt(1 − 𝑒^2 sin^2 𝜙)

			𝑡(𝜙) = tan( 	𝜋/4 − 𝜙/2 ) ((1 - 𝑒 sin𝜙) / (1 + 𝑒 sin𝜙)) ^ (𝑒/2)

			𝑛 = 	ln(𝑚(𝜙1	)) − ln(𝑚(𝜙2)) / ln(𝑡(𝜙1)) − ln(𝑡(𝜙2))

			𝐹 = 𝑚(𝜙1) / 𝑛 𝑡(𝜙1)^𝑛

			𝜌(𝜙) = 𝑎𝐹 / 𝑡(𝜙)^𝑛

			𝜌0 = 𝜌(𝜙0)

	2. Forward projection(lat φ, lon λ → x, y)
			
			𝜃 = 	𝑛(𝜆 	− 𝜆0)

			𝑥 = 	𝐸0 + 𝜌(𝜙) sin𝜃

			𝑦 = 	𝑁0 + 𝜌0 − 𝜌(𝜙) cos𝜃


Transverse Mercator(TM)
	Used for zones like New York Long Island, some Texas zones, and UTM.
	Requires:

		λ₀ = central meridian
		φ₀ = latitude of origin
		k₀ = scale factor
		E₀, N₀ = false easting / northing
		a, e = ellipsoid parameters

	1. Precompute ellipsoid terms

			𝑒′^2 = 𝑒^2 / (1 − 𝑒^2)

			𝑁 = 𝑎 / sqrt(1 − 𝑒^2 sin^2 𝜙)

			𝑇 = tan^⁡2 𝜙

			𝐶 = 𝑒′^2 cos^2 𝜙

			𝐴 = (𝜆 − 𝜆0) cos 𝜙

	2. Meridional arc

			𝑀 = 𝑎 [ (1 − (𝑒^2) / 4 − (3𝑒^4) / 64 − (5𝑒^6) / 256)𝜙 −((3𝑒^2) / 8 + (3𝑒^4) / 32 + (	45𝑒^	6) / 1024)sin(2𝜙) + ((15𝑒^4)/256 + (45𝑒^	6)/1024)sin(4𝜙)	− ((35𝑒^	6)/3072)sin(6𝜙)	]

	3. Forward projection

			𝑥 = 	𝐸0 + 𝑘0 𝑁 [ 𝐴 + (( 1 − 𝑇 + 𝐶 ) 𝐴^3)/6 + (( 5 − 18𝑇 + 𝑇^2 + 72𝐶 − 58𝑒′^2) 𝐴^5)/120 ]

			𝑦 = 𝑁0 + 𝑘0 [ 𝑀 − 𝑀0 + 𝑁 tan𝜙 ((𝐴^2)/2 + ((5 − 𝑇 + 9𝐶 + 4𝐶^2)	𝐴^4)/24 + ((61 − 58𝑇	+ 𝑇^2 + 600𝐶 − 330𝑒′^2)	𝐴^6)/720 ) ]

#endif

namespace
{
	// Method: Transverse Mercator

	// Ellipsoid(NAD83) :
	constexpr double semi_major_axis = 6378137.0;

	// Flattening
	constexpr double flattening = 1.0 / 298.257222101;

	// Zone parameters:
	// 
	// Latitude of Origin = 36 40′ 00′′
	constexpr double latitude_of_origin_deg = 36.6666666667;

	// Central Meridian = 88 20′ 00′′
	constexpr double central_meridian_deg = -88.3333333333;

	// Scale factor at central meridian
	constexpr double scale_factor_at_central_meridian = 0.999975;

	// False easting
	constexpr double false_easting_m = 300000.0;

	// False northing
	constexpr double false_northing_m = 0.0;

	constexpr double Re = 6371000.0;   // Earth radius in meters at equator


/*
	// Lambert Conformal Conic Projection (two standard parallels)
	struct LCCParams {
		double lat0;   // Latitude of origin (degrees)
		double lon0;   // Central meridian (degrees)
		double lat1;   // First standard parallel (degrees)
		double lat2;   // Second standard parallel (degrees)
		double falseE; // False easting (meters)
		double falseN; // False northing (meters)
		double R;      // Earth radius (meters)
	};

	void lambertConformalConic(double latDeg, double lonDeg, const LCCParams& p, double& x, double& y)
	{
		// Input validation
		if (p.R <= 0) throw std::invalid_argument("Earth radius must be positive.");

		if (std::abs(p.lat1 - p.lat2) < 1e-12)
			throw std::invalid_argument("Standard parallels must be different.");

		// Convert degrees to radians
		double lat = latDeg * DEG_TO_RAD;
		double lon = lonDeg * DEG_TO_RAD;
		double lat0 = p.lat0 * DEG_TO_RAD;
		double lon0 = p.lon0 * DEG_TO_RAD;
		double lat1 = p.lat1 * DEG_TO_RAD;
		double lat2 = p.lat2 * DEG_TO_RAD;

		// Compute projection constants
		double n = std::log(std::cos(lat1) / std::cos(lat2)) / std::log(std::tan(M_PI / 4 + lat2 / 2) / std::tan(M_PI / 4 + lat1 / 2));

		double F = (std::cos(lat1) * std::pow(std::tan(M_PI / 4 + lat1 / 2), n)) / n;

		double rho = p.R * F / std::pow(std::tan(M_PI / 4 + lat / 2), n);

		double rho0 = p.R * F / std::pow(std::tan(M_PI / 4 + lat0 / 2), n);

		// Compute projected coordinates
		x = p.falseE + rho * std::sin(n * (lon - lon0));
		y = p.falseN + rho0 - rho * std::cos(n * (lon - lon0));
	}
*/

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



struct StatePlane {
	double easting;
	double northing;
	double height;
};

//StatePlane LatLonToIllinoisEastSPCS(double lat_deg, double lon_deg, double height_m)
//{

//}

// Convert latitude, longitude, ellipsoid height → State Plane coordinates
StatePlane LatLonToStatePlane(double lat_deg, double lon_deg, double h_meters, const char* spcs_epsg)
{
	// Create PROJ context
	PJ_CONTEXT* ctx = proj_context_create();

/*
	Know your State Plane zone(e.g., EPSG: 6456 = NAD83(2011) / Illinois East).

		Champaign, IL → Illinois East(SPCS)

		NAD83(2011) : EPSG 6456

		NAD83 : EPSG 3435
*/

	// NAD83: EPSG 3435
	// 
	// Geographic input CRS (WGS84)
	PJ* src = proj_create(ctx, "EPSG:4979");  // WGS84 lat/lon + ellipsoid height

	// State Plane output CRS
	PJ* dst = proj_create(ctx, spcs_epsg);

	PJ_AREA* area = proj_area_create();

	// Create transformation
	PJ* transform = proj_create_crs_to_crs_from_pj(ctx, src, dst, area, nullptr);

	// Normalize for coordinate operations
	PJ* norm = proj_normalize_for_visualization(ctx, transform);

	// Input coordinate (radians for lat/lon)
	PJ_COORD in;
	in.lpzt.phi = proj_torad(lat_deg);
	in.lpzt.lam = proj_torad(lon_deg);
	in.lpzt.z = h_meters;
	in.lpzt.t = 0;

	// Transform
	PJ_COORD out = proj_trans(norm, PJ_FWD, in);

	// Cleanup
	proj_area_destroy(area);
	proj_destroy(src);
	proj_destroy(dst);
	proj_destroy(transform);
	proj_destroy(norm);
	proj_context_destroy(ctx);

	// Return State Plane coordinates
	return { out.xyz.x, out.xyz.y, out.xyz.z };
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
	// Illinois East NAD83(2011)
	//const char* epsg = "EPSG:6454";

	//StatePlane sp = LatLonToStatePlane(lat_rad * nConstants::RAD_TO_DEG, lng_rad * nConstants::RAD_TO_DEG, height_m, epsg);

	//sp.northing;
	//sp.easting;
	//sp.height;

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




/*
#include <iostream>
#include <proj.h>

struct StatePlane {
	double easting;
	double northing;
	double height;
};

// Convert latitude, longitude, ellipsoid height → State Plane coordinates
StatePlane LatLonToStatePlane(double lat_deg, double lon_deg, double h_meters,
							  const char* spcs_epsg)
{
	// Create PROJ context
	PJ_CONTEXT* ctx = proj_context_create();

	// Geographic input CRS (WGS84)
	PJ* src = proj_create(ctx, "EPSG:4979");  // WGS84 lat/lon + ellipsoid height

	// State Plane output CRS
	PJ* dst = proj_create(ctx, spcs_epsg);

	// Create transformation
	PJ* transform = proj_create_crs_to_crs_from_pj(ctx, src, dst, nullptr);

	// Normalize for coordinate operations
	PJ* norm = proj_normalize_for_visualization(ctx, transform);

	// Input coordinate (radians for lat/lon)
	PJ_COORD in;
	in.lpzt.phi = proj_torad(lat_deg);
	in.lpzt.lam = proj_torad(lon_deg);
	in.lpzt.z   = h_meters;
	in.lpzt.t   = 0;

	// Transform
	PJ_COORD out = proj_trans(norm, PJ_FWD, in);

	// Cleanup
	proj_destroy(src);
	proj_destroy(dst);
	proj_destroy(transform);
	proj_destroy(norm);
	proj_context_destroy(ctx);

	// Return State Plane coordinates
	return { out.xyz.x, out.xyz.y, out.xyz.z };
}

int main() {
	// Example: Champaign, IL
	double lat = 40.1164;
	double lon = -88.2434;
	double h   = 230.0;  // meters

	// Illinois East NAD83(2011)
	const char* epsg = "EPSG:6456";

	StatePlane sp = LatLonToStatePlane(lat, lon, h, epsg);

	std::cout << "State Plane Easting:  " << sp.easting << "\n";
	std::cout << "State Plane Northing: " << sp.northing << "\n";
	std::cout << "State Plane Height:   " << sp.height << "\n";

	return 0;
}
*/