
#pragma once

#include <string>
#include <vector>

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

	void loadFromFile(const std::string& filename);

	const positions_t& GetPoints() const;

private:
	positions_t mPoints;
};

