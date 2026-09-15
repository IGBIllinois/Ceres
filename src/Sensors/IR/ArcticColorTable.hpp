
#pragma once

#include "ThermalColorTable.hpp"

class cArcticColorTable : public cThermalColorTable
{
public:
	cArcticColorTable();
	cArcticColorTable(double min, double max);

	eColorTable getType() const override { return eColorTable::ARCTIC; };

	uint32_t getColorValue(double v) const override;
};

