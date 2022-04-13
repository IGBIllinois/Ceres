
#pragma once

#include "ExperimentCtrlView.hpp"

// Qt Forward Declaration
QT_BEGIN_NAMESPACE
class QLabel;
class QLineEdit;
QT_END_NAMESPACE


class cDummyControlView : public cExperimentControlView
{
public:
    cDummyControlView();
	~cDummyControlView();

    /*
     * Apply any configuration parameters to the experiment
     * controller view.
     */
    void configure(const nlohmann::json& jsonCfg) override;


public slots:
    void updateRecordingState(bool recording);

private:
};

