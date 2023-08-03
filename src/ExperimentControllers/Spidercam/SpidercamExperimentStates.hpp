
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

	bool mMotionDetected;
	bool mMoveCommandSent;
	bool mStopCommandSent;
	bool mBusy;
	bool mIsConsoleConnected;
	bool mIsMoving;
	bool mIsSetPointEnabled;
	bool mInError;
	bool mInScriptMode;

	bool mRecordData;

	double mX_mm;
	double mY_mm;
	double mZ_mm;

	bool mX_NeedsInitialization;
	bool mY_NeedsInitialization;
	bool mZ_NeedsInitialization;

	uint32_t mTolerance_mm;

	double mSpeed_mmps;
	double mPan_deg;
	double mTilt_deg;
};

