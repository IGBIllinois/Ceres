
#include "GpsFileReader.hpp"
#include "RappFieldBoundary.hpp"

#include "Constants.hpp"

#include <csv2/reader.hpp>

cGpsFileReader::cGpsFileReader()
{
}

cGpsFileReader::cGpsFileReader(const std::string& filename)
{
	loadFromFile(filename);
}

const std::optional<sSpiderCamPosition_t>& cGpsFileReader::GetRefPoint() const
{
	return mRefPoint;
}

const positions_t& cGpsFileReader::GetPoints() const
{
	return mPoints;
}

void cGpsFileReader::loadFromFile(const std::string& filename)
{
	csv2::Reader reader;

	if (!reader.mmap(filename))
	{
		return;
	}

	bool isIlStatePlane = true;

	auto header = reader.header();

	if (header.length() > 0)
	{
		auto it = header.begin();

		std::string type;
		(*it).read_value<std::string>(type);
		++it;

		if (type == "ILUC")
		{
			isIlStatePlane = true;
		}
		else if (type == "SPDR")
		{
			isIlStatePlane = false;

			std::string val;
			(*it).read_value<std::string>(val);
			auto x_mm = std::stoi(val);
			++it;
			val.clear();

			(*it).read_value<std::string>(val);
			auto y_mm = std::stoi(val);
			++it;
			val.clear();

			(*it).read_value<std::string>(val);
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

	auto c = reader.cols();
	auto n = reader.rows();

	for (const auto& row : reader)
	{
		if (row.length() == 0) break;

		auto it = row.begin();

		std::string label;
		(*it).read_value<std::string>(label);
		++it;

		std::string val;
		(*it).read_value<std::string>(val);
		auto northing_ft = std::stod(val);
		++it;
		val.clear();

		(*it).read_value<std::string>(val);
		auto easting_ft = std::stod(val);
		++it;
		val.clear();

		(*it).read_value<std::string>(val);
		auto elevation_ft = std::stod(val);
		val.clear();

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
}

