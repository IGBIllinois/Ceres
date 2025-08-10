
#pragma once

#include "ExperimentState.hpp"

#include <spidercam/spidercam_types.hpp>

// Forward Declarations
class iSpidercamController;


class cSpidercamExperimentState : public cExperimentState
{
public:
	cSpidercamExperimentState(const spidercam::sPosition_1_t& pos,
		iSpidercamController& controller, uint32_t tolerance_mm);

	QString getStatusStr() override;

	bool recording() override;

	void run() override;
	void pause() override;
	void stop() override;
	eRESULT finished() override;

protected:
	const spidercam::sPosition_1_t& mDollyPos;
	iSpidercamController& mController;

	bool mErrorDetected = false;

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

	uint32_t mTolerance_mm = 10.0;

	double mSpeed_mmps = 0.0;

	double mX_mm = 0.0;
	double mY_mm = 0.0;
	double mZ_mm = 0.0;

	double mPan_deg = 0.0;
	double mTilt_deg = 0.0;
	double mRoll_deg = 0.0;
};


class cSpidercamExperimentState_Movement : public cSpidercamExperimentState
{
public:
	cSpidercamExperimentState_Movement(const spidercam::sPosition_1_t& pos,
		iSpidercamController& controller, uint32_t tolerance_mm);

	bool configure(const nlohmann::json& stateDoc) override;

	bool initialize() override;

private:
	int32_t mHeight_mm = 0;
	int32_t mReferenceHeight_mm = 0;
	bool mAGH_NeedsInitialization = false;
	bool mACH_NeedsInitialization = false;

	bool mX_NeedsInitialization = false;
	bool mY_NeedsInitialization = false;
	bool mZ_NeedsInitialization = false;

	bool mPan_NeedsInitialization = false;
	bool mTilt_NeedsInitialization = false;
	bool mRoll_NeedsInitialization = false;
};


class cSpidercamExperimentState_DeltaMovement : public cSpidercamExperimentState
{
public:
	cSpidercamExperimentState_DeltaMovement(const spidercam::sPosition_1_t& pos,
		iSpidercamController& controller, uint32_t tolerance_mm);

	bool configure(const nlohmann::json& stateDoc) override;

	bool initialize() override;

private:
	double mDeltaX_mm = 0.0;
	double mDeltaY_mm = 0.0;
	double mDeltaZ_mm = 0.0;

	bool mPan_NeedsInitialization = false;
	bool mTilt_NeedsInitialization = false;
	bool mRoll_NeedsInitialization = false;
};

