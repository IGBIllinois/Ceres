
#include "SpidercamExperimentStates.hpp"
#include "SpidercamCtrl.hpp"
#include "../../Utilities/Constants.hpp"

#include <QMessageBox>

#include <thread>

namespace
{
	template <typename T>
	T abs_difference(T v1, T v2)
	{
		return (v2 > v1) ? v2 - v1 : v1 - v2;
	}
}


/******************************************************************************
 * Base Class for Spidercam Experiment States
 ******************************************************************************/

cSpidercamExperimentState::cSpidercamExperimentState(const spidercam::sPosition_1_t& pos,
	cSpidercamController& controller, uint32_t tolerance_mm)
	:
	mDollyPos(pos), mController(controller), mTolerance_mm(tolerance_mm)
{
}

QString cSpidercamExperimentState::getStatusStr()
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

bool cSpidercamExperimentState::recording()
{
	return mRecordData;
}

void cSpidercamExperimentState::run()
{
	mBusy = mController.isBusy();
	mIsConsoleConnected = mController.isConsoleConnected();
	mIsMoving = mController.isMoving();
	mIsSetPointEnabled = mController.isSetPointEnabled();
	mInError = mController.isInError();
	mInScriptMode = mController.isInScriptMode();

	bool readyForMotion = mIsConsoleConnected && mIsSetPointEnabled && !mBusy; // && !mInError;

	if (mMotionDetected) return;

	if (mMoveCommandSent)
	{
		if ((mDollyPos.speed_mmps > 0) || mIsMoving)
		{
			mMotionDetected = true;
			return;
		}
	}

	if (!readyForMotion) return;

	mMoveCommandSent = mController.sendRequestNewPosition(mX_mm, mY_mm, mZ_mm,
		mZ_mm, mSpeed_mmps, mPan_deg, mTilt_deg, 0.0f);
	mStopCommandSent = false;
}

void cSpidercamExperimentState::pause()
{
	if (mIsMoving)
		mStopCommandSent = mController.requestStop();

	mMotionDetected = false;
	mMoveCommandSent = false;
	mBusy = false;
	mIsMoving = false;
	mIsSetPointEnabled = false;
	mInError = false;
}

void cSpidercamExperimentState::stop()
{
	if (mIsMoving)
		mController.requestStop();
}

cExperimentState::eRESULT cSpidercamExperimentState::finished()
{
	mIsConsoleConnected = mController.isConsoleConnected();
	mIsSetPointEnabled = mController.isSetPointEnabled();
	mInError = mController.isInError();
	mInScriptMode = mController.isInScriptMode();

	bool readyForMotion = mIsConsoleConnected && mIsSetPointEnabled; // && !mInError;
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


/******************************************************************************
 * Basic Movement Step
 ******************************************************************************/

cSpidercamExperimentState_Movement::cSpidercamExperimentState_Movement(const spidercam::sPosition_1_t& pos,
	cSpidercamController& controller, uint32_t tolerance_mm)
:
	cSpidercamExperimentState(pos, controller, tolerance_mm)
{
}

bool cSpidercamExperimentState_Movement::configure(const nlohmann::json& stateDoc)
{
	using namespace nlohmann;

	try
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

		if (stateDoc.contains("speed (mm/s)"))
		{
			mSpeed_mmps = stateDoc["speed (mm/s)"].get<double>();
		}
		else
			mSpeed_mmps = stateDoc["speed (m/s)"].get<double>() * nConstants::M_TO_MM;

		if (stateDoc.contains("pan"))
		{
			mPan_deg = stateDoc["pan"];
		}
		else if (stateDoc.contains("pan (deg)"))
		{
			mPan_deg = stateDoc["pan (deg)"];
		}
		else
		{
			mPan_NeedsInitialization = true;
		}

		if (stateDoc.contains("tilt"))
		{
			mTilt_deg = stateDoc["tilt"];
		}
		else if (stateDoc.contains("tilt (deg)"))
		{
			mTilt_deg = stateDoc["tilt (deg)"];
		}
		else
		{
			mTilt_NeedsInitialization = true;
		}

		if (stateDoc.contains("roll"))
		{
			mRoll_deg = stateDoc["roll"];
		}
		else if (stateDoc.contains("roll (deg)"))
		{
			mRoll_deg = stateDoc["roll (deg)"];
		}
		else
		{
			mRoll_NeedsInitialization = true;
		}

		mRecordData = stateDoc["record"];
	}
	catch (const detail::parse_error& e)
	{
		QString msg = "Parse Error: ";
		msg += e.what();

		QMessageBox mb(QMessageBox::Critical, "SpiderCam Experiment State Error", msg);
		mb.exec();

		return false;
	}
	catch (const detail::type_error& e)
	{
		QString msg = "Type Error: ";
		msg += e.what();

		QMessageBox mb(QMessageBox::Critical, "SpiderCam Experiment State Error", msg);
		mb.exec();

		return false;
	}
	catch (const detail::exception& e)
	{
		QString msg = "Unknown Error: ";
		msg += e.what();

		QMessageBox mb(QMessageBox::Critical, "SpiderCam Experiment State Error", msg);
		mb.exec();

		return false;
	}

	return true;
}

bool cSpidercamExperimentState_Movement::initialize()
{
	mMotionDetected = false;
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

	if (mPan_NeedsInitialization)
	{
		mPan_deg = mController.getLastKnownPosition().pan_deg;
	}
	
	if (mTilt_NeedsInitialization)
	{
		mTilt_deg = mController.getLastKnownPosition().tilt_deg;
	}

	if (mRoll_NeedsInitialization)
	{
		mRoll_deg = mController.getLastKnownPosition().roll_gimbal_deg;
	}

	return true;
}


/******************************************************************************
 * Delta Movement Step
 ******************************************************************************/

cSpidercamExperimentState_DeltaMovement::cSpidercamExperimentState_DeltaMovement(const spidercam::sPosition_1_t& pos,
	cSpidercamController& controller, uint32_t tolerance_mm)
	:
	cSpidercamExperimentState(pos, controller, tolerance_mm)
{
}

bool cSpidercamExperimentState_DeltaMovement::configure(const nlohmann::json& stateDoc)
{
	using namespace nlohmann;

	try
	{
		auto pos = stateDoc["position"];

		if (pos.contains("dx (mm)"))
		{
			mDeltaX_mm = static_cast<uint32_t>(pos["dx (mm)"].get<double>());
		}
		else if (pos.contains("dx (m)"))
		{
			mDeltaX_mm = static_cast<uint32_t>(pos["x (m)"].get<double>() * nConstants::M_TO_MM);
		}

		if (pos.contains("dy (mm)"))
		{
			mDeltaY_mm = static_cast<uint32_t>(pos["dy (mm)"].get<double>());
		}
		else if (pos.contains("dy (m)"))
		{
			mDeltaY_mm = static_cast<uint32_t>(pos["dy (m)"].get<double>() * nConstants::M_TO_MM);
		}

		if (pos.contains("dz (mm)"))
		{
			mDeltaZ_mm = static_cast<uint32_t>(pos["dz (mm)"].get<double>());
		}
		else if (pos.contains("dz (m)"))
		{
			mDeltaZ_mm = static_cast<uint32_t>(pos["dz (m)"].get<double>() * nConstants::M_TO_MM);
		}

		if (stateDoc.contains("speed (mm/s)"))
		{
			mSpeed_mmps = stateDoc["speed (mm/s)"].get<double>();
		}
		else
			mSpeed_mmps = stateDoc["speed (m/s)"].get<double>() * nConstants::M_TO_MM;

		if (stateDoc.contains("pan"))
		{
			mPan_deg = stateDoc["pan"];
		}
		else if (stateDoc.contains("pan (deg)"))
		{
			mPan_deg = stateDoc["pan (deg)"];
		}
		else
		{
			mPan_NeedsInitialization = true;
		}

		if (stateDoc.contains("tilt"))
		{
			mTilt_deg = stateDoc["tilt"];
		}
		else if (stateDoc.contains("tilt (deg)"))
		{
			mTilt_deg = stateDoc["tilt (deg)"];
		}
		else
		{
			mTilt_NeedsInitialization = true;
		}

		if (stateDoc.contains("roll"))
		{
			mRoll_deg = stateDoc["roll"];
		}
		else if (stateDoc.contains("roll (deg)"))
		{
			mRoll_deg = stateDoc["roll (deg)"];
		}
		else
		{
			mRoll_NeedsInitialization = true;
		}

		mRecordData = stateDoc["record"];
	}
	catch (const detail::parse_error& e)
	{
		QString msg = "Parse Error: ";
		msg += e.what();

		QMessageBox mb(QMessageBox::Critical, "SpiderCam Experiment State Error", msg);
		mb.exec();

		return false;
	}
	catch (const detail::type_error& e)
	{
		QString msg = "Type Error: ";
		msg += e.what();

		QMessageBox mb(QMessageBox::Critical, "SpiderCam Experiment State Error", msg);
		mb.exec();

		return false;
	}
	catch (const detail::exception& e)
	{
		QString msg = "Unknown Error: ";
		msg += e.what();

		QMessageBox mb(QMessageBox::Critical, "SpiderCam Experiment State Error", msg);
		mb.exec();

		return false;
	}

	return true;
}

bool cSpidercamExperimentState_DeltaMovement::initialize()
{
	mMotionDetected = false;
	mMoveCommandSent = false;
	mStopCommandSent = false;
	mIsMoving = false;
	mBusy = mController.isBusy();
	mIsSetPointEnabled = mController.isSetPointEnabled();
	mInError = mController.isInError();
	mIsConsoleConnected = mController.isConsoleConnected();
	mInScriptMode = mController.isInScriptMode();

	mX_mm = mController.getLastKnownPosition().X_mm;
	mY_mm = mController.getLastKnownPosition().Y_mm;
	mZ_mm = mController.getLastKnownPosition().Z_mm;

	mX_mm += mDeltaX_mm;
	mY_mm += mDeltaY_mm;
	mZ_mm += mDeltaZ_mm;

	if (mPan_NeedsInitialization)
	{
		mPan_deg = mController.getLastKnownPosition().pan_deg;
	}

	if (mTilt_NeedsInitialization)
	{
		mTilt_deg = mController.getLastKnownPosition().tilt_deg;
	}

	if (mRoll_NeedsInitialization)
	{
		mRoll_deg = mController.getLastKnownPosition().roll_gimbal_deg;
	}

	return true;
}


