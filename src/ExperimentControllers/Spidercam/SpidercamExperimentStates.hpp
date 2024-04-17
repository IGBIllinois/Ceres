
#pragma once

#include "ExperimentState.hpp"

#include <spidercam/spidercam_types.hpp>

// Forward Declarations
class cSpidercamController;


class cSpidercamExperimentState_Movement : public cExperimentState
{
public:
	cSpidercamExperimentState_Movement(const spidercam::sPosition_1_t& pos,
		cSpidercamController& controller, uint32_t tolerance_mm);

	QString getStatusStr() override;

	bool configure(const nlohmann::json& stateDoc) override;

	bool recording() override;

	bool initialize() override;
	void run() override;
	void pause() override;
	void stop() override;
	eRESULT finished() override;

private:
	const spidercam::sPosition_1_t& mDollyPos;
	cSpidercamController& mController;

	bool mMotionDetected = false;
	bool mMoveCommandSent = false;
	bool mStopCommandSent = false;
	bool mBusy = false;
	bool mIsConsoleConnected = false;
	bool mIsMoving = false;
	bool mIsSetPointEnabled = false;
	bool mInError = false;
	bool mInScriptMode = false;

	bool mRecordData = false;

	double mX_mm = 0.0;
	double mY_mm = 0.0;
	double mZ_mm = 0.0;

	bool mX_NeedsInitialization = false;
	bool mY_NeedsInitialization = false;
	bool mZ_NeedsInitialization = false;

	uint32_t mTolerance_mm = 10.0;

	double mSpeed_mmps = 0.0;

	double mPan_deg = 0.0;
	double mTilt_deg = 0.0;
	double mRoll_deg = 0.0;

	bool mPan_NeedsInitialization = false;
	bool mTilt_NeedsInitialization = false;
	bool mRoll_NeedsInitialization = false;
};

