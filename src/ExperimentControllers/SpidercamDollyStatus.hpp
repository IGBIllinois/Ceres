
#pragma once

#include <spidercam/spidercam_types.hpp>

#include <QAbstractScrollArea>


class cSpidercamDollyStatus : public QAbstractScrollArea
{
public:
    cSpidercamDollyStatus();
	~cSpidercamDollyStatus();

public slots:
	void updateLimits(spidercam::sWorkingDimensions limits);
	void updatedPosition(spidercam::sPosition pos);

protected:
	void paintEvent(QPaintEvent* event) override;

private:
    double mMinX_mm = 0;
    double mMaxX_mm = 0;
    double mMinY_mm = 0;
    double mMaxY_mm = 0;
    double mMinHeight_mm = 0;
    double mMaxHeight_mm = 0;
};

