
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
	bool finished() override;

private:
	const spidercam::sPosition& mDollyPos;
	cSpidercamController& mController;

	bool mMoveCommandSent;
	bool mBusy;
	bool mIsMoving;
	bool mIsSetPointEnabled;
	bool mInError;

	bool mRecordData;

	uint32_t mX_mm;
	uint32_t mY_mm;
	uint32_t mZ_mm;

	uint32_t mTolerance_mm;

	double mSpeed_mmps;
	double mPan_deg;
	double mTilt_deg;
};

