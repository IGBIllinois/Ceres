
#include "SpidercamUtils.hpp"
#include <cmath>


using namespace spidercam;

bool spidercam::hasPositionChanged(const sPosition& pos1, const sPosition& pos2, double tolerance_mm)
{
	double diff = std::abs(static_cast<double>(pos1.X_mm) - static_cast<double>(pos2.X_mm));
	if (diff > tolerance_mm)
		return true;

	diff = std::abs(static_cast<double>(pos1.Y_mm) - static_cast<double>(pos2.Y_mm));
	return (diff > tolerance_mm);
}
