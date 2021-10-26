
#include "SpidercamDollyStatus.hpp"


cSpidercamDollyStatus::cSpidercamDollyStatus()
{
}

cSpidercamDollyStatus::~cSpidercamDollyStatus()
{
}

void cSpidercamDollyStatus::updateLimits(spidercam::sWorkingDimensions limits)
{
	mMinX_mm = limits.minX_mm;
	mMaxX_mm = limits.maxX_mm;
	mMinY_mm = limits.minY_mm;
	mMaxY_mm = limits.maxY_mm;
	mMinHeight_mm = limits.minHeight_mm;
	mMaxHeight_mm = limits.maxHeight_mm;
}

void cSpidercamDollyStatus::updatedPosition(spidercam::sPosition pos)
{
	repaint();
}

