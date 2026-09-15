
#pragma once

#include "ThermalColorTable.hpp"

class cRainbow_HC_ColorTable : public cThermalColorTable
{
public:
	cRainbow_HC_ColorTable();
	cRainbow_HC_ColorTable(double min, double max);

	eColorTable getType() const override { return eColorTable::RAINBOW_HC; };

	uint32_t getColorValue(double v) const override;
};

