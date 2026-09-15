
#pragma once

#include "ThermalColorTable.hpp"

class cRainbowColorTable : public cThermalColorTable
{
public:
	cRainbowColorTable();
	cRainbowColorTable(double min, double max);

	eColorTable getType() const override { return eColorTable::RAINBOW; };

	uint32_t getColorValue(double v) const override;
};

