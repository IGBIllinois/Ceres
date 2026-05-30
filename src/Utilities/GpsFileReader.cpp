
#include "GpsFileReader.hpp"
#include "RappFieldBoundary.hpp"

#include "Constants.hpp"

#include <rapidcsv.h>

cGpsFileReader::cGpsFileReader()
{
}

cGpsFileReader::cGpsFileReader(const std::string& filename)
{
	loadFromFile(filename);
}

bool cGpsFileReader::empty() const
{
	return mPoints.empty();
}

const std::optional<sSpiderCamPosition_t>& cGpsFileReader::GetRefPoint() const
{
	return mRefPoint;
}

const positions_t& cGpsFileReader::GetPoints() const
{
	return mPoints;
}

bool cGpsFileReader::loadFromFile(const std::string& filename)
{
	rapidcsv::Document doc(filename, rapidcsv::LabelParams(0, -1));

	if (doc.GetColumnCount() == 0)
	{
		return false;
	}

	bool isIlStatePlane = true;

	auto header = doc.GetColumnNames();

	if (header.size() > 0)
	{
		auto it = header.begin();

		std::string type = *it;
		++it;

		if (type == "ILUC")
		{
			isIlStatePlane = true;
		}
		else if (type == "SPDR")
		{
			isIlStatePlane = false;

			std::string val;
			val = *it;
			auto x_mm = std::stoi(val);
			++it;
			val.clear();

			val = *it;
			auto y_mm = std::stoi(val);
			++it;
			val.clear();

			val = *it;
			auto z_mm = std::stoi(val);
			val.clear();
	
			sSpiderCamPosition_t point;
			point.label = "ref";
			point.x_m = x_mm * nConstants::MM_TO_M;
			point.y_m = y_mm * nConstants::MM_TO_M;
			point.z_m = z_mm * nConstants::MM_TO_M;
			
			mRefPoint = point;
		}
	}

	auto c = doc.GetColumnCount();
	auto n = doc.GetRowCount();

	for (size_t i = 0; i < n; ++i)
	{
		auto values = doc.GetRow<std::string>(i);

		auto label = values.at(0);
		auto northing_ft = std::stod(values.at(1));
		auto easting_ft = std::stod(values.at(2));
		auto elevation_ft = std::stod(values.at(3));

		rfm::rappPoint_t rapp_point;
		if (isIlStatePlane)
		{
			rapp_point = rfb::fromStatePlane(northing_ft, easting_ft, elevation_ft);
		}
		else
		{
			rapp_point = { static_cast<std::int32_t>(northing_ft), static_cast<std::int32_t>(easting_ft), static_cast<std::int32_t>(elevation_ft) };
		}

		sSpiderCamPosition_t point;
		point.label = label;
		point.x_m = rapp_point.x_mm * nConstants::MM_TO_M;
		point.y_m = rapp_point.y_mm * nConstants::MM_TO_M;
		point.z_m = rapp_point.z_mm * nConstants::MM_TO_M;

		mPoints.emplace_back(point);
	}

	return true;
}

