
#include "SpidercamModel_sim.hpp"


cSpidercamModel_sim::cSpidercamModel_sim(QObject* parent)
:
    cSpidercamModel(parent)
{
    mTargetX_mm = -1;
    mTargetY_mm = -1;
    mTargetZ_mm = -1;

    mUSL_2000_mm = false;
    mUSL_1500_mm = false;
    mUSL_1000_mm = false;
    mUSL_500_mm = false;
    mDone = false;
    mMoving = false;
    mInPosition = false;
    mInCalibPos = false;
    mInCorrPos = false;
    mInScriptM = true;
    mInInterActM = false;

    mBatteryLevel = 225;

    mFlags = BASE_FLAGS | DONE | IN_POSITION | IN_SCRIPT_MODE;

    mTime_ms = 0;
}

cSpidercamModel_sim::~cSpidercamModel_sim()
{}

bool cSpidercamModel_sim::startCommunications()
{
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
//    if (type == "movement")
//        return new cSpidercamExperimentState_Movement(mCurrentPosition, mController, mPositionTolerance_mm);

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


void cSpidercamModel_sim::update()
{
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

        if (mX_mm >= MAX_X_mm)
        {
            reset_x_to_pos(MAX_X_mm);
            mDone = true;
            mInPosition = true;
            mMoving = false;
            update_flags();
        }

        if (mY_mm <= MIN_Y_mm)
        {
            reset_y_to_pos(MIN_Y_mm);
            mDone = true;
            mInPosition = true;
            mMoving = false;
            update_flags();
        }

        if (mY_mm >= MAX_Y_mm)
        {
            reset_y_to_pos(MAX_Y_mm);
            mDone = true;
            mInPosition = true;
            mMoving = false;
            update_flags();
        }

        if (mZ_mm <= MIN_Z_mm)
        {
            reset_z_to_pos(MIN_Z_mm);
            mDone = true;
            mInPosition = true;
            mMoving = false;
            update_flags();
        }

        if (mZ_mm >= MAX_Z_mm)
        {
            reset_z_to_pos(MAX_Z_mm);
            mDone = true;
            mInPosition = true;
            mMoving = false;
            update_flags();
        }
    }
    else
    {
        unsigned long now_ms = millis();

        float delta_sec = 0;

        // Check to see if the timer has overflowed...
        if (now_ms < mTime_ms)
        {
            unsigned long delta_ms = UINT64_MAX - mTime_ms;
            delta_ms += now_ms;
            delta_sec = static_cast<float>(delta_ms) * 0.001;
        }
        else
        {
            delta_sec = static_cast<float>(now_ms - mTime_ms) * 0.001;
        }

        mTime_ms = now_ms;

        bool x_moving = mTargetX_mm > 0;
        bool y_moving = mTargetY_mm > 0;
        bool z_moving = mTargetZ_mm > 0;

        // Move in the X-Axis...
        if (x_moving)
        {
            mX_mm += (mXspeed_mmps * delta_sec);

            if (mXspeed_mmps > 1.0)
            {
                if (mX_mm >= (mTargetX_mm - TOLERANCE_mm))
                {
                    reset_x_to_pos(mTargetX_mm);
                    x_moving = false;
                }
            }
            else if (mXspeed_mmps < -1.0)
            {
                if (mX_mm <= (mTargetX_mm + TOLERANCE_mm))
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
            if (mX_mm <= MIN_X_mm)
            {
                reset_x_to_pos(MIN_X_mm);
                x_moving = false;
            }

            if (mX_mm >= MAX_X_mm)
            {
                reset_x_to_pos(MAX_X_mm);
                x_moving = false;
            }
        }

        // Move in the Y-Axis...
        if (y_moving)
        {
            mY_mm += (mVy_mmps * delta_sec);

            if (mVy_mmps > 1.0)
            {
                if (mY_mm >= (mTargetY_mm - TOLERANCE_mm))
                {
                    reset_y_to_pos(mTargetY_mm);
                    y_moving = false;
                }
            }
            else if (mVy_mmps < -1.0)
            {
                if (mY_mm <= (mTargetY_mm - TOLERANCE_mm))
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
            if (mY_mm <= MIN_Y_mm)
            {
                reset_y_to_pos(MIN_Y_mm);
                y_moving = false;
            }

            if (mY_mm >= MAX_Y_mm)
            {
                reset_y_to_pos(MAX_Y_mm);
                y_moving = false;
            }
        }

        // Move in the Z-Axis...
        if (z_moving)
        {
            mZ_mm += (mZspeed_mmps * delta_sec);

            if (mZspeed_mmps > 1.0)
            {
                if (mZ_mm >= (mTargetZ_mm - TOLERANCE_mm))
                {
                    reset_z_to_pos(mTargetZ_mm);
                    z_moving = false;
                }
            }
            else if (mZspeed_mmps < -1.0)
            {
                if (mZ_mm <= (mTargetZ_mm + TOLERANCE_mm))
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
            if (mZ_mm <= MIN_Z_mm)
            {
                reset_z_to_pos(MIN_Z_mm);
                z_moving = false;
            }

            if (mZ_mm >= MAX_Z_mm)
            {
                reset_z_to_pos(MAX_Z_mm);
                z_moving = false;
            }
        }

        mUSL_2000_mm = mZ_mm < 2000;
        mUSL_1500_mm = mZ_mm < 1500;
        mUSL_1000_mm = mZ_mm < 1000;
        mUSL_500_mm = mZ_mm < 500;

        mMoving = x_moving || y_moving || z_moving;
        if (!mMoving)
        {
            mDone = true;
            mInPosition = true;
            mMoving = false;
        }

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
}

