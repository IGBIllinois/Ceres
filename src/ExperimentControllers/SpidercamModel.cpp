
#include "SpidercamModel.hpp"
#include "../Utilities/Constants.hpp"

#include <QMessageBox>

namespace
{
    uint32_t TOLERANCE_MM = 10;

    template <typename T>
    T abs_difference(T v1, T v2)
    {
        return (v2 > v1) ? v2 - v1 : v1 - v2;
    }
}

cSpidercamModel::cSpidercamModel()
{
    mPositionTolerance_mm = TOLERANCE_MM;
}

cSpidercamModel::~cSpidercamModel()
{

}

void cSpidercamModel::configure(const nlohmann::json& jsonCfg)
{
    std::string c2_ip;
    std::string c3_ip;
    uint16_t port = 0;

    double minX_mm = 0;
    double maxX_mm = 0;
    double minY_mm = 0;
    double maxY_mm = 0;
    double minHeight_mm = 0;
    double maxHeight_mm = 0;
    double maxSpeed_mmps = 0;

    try
    {
        c2_ip = static_cast<std::string>(jsonCfg["c2_ip"]);
        c3_ip = static_cast<std::string>(jsonCfg["c3_ip"]);
        port = jsonCfg["port"];

        minX_mm = jsonCfg["min X position (m)"] * nConstants::M_TO_MM;
        maxX_mm = jsonCfg["max X position (m)"] * nConstants::M_TO_MM;
        minY_mm = jsonCfg["min Y position (m)"] * nConstants::M_TO_MM;
        maxY_mm = jsonCfg["max Y position (m)"] * nConstants::M_TO_MM;
        minHeight_mm = jsonCfg["min height (m)"] * nConstants::M_TO_MM;
        maxHeight_mm = jsonCfg["max height (m)"] * nConstants::M_TO_MM;
        maxSpeed_mmps = jsonCfg["max speed (m/s)"] * nConstants::M_TO_MM;

        double interval_ms = jsonCfg["update interval (ms)"];
        if ((interval_ms <= 0) || (interval_ms > 60000.0))
        {
            QString str = "Invalid \"update interval (ms)\" in the \"spidercam\" configuration.\n";
            str.append("The interval must be in the range >0 to <60,000.  The value will be ignored.");
            QMessageBox msg(QMessageBox::Critical, "Configuration Error", str);
            msg.exec();
        }
        else
        {
            mTimer.interval_ms(static_cast<uint32_t>(interval_ms));
        }

        mPositionTolerance_mm = jsonCfg["position tolerance (cm)"] * nConstants::CM_TO_MM;
    }
    catch (const std::exception& e)
    {
        QString str = "Error in the \"spidercam\" configuration: ";
        str.append(e.what());
        QMessageBox msg(QMessageBox::Critical, "Configuration Error", str);
        msg.exec();
        return;
    }

    spidercam::sWorkingDimensions limits = { static_cast<uint32_t>(minX_mm), static_cast<uint32_t>(maxX_mm), 
        static_cast<uint32_t>(minY_mm), static_cast<uint32_t>(maxY_mm),
        static_cast<uint32_t>(minHeight_mm), static_cast<uint32_t>(maxHeight_mm) };

    mController.setSafetyLimits(limits, maxSpeed_mmps);

    QString msg("Trying to establishing connection to Spidercam at ");
    msg.append(c2_ip.c_str());
    msg.append("...");

    emit statusMessage(msg);

/*
    if (!mController.try_to_connect(c2_ip, port))
    {
        QMessageBox msg(QMessageBox::Critical, "Spidercam Error", "Could not establish required command connection to Spidercam C2 computer!");
        msg.exec();

#ifdef NDEBUG
        exit(EXIT_FAILURE);
#endif // NDEBUG
    }
*/

    mController.clearIncomingBuffer();

    if (mController.isConnected())
    {
        mController.requestCurrentPosition();
    }

    mCurrentPosition = mController.getLastKnownPosition();
}

void cSpidercamModel::loadExperiment(const nlohmann::json& expDoc)
{
    if (mRunning)
        return;

    for (std::size_t i = 0; i < mExperiment.size(); ++i)
    {
        delete mExperiment[i];
        mExperiment[i] = nullptr;
    }

    mExperiment.clear();
    mExperiment.push_back(new cExperimentState_Dummy());

    for (auto entry : expDoc)
    {
        std::string type = entry["type"];

        cExperimentState* pState = nullptr;

        if (type == "delay")
        {
            pState = new cExperimentState_Delay();
        }
        else if (type == "movement")
        {
            pState = new cSpidercamExperimentState_Movement(mCurrentPosition, mController, TOLERANCE_MM);
        }

        if (pState)
        {
            pState->configure(entry);
            mExperiment.push_back(pState);
        }
    }
}

void cSpidercamModel::writeDataHeader(cBlockDataFile& file)
{

}

void cSpidercamModel::update()
{
    if (mController.checkForReply())
    {
        mController.readReply();
    }

    const auto& pos = mController.getLastKnownPosition();

    if (spidercam::hasPositionChanged(pos, mCurrentPosition, mPositionTolerance_mm))
    {
        mCurrentPosition = pos;
        emit positionChanged(mCurrentPosition);
    }

    mDollyConnected = mController.isDollyConnected();
    mConsoleConnected = mController.isConsoleConnected();
    mActivated = mController.isActivated();
    mDollyPositionKnown = mController.isDollyPositionKnown();
    mConsoleEnabled = mController.isConsoleEnabled();

    mInError = mController.isInError();
    mBusy = mController.isBusy();
    mMoving = mController.isMoving();
    mInPosition = mController.isInPosition();
    mDone = mController.isDone();
    mBatteryLevel_pct = mController.getBatteryLevel_pct();
    mObstacleLessThan2000mm = mController.isObstacleLessThan2000mm();
    mObstacleLessThan1500mm = mController.isObstacleLessThan1500mm();
    mObstacleLessThan1000mm = mController.isObstacleLessThan1000mm();
    mObstacleLessThan500mm = mController.isObstacleLessThan500mm();

    if (mBusy.HasChanged())
        emit busyChanged(mBusy);

    if (mMoving.HasChanged())
        emit movingChanged(mMoving);

    if (mInPosition.HasChanged())
        emit inPositionStateChanged(mInPosition);

    if (mBatteryLevel_pct.HasChanged())
        emit batteryLevelChanged(mBatteryLevel_pct);

    if (mObstacleLessThan2000mm)
    {
        if (mObstacleLessThan1500mm)
        {
            if (mObstacleLessThan1000mm)
            {
                if (mObstacleLessThan500mm)
                {
                    if (mObstacleLessThan500mm.IsRising())
                        emit obstacleDistanceChanged(0.0);
                }
                else if (mObstacleLessThan1000mm.IsRising() || mObstacleLessThan500mm.IsFalling())
                    emit obstacleDistanceChanged(500.0);
            }
            else if (mObstacleLessThan1500mm.IsRising() || mObstacleLessThan1000mm.IsFalling())
                emit obstacleDistanceChanged(1000.0);
        }
        else if (mObstacleLessThan2000mm.IsRising() || mObstacleLessThan1500mm.IsFalling())
            emit obstacleDistanceChanged(1500.0);
    }
    else if (mObstacleLessThan2000mm.IsFalling())
        emit obstacleDistanceChanged(-1.0);


    bool readyForMotion = mDollyConnected && mConsoleConnected && mController.isSetPointEnabled() && !mBusy && !mInError;
    if (mTimer.elapsed())
    {
        mController.sendRequestForCurrentPosition();
    }

    updateExperimentStateMachine();
}

