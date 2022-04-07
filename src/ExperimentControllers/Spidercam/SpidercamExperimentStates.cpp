
#include "SpidercamExperimentStates.hpp"
#include "SpidercamCtrl.hpp"
#include "../../Utilities/Constants.hpp"

#include <thread>

namespace
{
	template <typename T>
	T abs_difference(T v1, T v2)
	{
		return (v2 > v1) ? v2 - v1 : v1 - v2;
	}
}



cSpidercamExperimentState_Movement::cSpidercamExperimentState_Movement(const spidercam::sPosition& pos, 
	cSpidercamController& controller, uint32_t tolerance_mm)
:
	mDollyPos(pos), mController(controller), mRecordData(false), 
	mX_mm(0), mY_mm(0), mZ_mm(0), mTolerance_mm(tolerance_mm),
	mSpeed_mmps(0), mPan_deg(0), mTilt_deg(0)

{
	mMoveCommandSent = false;
	mStopCommandSent = false;
	mBusy = false;
	mIsMoving = false;
	mIsSetPointEnabled = false;
	mInError = false;
}

void cSpidercamExperimentState_Movement::configure(const nlohmann::json& stateDoc)
{
	auto pos = stateDoc["position"];
	mX_mm = static_cast<uint32_t>(pos["x (m)"].get<double>() * nConstants::M_TO_MM);
	mY_mm = static_cast<uint32_t>(pos["y (m)"].get<double>() * nConstants::M_TO_MM);
	mZ_mm = static_cast<uint32_t>(pos["z (m)"].get<double>() * nConstants::M_TO_MM);

	mSpeed_mmps = stateDoc["speed (m/s)"].get<double>() * nConstants::M_TO_MM;
	mPan_deg = stateDoc["pan"];
	mTilt_deg = stateDoc["tilt"];

	mRecordData = stateDoc["record"];
}

QString cSpidercamExperimentState_Movement::getStatusStr()
{
	QString msg = "Moving to: ";
	msg.append(std::to_string(mX_mm).c_str());
	msg += "mm, ";
	msg.append(std::to_string(mY_mm).c_str());
	msg += "mm, ";
	msg.append(std::to_string(mZ_mm).c_str());
	msg += "mm.";
	return msg;
}

bool cSpidercamExperimentState_Movement::recording()
{
	return mRecordData;
}

void cSpidercamExperimentState_Movement::initialize()
{
	mMoveCommandSent = false;
	mStopCommandSent = false;
	mBusy = false;
	mIsMoving = false;
	mIsSetPointEnabled = false;
	mInError = false;
}

void cSpidercamExperimentState_Movement::run()
{
	mBusy = mController.isBusy();
	mIsMoving = mController.isMoving();
	mIsSetPointEnabled = mController.isSetPointEnabled();
	mInError = mController.isInError();

	bool readyForMotion = mIsSetPointEnabled && !mBusy && !mInError;

	if (mMoveCommandSent) return;

	if (!readyForMotion) return;

	mMoveCommandSent = mController.sendRequestNewPosition(mX_mm, mY_mm, mZ_mm, mSpeed_mmps, mPan_deg, mTilt_deg, 0.0f);
	mStopCommandSent = false;

}

void cSpidercamExperimentState_Movement::pause()
{
	if (!mStopCommandSent)
		mStopCommandSent = mController.requestStop();

	mMoveCommandSent = false;
	mBusy = false;
	mIsMoving = false;
	mIsSetPointEnabled = false;
	mInError = false;
}

bool cSpidercamExperimentState_Movement::finished()
{
	if (mIsMoving)
		return false;

	return ((abs_difference(mDollyPos.X_mm, mX_mm) < mTolerance_mm) &&
			(abs_difference(mDollyPos.Y_mm, mY_mm) < mTolerance_mm) &&
			(abs_difference(mDollyPos.Z_mm, mZ_mm) < mTolerance_mm));
}

