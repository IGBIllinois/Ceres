
#pragma once

#include "../ExperimentStates.hpp"

#include <spidercam/spidercam_types.hpp>

// Forward Declarations
class cSpidercamController;


class cSpidercamExperimentState_Movement : public cExperimentState
{
public:
	cSpidercamExperimentState_Movement(const spidercam::sPosition& pos,
		cSpidercamController& controller, uint32_t tolerance_mm);

	QString getStatusStr() override;

	void configure(const nlohmann::json& stateDoc) override;

	bool recording() override;

	void initialize() override;
	void run() override;
	void pause() override;
	eRESULT finished() override;

private:
	const spidercam::sPosition& mDollyPos;
	cSpidercamController& mController;

	bool mMoveCommandSent;
	bool mStopCommandSent;
	bool mBusy;
	bool mIsConsoleConnected;
	bool mIsMoving;
	bool mIsSetPointEnabled;
	bool mInError;

	bool mRecordData;

	double mX_mm;
	double mY_mm;
	double mZ_mm;

	uint32_t mTolerance_mm;

	double mSpeed_mmps;
	double mPan_deg;
	double mTilt_deg;
};

