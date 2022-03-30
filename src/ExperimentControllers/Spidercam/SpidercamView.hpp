
#pragma once

#include "../ExperimentCtrlView.hpp"
#include "SpidercamScanArea.hpp"

#include <spidercam/spidercam_types.hpp>

// Qt Forward Declaration
QT_BEGIN_NAMESPACE
class QLabel;
class QLineEdit;
QT_END_NAMESPACE


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
	void updatePosition(spidercam::sPosition pos);
    void updateRecordingState(bool recording);

private:
    double mMinX_mm = 0;
    double mMaxX_mm = 0;
    double mMinY_mm = 0;
    double mMaxY_mm = 0;
    double mMinHeight_mm = 0;
    double mMaxHeight_mm = 0;

    cSpidercamScanArea* mpScanArea;

    QLineEdit* mpX_m;
    QLineEdit* mpY_m;
    QLineEdit* mpZ_m;

};

