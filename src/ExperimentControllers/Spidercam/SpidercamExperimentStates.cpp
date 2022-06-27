
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
	mX_NeedsInitialization = false;
	mY_NeedsInitialization = false;
	mZ_NeedsInitialization = false;

	mMoveCommandSent = false;
	mStopCommandSent = false;
	mBusy = false;
	mIsMoving = false;
	mIsSetPointEnabled = false;
	mInError = false;
	mInScriptMode = false;
}

void cSpidercamExperimentState_Movement::configure(const nlohmann::json& stateDoc)
{
	auto pos = stateDoc["position"];

	if (pos.contains("x (mm)"))
	{
		mX_mm = static_cast<uint32_t>(pos["x (mm)"].get<double>());
	}
	else if (pos.contains("x (m)"))
	{
		mX_mm = static_cast<uint32_t>(pos["x (m)"].get<double>() * nConstants::M_TO_MM);
	}
	else
	{
		mX_NeedsInitialization = true;
		mX_mm = -1.0;
	}

	if (pos.contains("y (mm)"))
	{
		mY_mm = static_cast<uint32_t>(pos["y (mm)"].get<double>());
	}
	else if (pos.contains("y (m)"))
	{
		mY_mm = static_cast<uint32_t>(pos["y (m)"].get<double>() * nConstants::M_TO_MM);
	}
	else
	{
		mY_NeedsInitialization = true;
		mY_mm = -1.0;
	}

	if (pos.contains("z (mm)"))
	{
		mZ_mm = static_cast<uint32_t>(pos["z (mm)"].get<double>());
	}
	else if (pos.contains("z (m)"))
	{
		mZ_mm = static_cast<uint32_t>(pos["z (m)"].get<double>() * nConstants::M_TO_MM);
	}
	else
	{
		mZ_NeedsInitialization = true;
		mZ_mm = -1.0;
	}

	mSpeed_mmps = stateDoc["speed (m/s)"].get<double>() * nConstants::M_TO_MM;
	mPan_deg = stateDoc["pan"];
	mTilt_deg = stateDoc["tilt"];

	mRecordData = stateDoc["record"];
}

QString cSpidercamExperimentState_Movement::getStatusStr()
{
	QString msg = "Moving to: ";
	msg.append(std::to_string(static_cast<uint32_t>(mX_mm)).c_str());
	msg += "mm, ";
	msg.append(std::to_string(static_cast<uint32_t>(mY_mm)).c_str());
	msg += "mm, ";
	msg.append(std::to_string(static_cast<uint32_t>(mZ_mm)).c_str());
	msg += "mm at ";
	msg.append(std::to_string(static_cast<uint32_t>(mSpeed_mmps)).c_str());
	msg += "mm/sec.";
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
	mIsMoving = false;
	mBusy = mController.isBusy();
	mIsSetPointEnabled = mController.isSetPointEnabled();
	mInError = mController.isInError();
	mIsConsoleConnected = mController.isConsoleConnected();
	mInScriptMode = mController.isInScriptMode();

	if (mX_NeedsInitialization)
	{
		mX_mm = mController.getLastKnownPosition().X_mm;
	}

	if (mY_NeedsInitialization)
	{
		mY_mm = mController.getLastKnownPosition().Y_mm;
	}

	if (mZ_NeedsInitialization)
	{
		mZ_mm = mController.getLastKnownPosition().Z_mm;
	}
}

void cSpidercamExperimentState_Movement::run()
{
	mBusy = mController.isBusy();
	mIsConsoleConnected = mController.isConsoleConnected();
	mIsMoving = mController.isMoving();
	mIsSetPointEnabled = mController.isSetPointEnabled();
	mInError = mController.isInError();
	mInScriptMode = mController.isInScriptMode();

	bool readyForMotion = mIsConsoleConnected && mIsSetPointEnabled && !mBusy && !mInError;

	if (mMoveCommandSent) return;

	if (!readyForMotion) return;

	mMoveCommandSent = mController.sendRequestNewPosition(mX_mm, mY_mm, mZ_mm,
		mZ_mm, mSpeed_mmps, mPan_deg, mTilt_deg, 0.0f);
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

cExperimentState::eRESULT cSpidercamExperimentState_Movement::finished()
{
	mIsConsoleConnected = mController.isConsoleConnected();
	mIsSetPointEnabled = mController.isSetPointEnabled();
	mInError = mController.isInError();
	mInScriptMode = mController.isInScriptMode();

	bool readyForMotion = mIsConsoleConnected && mIsSetPointEnabled && !mInError;
	if (!readyForMotion)
		return eRESULT::ABORT;

	if (mIsMoving)
		return eRESULT::WAITING;

	auto dx = abs_difference(mDollyPos.X_mm, mX_mm);
	auto dy = abs_difference(mDollyPos.Y_mm, mY_mm);
	auto dz = abs_difference(mDollyPos.Z_mm, mZ_mm);

	if ((dx < mTolerance_mm) && (dy < mTolerance_mm) && (dz < mTolerance_mm))
	{
		if (mDollyPos.speed_mmps == 0)
			return eRESULT::DONE;
	}
		
	return eRESULT::WAITING;
}

