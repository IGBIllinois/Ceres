
#pragma once

#include "../ExperimentCtrlView.hpp"
#include "SpidercamScanArea.hpp"

#include <spidercam/spidercam_types.hpp>

// Qt Forward Declaration
QT_BEGIN_NAMESPACE
class QLabel;
class QLineEdit;
class QStatusBar;
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
	void updatePosition(spidercam::sPosition_1_t pos);
    void updateRecordingState(bool recording);

protected:
    void refresh() override;
    void experimentStateChanging(experiment::eState state) override;
    void experimentStatusUpdating(QString msg) override;

private:
    double mMinX_mm = 0;
    double mMaxX_mm = 0;
    double mMinY_mm = 0;
    double mMaxY_mm = 0;
    double mMinHeight_mm = 0;
    double mMaxHeight_mm = 0;

    std::string mLayoutFilename;

    cSpidercamScanArea* mpScanArea = nullptr;

    QLineEdit* mpX_m = nullptr;
    QLineEdit* mpY_m = nullptr;
    QLineEdit* mpZ_m = nullptr;

    QStatusBar* mpExperimentStatus = nullptr;
};

