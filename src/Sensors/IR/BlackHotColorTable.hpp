
#pragma once

#include "ThermalColorTable.hpp"

class cBlackHotColorTable : public cThermalColorTable
{
public:
	cBlackHotColorTable();
	cBlackHotColorTable(double min, double max);

	eColorTable getType() const override;

	uint32_t getColorValue(double v) const override;
};

