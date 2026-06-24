
#include "SpidercamExperimentStates.hpp"
#include "SpidercamCtrlProxies.hpp"
#include "AerialCompensationDlg.hpp"
#include "../../ExperimentStateMachine/ExperimentVariableTable.hpp"
#include "../../Utilities/Constants.hpp"
#include "../../Utilities/RappFieldBoundary.hpp"
#include "../../Utilities/RappFieldModel.hpp"


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
	iSpidercamController& controller, uint32_t tolerance_mm)
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
	mErrorDetected = mInError || mController.linkError();

	if (mErrorDetected)
		return;


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
	if (mErrorDetected)
		return eRESULT::ABORT;

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
	iSpidercamController& controller, uint32_t tolerance_mm)
:
	cSpidercamExperimentState(pos, controller, tolerance_mm)
{
	mReferenceHeight_mm = nRFM::reference_height_mm();
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

		if (!mX_NeedsInitialization && !mY_NeedsInitialization)
		{
			if (!rfb::withinMeasurementBoundary(mX_mm, mY_mm))
			{
				QString msg = "Spidercam Movement Error: The desired position x_mm, ";
				msg += QString::number(mX_mm);
				msg += ", and y_mm, ";
				msg += QString::number(mY_mm);
				msg += ", is outside of the measurement boundary.";

				QMessageBox mb(QMessageBox::Critical, "SpiderCam Experiment State Error", msg);
				mb.exec();

				return false;
			}
		}

		if (pos.contains("height acl (mm)"))
		{
			double aboveCanopy_mm = pos["height acl (mm)"].get<double>();

			int32_t sensor_offset_mm = -10000;
			int32_t canopy_height_mm = -10000;

			auto variables = mVariables.lock();

			if (variables->contains("canopy_height_mm"))
				variables->get("canopy_height_mm", canopy_height_mm);

			if (variables->contains("sensor_offset_mm"))
				variables->get("sensor_offset_mm", sensor_offset_mm);

			if (variables->contains("reference_height_mm"))
				variables->get("reference_height_mm", mReferenceHeight_mm);

			if ((sensor_offset_mm == -10000) || (canopy_height_mm == -10000))
			{
				cAboveCanopyHeightDlg* pDlg = new cAboveCanopyHeightDlg();

				if (canopy_height_mm != -10000)
					pDlg->setCanopyHeight_mm(canopy_height_mm);
				if (sensor_offset_mm != -10000)
					pDlg->setSensorOffset_mm(sensor_offset_mm);
				pDlg->setReferenceHeight_mm(mReferenceHeight_mm);

				auto result = pDlg->exec();

				if (result == QDialog::Rejected)
				{
					pDlg->deleteLater();
					return false;
				}

				canopy_height_mm = pDlg->getCanopyHeight_mm();
				sensor_offset_mm = pDlg->getSensorOffset_mm();
				mReferenceHeight_mm = pDlg->getReferenceHeight_mm();

				if (!variables->set("canopy_height_mm", canopy_height_mm))
					variables->add("canopy_height_mm", canopy_height_mm);

				if (!variables->set("sensor_offset_mm", sensor_offset_mm))
					variables->add("sensor_offset_mm", sensor_offset_mm);

				if (!variables->set("reference_height_mm", mReferenceHeight_mm))
					variables->add("reference_height_mm", mReferenceHeight_mm);

				pDlg->deleteLater();
			}

			mHeight_mm = static_cast<uint32_t>(aboveCanopy_mm) + canopy_height_mm + sensor_offset_mm;

			if (!mX_NeedsInitialization && !mY_NeedsInitialization)
			{
				mZ_mm = nRFM::compute_dolly_height_mm(mHeight_mm, mX_mm, mY_mm, mReferenceHeight_mm);

				if (!rfb::withinMeasurementHeight(mZ_mm))
				{
					QString msg = "Above Canopy Height Error: Computed z_mm, ";
					msg += QString::number(mZ_mm);
					msg += ", is outside of the measurement boundary.";

					QMessageBox mb(QMessageBox::Critical, "SpiderCam Experiment State Error", msg);
					mb.exec();

					return false;
				}

				mACH_NeedsInitialization = false;
			}
			else
			{
				mACH_NeedsInitialization = true;
			}

			mZ_NeedsInitialization = false;
		}
		else if (pos.contains("height agl (mm)"))
		{
			double aboveGround_mm = pos["height agl (mm)"].get<double>();

			int32_t sensor_offset_mm = -10000;
			int32_t reference_height_mm = -10000;

			auto variables = mVariables.lock();

			if (variables->contains("sensor_offset_mm"))
				variables->get("sensor_offset_mm", sensor_offset_mm);

			if (variables->contains("reference_height_mm"))
				variables->get("reference_height_mm", reference_height_mm);

			if (sensor_offset_mm == -10000)
			{
				cAboveGroundHeightDlg* pDlg = new cAboveGroundHeightDlg();

				if (sensor_offset_mm != -10000)
					pDlg->setSensorOffset_mm(sensor_offset_mm);
				pDlg->setReferenceHeight_mm(reference_height_mm);

				auto result = pDlg->exec();

				if (result == QDialog::Rejected)
				{
					pDlg->deleteLater();
					return false;
				}

				sensor_offset_mm = pDlg->getSensorOffset_mm();
				reference_height_mm = pDlg->getReferenceHeight_mm();

				if (!variables->set("sensor_offset_mm", sensor_offset_mm))
					variables->add("sensor_offset_mm", sensor_offset_mm);

				if (!variables->set("reference_height_mm", reference_height_mm))
					variables->add("reference_height_mm", reference_height_mm);

				pDlg->deleteLater();
			}

			mHeight_mm = static_cast<uint32_t>(aboveGround_mm) + sensor_offset_mm;

			if (!mX_NeedsInitialization && !mY_NeedsInitialization)
			{
				mZ_mm = nRFM::compute_dolly_height_mm(mHeight_mm, mX_mm, mY_mm, reference_height_mm);
				if (!rfb::withinMeasurementHeight(mZ_mm))
				{
					QString msg = "Above Ground Height Error: Computed z_mm, ";
					msg += QString::number(mZ_mm);
					msg += ", is outside of the measurement boundary.";

					QMessageBox mb(QMessageBox::Critical, "SpiderCam Experiment State Error", msg);
					mb.exec();

					return false;
				}

				mAGH_NeedsInitialization = false;
			}
			else
			{
				mAGH_NeedsInitialization = true;
			}

			mZ_NeedsInitialization = false;
		}
		else if (pos.contains("z (mm)"))
		{
			mZ_mm = static_cast<uint32_t>(pos["z (mm)"].get<double>());

			if (!rfb::withinMeasurementHeight(mZ_mm))
			{
				QString msg = "Spidercam Movement Error: The desired z_mm, ";
				msg += QString::number(mZ_mm);
				msg += ", is outside of the measurement boundary.";

				QMessageBox mb(QMessageBox::Critical, "SpiderCam Experiment State Error", msg);
				mb.exec();

				return false;
			}
		}
		else if (pos.contains("z (m)"))
		{
			mZ_mm = static_cast<uint32_t>(pos["z (m)"].get<double>() * nConstants::M_TO_MM);

			if (!rfb::withinMeasurementHeight(mZ_mm))
			{
				QString msg = "Spidercam Movement Error: The desired z_mm, ";
				msg += QString::number(mZ_mm);
				msg += ", is outside of the measurement boundary.";

				QMessageBox mb(QMessageBox::Critical, "SpiderCam Experiment State Error", msg);
				mb.exec();

				return false;
			}
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
	mErrorDetected = false;
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

	if (mACH_NeedsInitialization)
	{
		mZ_mm = nRFM::compute_dolly_height_mm(mHeight_mm, mX_mm, mY_mm, mReferenceHeight_mm);
	}

	if (mAGH_NeedsInitialization)
	{
		mZ_mm = nRFM::compute_dolly_height_mm(mHeight_mm, mX_mm, mY_mm, mReferenceHeight_mm);
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
	iSpidercamController& controller, uint32_t tolerance_mm)
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
	mErrorDetected = false;
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


