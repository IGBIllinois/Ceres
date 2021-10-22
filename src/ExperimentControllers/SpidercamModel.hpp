
#pragma once

#include "ExperimentCtrlModel.hpp"

#include "SpidercamCtrl.hpp"
#include "SpidercamUtils.hpp"
#include "../Utilities/Timers.hpp"
#include "../Utilities/Utilities.hpp"

class cSpidercamModel : public cExperimentControlModel
{
    Q_OBJECT

public:
	cSpidercamModel();
	~cSpidercamModel();


    /*
     * Apply any configuration parameters to the sensor
     * model.
     */
    void configure(const nlohmann::json& jsonCfg) override;

    /*
     * Write any "header" data block into the data file.
     * A header data block is a metadata block that is
     * constant over the span of the experiment.
     */
    void writeDataHeader(cBlockDataFile& file) override;


signals:
    void limitsChanged(spidercam::sWorkingDimensions limits);
    void positionChanged(spidercam::sPosition pos);
	void movingChanged(bool moving);
	void batteryLevelChanged(float level_pct);

/*
	bool mDollyConnected;
	bool mConsoleConnected;
	bool mIpCameraConnected;
	bool mSafetyCircuitRestarted;
	bool mActivated;
	bool mDollyPositionKnown;
	bool mPlaceKnown;
	bool mBordersKnown;
	bool mFieldKnown;
	bool mSetPointEnabled;
	bool mConsoleEnabled;
	bool mObstacleLessThan2000mm;
	bool mObstacleLessThan1500mm;
	bool mObstacleLessThan1000mm;
	bool mObstacleLessThan500mm;
	bool mBusy;
	bool mDone;
	bool mMoving;
	bool mCalibrated;
	bool mCableLengthAdjustmentRequired;
	bool mNearBorder;
	bool mInPosition;
	bool mAtCalibrationPosition;
	bool mAtCorrectionPosition;
	bool mInScriptMode;
	bool mInInteractiveMode;
	bool mInError;

	float mBatteryLevel_pct;
*/

protected:
	void run() override;

protected:

	spidercam::sPosition mCurrentPosition;


	comparator<int, spidercam::window_compare<int, 2>> mBatteryLevel_pct;

	edge_detect<bool>	mBusy;
	edge_detect<bool>	mInError;
	edge_detect<bool>	mDone;
	edge_detect<bool>	mMoving;
	edge_detect<bool>	mObstacleLessThan2000mm;
	edge_detect<bool>	mObstacleLessThan1500mm;
	edge_detect<bool>	mObstacleLessThan1000mm;
	edge_detect<bool>	mObstacleLessThan500mm;

	cIntervalTimer	mTimer;

	double mPositionTolerance_mm;


private:
    cSpidercamController mController;
};

