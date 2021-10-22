
#pragma once

#include "ExperimentCtrlView.hpp"

#include <spidercam/spidercam_types.hpp>

#include <QPainter>
#include <QPen>

class cSpidercamView : public cExperimentControlView
{
public:
	cSpidercamView();
	~cSpidercamView();

    /*
     * Apply any configuration parameters to the experiment
     * controller view.
     */
    void configure(const nlohmann::json& jsonCfg) override;


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

    QPainter*	mpDrawingArea;
	QPen* mpBorderPen;
};

