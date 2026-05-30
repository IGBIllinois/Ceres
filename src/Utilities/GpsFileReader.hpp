
#pragma once

#include <string>
#include <vector>
#include <optional>

struct sSpiderCamPosition_t
{
	std::string label;
	float x_m = 0.0;
	float y_m = 0.0;
	float z_m = 0.0;
};

typedef std::vector<sSpiderCamPosition_t> positions_t;


class cGpsFileReader
{
public:
	cGpsFileReader();
	explicit cGpsFileReader(const std::string& filename);

	bool empty() const;

	bool loadFromFile(const std::string& filename);

	const std::optional<sSpiderCamPosition_t>& GetRefPoint() const;

	const positions_t& GetPoints() const;

private:
	std::optional<sSpiderCamPosition_t> mRefPoint;
	positions_t mPoints;
};

