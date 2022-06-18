
#include "SpidercamUtils.hpp"
#include <cmath>


using namespace spidercam;

bool spidercam::hasPositionChanged(const sPosition& pos1, const sPosition& pos2, double tolerance_mm)
{
	double diff = std::abs(pos1.X_mm - pos2.X_mm);
	if (diff > tolerance_mm)
		return true;

	diff = std::abs(pos1.Y_mm - pos2.Y_mm);
	if (diff > tolerance_mm)
		return true;

	diff = std::abs(pos1.Z_mm - pos2.Z_mm);
	return (diff > tolerance_mm);
}
