
#pragma once

#include "ThermalColorTable.hpp"

class cWhiteHotColorTable : public cThermalColorTable
{
public:
	cWhiteHotColorTable();
	cWhiteHotColorTable(double min, double max);

	eColorTable getType() const override { return eColorTable::WHITE_HOT; };

	uint32_t getColorValue(double v) const override;
};

