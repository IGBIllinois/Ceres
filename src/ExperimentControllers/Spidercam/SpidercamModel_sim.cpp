
#include "SpidercamModel_sim.hpp"

namespace
{
    const uint64_t	DOLLY_CONNECTED = 0x00'00'00'00'00'00'00'01;
    const uint64_t	CONSOLE_CONNECTED = 0x00'00'00'00'00'00'00'04;
    const uint64_t	IP_CAMERA_CONNECTED = 0x00'00'00'00'00'00'00'08;
    const uint64_t	SAFETY_CIRCUIT_RESTARTED = 0x00'00'00'00'00'00'00'10;
    const uint64_t	ACTIVATED = 0x00'00'00'00'00'00'00'20;
    const uint64_t	DOLLY_POS_KNOWN = 0x00'00'00'00'00'00'00'40;
    const uint64_t	PLACE_KNOWN = 0x00'00'00'00'00'00'00'80;

    const uint64_t	BORDER_KNOWN = 0x00'00'00'00'00'00'01'00;
    const uint64_t	FIELD_KNOWN = 0x00'00'00'00'00'00'02'00;
    const uint64_t	SETPOINT_GEN_ENABLED = 0x00'00'00'00'00'00'04'00;
    const uint64_t	CONSOLE_ENABLED = 0x00'00'00'00'00'00'08'00;
    const uint64_t	OBSTACLE_LESS_THAN_2000mm = 0x00'00'00'00'00'00'20'00;
    const uint64_t	OBSTACLE_LESS_THAN_1500mm = 0x00'00'00'00'00'00'40'00;
    const uint64_t	OBSTACLE_LESS_THAN_1000mm = 0x00'00'00'00'00'00'80'00;

    const uint64_t	OBSTACLE_LESS_THAN_500mm = 0x00'00'00'00'00'01'00'00;
    const uint64_t	BUSY = 0x00'00'00'00'00'02'00'00;
    const uint64_t	DONE = 0x00'00'00'00'00'04'00'00;
    const uint64_t	MOVING = 0x00'00'00'00'00'08'00'00;
    const uint64_t	CALIBRATED = 0x00'00'00'00'00'10'00'00;
    const uint64_t	CABLE_LEN_ADJ_REQUIRED = 0x00'00'00'00'00'20'00'00;
    const uint64_t	NEAR_BORDER = 0x00'00'00'00'00'40'00'00;
    const uint64_t	IN_POSITION = 0x00'00'00'00'00'80'00'00;

    const uint64_t	AT_CALIBRATION_POSITION = 0x00'00'00'00'01'00'00'00;
    const uint64_t	AT_CORRECTION_POSITION = 0x00'00'00'00'02'00'00'00;
    const uint64_t	IN_SCRIPT_MODE = 0x00'00'00'00'04'00'00'00;
    const uint64_t	IN_INTERACTIVE_MODE = 0x00'00'00'00'08'00'00'00;
    const uint64_t	IN_ERROR = 0x00'00'00'00'80'00'00'00;

    const uint64_t	DOLLY_BATTERY_MASK = 0x00'00'7F'80'00'00'00'00;

    const uint64_t BASE_FLAGS = DOLLY_CONNECTED || CONSOLE_CONNECTED 
        || SAFETY_CIRCUIT_RESTARTED || ACTIVATED || DOLLY_POS_KNOWN || PLACE_KNOWN
        || BORDER_KNOWN || FIELD_KNOWN || SETPOINT_GEN_ENABLED || CONSOLE_ENABLED
        || CALIBRATED;

}

cSpidercamModel_sim::cSpidercamModel_sim(QObject* parent)
:
    cSpidercamModel(parent)
{
    mTargetX_mm = -1;
    mTargetY_mm = -1;
    mTargetZ_mm = -1;

    mObstacleLessThan2000mm = false;
    mObstacleLessThan1500mm = false;
    mObstacleLessThan1000mm = false;
    mObstacleLessThan500mm = false;
    mDone = false;
    mMoving = false;
    mInPosition = false;
    mDollyPositionKnown = false;
    mConsoleEnabled = false;
    mInScriptMode = true;
    mInInteractiveMode = false;

    mBatteryLevel_pct = 100;

//    mFlags = BASE_FLAGS | DONE | IN_POSITION | IN_SCRIPT_MODE;

//    mTime_ms = 0;

    mpProxy = std::make_unique<cSpidercamCtrlProxy<cSpidercamModel_sim>>(this);
}

cSpidercamModel_sim::~cSpidercamModel_sim()
{}

bool cSpidercamModel_sim::startCommunications()
{
    mLastUpdateTime = std::chrono::high_resolution_clock::now();

    mBusy = false;
    mInError = false;
    mDone = true;
    mMoving = false;
    mObstacleLessThan2000mm = false;
    mObstacleLessThan1500mm = false;
    mObstacleLessThan1000mm = false;
    mObstacleLessThan500mm = false;
    mInPosition = true;
    mInScriptMode = true;

    emit positionChanged(mCurrentPosition);
    emit busyChanged(mBusy);
    emit movingChanged(mMoving);
    emit batteryLevelChanged(mBatteryLevel_pct);
    emit inPositionStateChanged(mInPosition);

    emit statusMessage("Ready");

    emit updateControllerConnection(true);

    return true;
}

void cSpidercamModel_sim::stopCommunications()
{}

cExperimentState* cSpidercamModel_sim::createState(const std::string& type, const nlohmann::json& expDoc)
{
    if (type == "movement")
        return new cSpidercamExperimentState_Movement(mCurrentPosition, *(mpProxy.get()), mPositionTolerance_mm);

    return cExperimentControlModel::createState(type, expDoc);
}

void cSpidercamModel_sim::configure(const nlohmann::json& jsonCfg)
{
    cSpidercamModel::configure(jsonCfg);

    mX_mm = (mLimits.maxX_mm + mLimits.minX_mm) / 2;
    mY_mm = (mLimits.maxY_mm + mLimits.minY_mm) / 2;
    mZ_mm = mLimits.maxHeight_mm - 1000;

    QString msg("Connection to Spidercam established.");

    emit statusMessage(msg);
}

bool cSpidercamModel_sim::systemReady() const
{
    return true;
}

bool cSpidercamModel_sim::isBusy() const
{
    return mBusy;
}

bool cSpidercamModel_sim::isConsoleConnected() const
{
    return mConsoleConnected;
}

bool cSpidercamModel_sim::isMoving() const
{
    return mMoving;

}

bool cSpidercamModel_sim::isSetPointEnabled() const
{
    return mSetPointEnabled;
}

bool cSpidercamModel_sim::isInScriptMode() const
{
    return mInScriptMode;
}

bool cSpidercamModel_sim::isInError() const
{
    return mInError;
}

const spidercam::sPosition_1_t& cSpidercamModel_sim::getLastKnownPosition() const
{
    return mCurrentPosition;
}

bool cSpidercamModel_sim::requestStop()
{
    mTargetX_mm = -1;
    mTargetY_mm = -1;
    mTargetZ_mm = -1;
    return true;
}

bool cSpidercamModel_sim::sendRequestNewPosition(double x_mm, double y_mm, double z_mm, double height_mm,
    uint32_t speed_mmps, float pan_deg, float tilt_deg, float roll_deg)
{
    if ((x_mm < mLimits.minX_mm) || (x_mm > mLimits.maxX_mm))
        return false;

    if ((y_mm < mLimits.minY_mm) || (y_mm > mLimits.maxY_mm))
        return false;

    if ((z_mm < mLimits.minHeight_mm) || (z_mm > mLimits.maxHeight_mm))
        return false;

    if ((speed_mmps < 1) || (speed_mmps > mMaxSpeed_mmps))
        return false;

    double deltaX = x_mm - mX_mm;
    double deltaY = y_mm - mY_mm;
    double deltaZ = z_mm - mZ_mm;

    double d2 = deltaX * deltaX + deltaY * deltaY + deltaZ * deltaZ;

    if (d2 < (mPositionTolerance_mm* mPositionTolerance_mm))
    {
        return true;
    }

    double d = sqrt(d2);
    double t = d / static_cast<double>(speed_mmps);

    mVx_mmps = deltaX / t;
    mVy_mmps = deltaY / t;
    mVz_mmps = deltaZ / t;

    if (mVx_mmps < -mMaxSpeed_mmps) mVx_mmps = -mMaxSpeed_mmps;
    if (mVx_mmps > mMaxSpeed_mmps) mVx_mmps = mMaxSpeed_mmps;

    if (mVy_mmps < -mMaxSpeed_mmps) mVy_mmps = -mMaxSpeed_mmps;
    if (mVy_mmps > mMaxSpeed_mmps) mVy_mmps = mMaxSpeed_mmps;

    if (mVz_mmps < -mMaxSpeed_mmps) mVz_mmps = -mMaxSpeed_mmps;
    if (mVz_mmps > mMaxSpeed_mmps) mVz_mmps = mMaxSpeed_mmps;

    mTargetX_mm = x_mm;
    mTargetY_mm = y_mm;
    mTargetZ_mm = z_mm;

    mDone = false;
    mInPosition = false;
    mMoving = true;

    return true;
}

void cSpidercamModel_sim::update()
{
    auto now = std::chrono::high_resolution_clock::now();
    auto diff = now - mLastUpdateTime;

    double dt_sec = duration_cast<std::chrono::milliseconds>(diff).count() / 1000.0;

    mLastUpdateTime = now;

    /*
     * Make sure the dolly always stays within bounds
     */
    if (!mMoving)
    {
        if (mX_mm <= mLimits.minX_mm)
        {
            reset_x_to_pos(mLimits.minX_mm);
            mDone = true;
            mInPosition = true;
            mMoving = false;
            update_flags();
        }

        if (mX_mm >= mLimits.maxX_mm)
        {
            reset_x_to_pos(mLimits.maxX_mm);
            mDone = true;
            mInPosition = true;
            mMoving = false;
            update_flags();
        }

        if (mY_mm <= mLimits.minY_mm)
        {
            reset_y_to_pos(mLimits.minY_mm);
            mDone = true;
            mInPosition = true;
            mMoving = false;
            update_flags();
        }

        if (mY_mm >= mLimits.maxY_mm)
        {
            reset_y_to_pos(mLimits.maxY_mm);
            mDone = true;
            mInPosition = true;
            mMoving = false;
            update_flags();
        }

        if (mZ_mm <= mLimits.minHeight_mm)
        {
            reset_z_to_pos(mLimits.minHeight_mm);
            mDone = true;
            mInPosition = true;
            mMoving = false;
            update_flags();
        }

        if (mZ_mm >= mLimits.maxHeight_mm)
        {
            reset_z_to_pos(mLimits.maxHeight_mm);
            mDone = true;
            mInPosition = true;
            mMoving = false;
            update_flags();
        }
    }
    else
    {

        bool x_moving = mTargetX_mm > 0;
        bool y_moving = mTargetY_mm > 0;
        bool z_moving = mTargetZ_mm > 0;

        // Move in the X-Axis...
        if (x_moving)
        {
            mX_mm += (mVx_mmps * dt_sec);

            if (mVx_mmps > 1.0)
            {
                if (mX_mm >= (mTargetX_mm - mPositionTolerance_mm))
                {
                    reset_x_to_pos(mTargetX_mm);
                    x_moving = false;
                }
            }
            else if (mVx_mmps < -1.0)
            {
                if (mX_mm <= (mTargetX_mm + mPositionTolerance_mm))
                {
                    reset_x_to_pos(mTargetX_mm);
                    x_moving = false;
                }
            }
            else
            {
                reset_x_to_pos(mTargetX_mm);
                x_moving = false;
            }

            // Check to make sure we don't go out-of-bound
            if (mX_mm <= mLimits.minX_mm)
            {
                reset_x_to_pos(mLimits.minX_mm);
                x_moving = false;
            }

            if (mX_mm >= mLimits.maxX_mm)
            {
                reset_x_to_pos(mLimits.maxX_mm);
                x_moving = false;
            }
        }
        else
        {
            mVx_mmps = 0.0;
        }

        // Move in the Y-Axis...
        if (y_moving)
        {
            mY_mm += (mVy_mmps * dt_sec);

            if (mVy_mmps > 1.0)
            {
                if (mY_mm >= (mTargetY_mm - mPositionTolerance_mm))
                {
                    reset_y_to_pos(mTargetY_mm);
                    y_moving = false;
                }
            }
            else if (mVy_mmps < -1.0)
            {
                if (mY_mm <= (mTargetY_mm - mPositionTolerance_mm))
                {
                    reset_y_to_pos(mTargetY_mm);
                    y_moving = false;
                }
            }
            else
            {
                reset_y_to_pos(mTargetY_mm);
                y_moving = false;
            }

            // Check to make sure we don't go out-of-bound
            if (mY_mm <= mLimits.minY_mm)
            {
                reset_y_to_pos(mLimits.minY_mm);
                y_moving = false;
            }

            if (mY_mm >= mLimits.maxY_mm)
            {
                reset_y_to_pos(mLimits.maxY_mm);
                y_moving = false;
            }
        }
        else
        {
            mVy_mmps = 0.0;
        }

        // Move in the Z-Axis...
        if (z_moving)
        {
            mZ_mm += (mVz_mmps * dt_sec);

            if (mVz_mmps > 1.0)
            {
                if (mZ_mm >= (mTargetZ_mm - mPositionTolerance_mm))
                {
                    reset_z_to_pos(mTargetZ_mm);
                    z_moving = false;
                }
            }
            else if (mVz_mmps < -1.0)
            {
                if (mZ_mm <= (mTargetZ_mm + mPositionTolerance_mm))
                {
                    reset_z_to_pos(mTargetZ_mm);
                    z_moving = false;
                }
            }
            else
            {
                reset_z_to_pos(mTargetZ_mm);
                z_moving = false;
            }

            // Check to make sure we don't go out-of-bound
            if (mZ_mm <= mLimits.minHeight_mm)
            {
                reset_z_to_pos(mLimits.minHeight_mm);
                z_moving = false;
            }

            if (mZ_mm >= mLimits.maxHeight_mm)
            {
                reset_z_to_pos(mLimits.maxHeight_mm);
                z_moving = false;
            }
        }
        else
        {
            mVz_mmps = 0.0;
        }

        mObstacleLessThan2000mm = mZ_mm < 2000;
        mObstacleLessThan1500mm = mZ_mm < 1500;
        mObstacleLessThan1000mm = mZ_mm < 1000;
        mObstacleLessThan500mm = mZ_mm < 500;

        mMoving = x_moving || y_moving || z_moving;
        if (!mMoving)
        {
            mDone = true;
            mInPosition = true;
            mMoving = false;
        }

        update_flags();
    }

    if (mTimer.elapsed())
    {
        mCurrentPosition.timestamp = std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch()).count();

        mCurrentPosition.X_mm = mX_mm;
        mCurrentPosition.Y_mm = mY_mm;
        mCurrentPosition.Z_mm = mZ_mm;
        mCurrentPosition.height_mm = mZ_mm;
        mCurrentPosition.speed_mmps = sqrt(mVx_mmps*mVx_mmps + mVy_mmps*mVy_mmps + mVz_mmps*mVz_mmps);
        mCurrentPosition.pan_deg = 0;
        mCurrentPosition.pan_speed_dps = 0;
        mCurrentPosition.pitch_deg = 0;
        mCurrentPosition.roll_deg = 0;
        mCurrentPosition.roll_gimbal_deg = 0;
        mCurrentPosition.tilt_deg = 0;
        mCurrentPosition.tilt_speed_dps = 0;
        mCurrentPosition.focus = 0;
        mCurrentPosition.iris = 0;
        mCurrentPosition.zoom = 0;

        update_flags();
    }

    updateExperimentStateMachine();
}

void cSpidercamModel_sim::reset_x_to_pos(long pos)
{
    mX_mm = pos;
    mVx_mmps = 0.0;
    mTargetX_mm = -1;
}

void cSpidercamModel_sim::reset_y_to_pos(long pos)
{
    mY_mm = pos;
    mVy_mmps = 0.0;
    mTargetY_mm = -1;
}

void cSpidercamModel_sim::reset_z_to_pos(long pos)
{
    mZ_mm = pos;
    mVz_mmps = 0.0;
    mTargetZ_mm = -1;
}

void cSpidercamModel_sim::update_flags()
{
/*
    mFlags = BASE_FLAGS;

    if (mUSL_2000_mm)
        mFlags |= USL_2000;

    if (mUSL_1500_mm)
        mFlags |= USL_1500;

    if (mUSL_1000_mm)
        mFlags |= USL_1000;

    if (mUSL_500_mm)
        mFlags |= USL_500;

    if (mDone)
        mFlags |= DONE;

    if (mMoving)
        mFlags |= MOVING;

    if (mInPosition)
        mFlags |= IN_POSITION;

    if (mInScriptM)
        mFlags |= IN_SCRIPT_MODE;

    mFlags |= BATTERY;
*/
}

