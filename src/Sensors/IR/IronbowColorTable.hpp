
#pragma once

#include "ThermalColorTable.hpp"


class cIronbowColorTable : public cThermalColorTable
{
public:
	cIronbowColorTable();
	cIronbowColorTable(double min, double max);

	eColorTable getType() const override { return eColorTable::IRONBOW; };

	uint32_t getColorValue(double v) const override;
};

