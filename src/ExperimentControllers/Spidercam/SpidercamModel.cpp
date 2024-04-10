
#include "SpidercamModel.hpp"
#include "../../Utilities/Constants.hpp"
#include "ExperimentTypes.hpp"

namespace
{
    uint32_t TOLERANCE_MM = 10;
}

cSpidercamModel::cSpidercamModel(QObject* parent)
:
    cExperimentControlModel(parent),
    mDollyConnected(false), mConsoleConnected(false), mActivated(false),
    mDollyPositionKnown(false), mConsoleEnabled(false), mInInteractiveMode(false),
    mInScriptMode(false),
    mController(this)

{
    QObject::connect(&mController, &cSpidercamController::connectionStateChange,
                this, &cSpidercamModel::onConnectionStateChange);

    mPositionTolerance_mm = TOLERANCE_MM;
}

cSpidercamModel::~cSpidercamModel()
{

}

bool cSpidercamModel::startCommunications()
{
    if (!mController.hasRemoteEndpoint())
    {
        QString msg("Cannot establishing connection to Spidercam.");

        emit statusMessage(msg);

        return false;
    }

    QString msg("Establishing connection to Spidercam at ");
    msg += mController.remoteEndpoint();
    msg.append("...");

    emit statusMessage(msg);

    bool result = mController.startCommunications();

    if (!result)
    {
        QString msg("Failed to connect to Spidercam at ");
        msg += mController.remoteEndpoint();
        msg.append(".");

        emit statusMessage(msg);

        return false;
    }

    mController.clearIncomingBuffer();

    if (mController.isConnected())
    {
        mController.requestCurrentPosition();
        mCurrentPosition = mController.getLastKnownPosition();
        updateState();

        emit positionChanged(mCurrentPosition);
        emit busyChanged(mBusy);
        emit movingChanged(mMoving);
        emit batteryLevelChanged(mBatteryLevel_pct);
        emit inPositionStateChanged(mInPosition);

        updateObstacleDistance();

        emit statusMessage("Ready");

        emit updateControllerConnection(true);
    }

    return result;
}

void cSpidercamModel::stopCommunications()
{
    emit updateControllerConnection(false);
    mController.stopCommunications();
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
            emit errorMessage("Configuration Error", str);
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
        emit errorMessage("Configuration Error", str);
        return;
    }

    spidercam::sWorkingDimensions limits = { static_cast<uint32_t>(minX_mm), static_cast<uint32_t>(maxX_mm), 
        static_cast<uint32_t>(minY_mm), static_cast<uint32_t>(maxY_mm),
        static_cast<uint32_t>(minHeight_mm), static_cast<uint32_t>(maxHeight_mm) };

    mController.setSafetyLimits(limits, maxSpeed_mmps);

    QString msg("Testing connection to Spidercam at ");
    msg.append(c2_ip.c_str());
    msg.append("...");

    emit statusMessage(msg);

    if (!mController.try_to_connect(c2_ip, port, c3_ip))
    {
        emit errorMessage("Spidercam Error", "Could not establish required command connection to Spidercam C2 computer!");

#ifdef NDEBUG
        exit(EXIT_FAILURE);
#endif // NDEBUG
    }

    {
        QString msg("Connection to Spidercam established.");

        emit statusMessage(msg);
    }
}

bool cSpidercamModel::systemReady() const
{
    if (!mController.isConnected())
    {
        QString str = "The C3 computer is not connected to the Spidercam C2 computer.\n";
        str += "Unable to run any experiment until connection is re-established.";
        emit errorMessage("Error", str);

        return false;
    }

    if (!mInScriptMode && !mRunning)
    {
        QString str = "Make sure the C2 computer is in remote mode.\n";
        str += "Please enter remote mode.";
        emit warningMessage("Message", str);

        return false;
    }

    return true;
}

cExperimentState* cSpidercamModel::createState(const std::string& type, const nlohmann::json& expDoc)
{
    if (type == "movement")
        return new cSpidercamExperimentState_Movement(mCurrentPosition, mController, mPositionTolerance_mm);

    return cExperimentControlModel::createState(type, expDoc);
}

void cSpidercamModel::startExperiment()
{
    if (!mInScriptMode && !mRunning)
    {
        QString str = "Make sure the C2 computer is in remote mode.\n";
        str += "Please enter remote mode.";
        emit warningMessage("Message", str);
    }

    cExperimentControlModel::startExperiment();
}


void cSpidercamModel::enableDataRecording(cBlockDataFileWriter& file)
{
    mSerializer.attach(&file);
}

void cSpidercamModel::disableDataRecording()
{
    mSerializer.detach();
}

void cSpidercamModel::writeDataHeader()
{}

void cSpidercamModel::stopDataRecording()
{}

void cSpidercamModel::onConnectionStateChange(bool connected)
{
    emit updateControllerConnection(connected);
}

void cSpidercamModel::update()
{
    if (!mController.isConnected()) return;

    if (mController.checkForReply() || true)
    {
        mController.readReply();

        mLastReplyWasError = mController.lastReplyWasError();

        if (mLastReplyWasError.IsRising())
        {
            const auto& error = mController.getLastReportedError();

            QString msg = "SpiderCam Error Response: Group ID = ";
            msg += QString::fromStdString(error.groupID);
            msg += ", Device ID = ";
            msg += QString::fromStdString(error.deviceID);

            switch (error.errorID)
            {
            case 1000:
                msg += ", Error = The data format sent by C3 is incorrect.";
                break;
            case 1001:
                msg += ", Error = Cannot convert \"CO\" position data.";
                break;
            case 1002:
                msg += ", Error = Command not accepted.";
                break;
            case 1003:
                msg += ", Error = Invalid data from C3.";
                break;
            case 1004:
                msg += ", Error = Found points outside the current border.";
                break;
            case 1005:
                msg += ", Error = Incorrect command for update fieldmap.";
                break;
            case 1006:
                msg += ", Error = While updating fieldmap - update denied.";
                break;
            case 1007:
                msg += ", Error = Incorrect data for \"Update fieldmap\".";
                break;
            case 1008:
                msg += ", Error = C2 not in script mode.";
                break;
            case 1009:
                msg += ", Error = Check Activate, TwinSafeRestart, and Motors.";
                break;
            case 1010:
                msg += ", Error = C1 is busy or moving.";
                break;
            case 1011:
                msg += ", Error = Unknown command.";
                break;
            case 1012:
                msg += ", Error = Exception while reading request.";
                break;
            case 1013:
                msg += ", Error = Header is not C3.";
                break;
            case 1014:
                msg += ", Error = Data couldn't be interpreted as XML.";
                break;
            case 1015:
                msg += ", Error = Exception while reading XML header.";
                break;
            case 1016:
                msg += ", Error = NullReferenceException while sending actual position to C3.";
                break;
            case 1017:
                msg += ", Error = SocketException while sending actual position to C3.";
                break;
            case 7000:
                msg += ", Error = Either pan or tilt is not in the range.";
                break;
            case 7001:
                msg += ", Error = Velocity is zero.";
                break;
            case 7002:
                msg += ", Error = Check zoom, focus, and iris.";
                break;
            case 7003:
                msg += ", Error = SG requested height is lower than 2m or higher than 15m.";
                break;
            default:
                msg += ", Error ID = ";
                msg += QString::number(error.errorID);
                break;
            }

            emit statusMessage(msg);
        }

        if (mLastReplyWasError.IsFalling())
        {
            emit statusMessage("");
        }
    }

    const auto& pos = mController.getLastKnownPosition();

    if (spidercam::hasPosSpeedChanged(pos, mCurrentPosition, mPositionTolerance_mm))
    {
        mCurrentPosition = pos;
        emit positionChanged(mCurrentPosition);

        if (mRecording && mSerializer)
        {
            mSerializer.write(mCurrentPosition);
        }
    }

    updateState();

    if (mBusy.HasChanged())
        emit busyChanged(mBusy);

    if (mMoving.HasChanged())
        emit movingChanged(mMoving);

    if (mInPosition.HasChanged())
        emit inPositionStateChanged(mInPosition);

    if (mBatteryLevel_pct.HasChanged())
        emit batteryLevelChanged(mBatteryLevel_pct);

    if (mInScriptMode.HasChanged())
        emit inScriptMode(mInScriptMode);

    updateObstacleDistance();

    bool readyForMotion = mDollyConnected && mConsoleConnected && mController.isSetPointEnabled() && !mBusy && !mInError;
    if (mTimer.elapsed())
    {
        mController.sendRequestForCurrentPosition();
    }

    updateExperimentStateMachine();
}


void cSpidercamModel::updateState()
{
    mDollyConnected = mController.isDollyConnected();
    mConsoleConnected = mController.isConsoleConnected();
    mActivated = mController.isActivated();
    mDollyPositionKnown = mController.isDollyPositionKnown();
    mConsoleEnabled = mController.isConsoleEnabled();
    mInInteractiveMode = mController.isInInteractiveMode();
    mInScriptMode = mController.isInScriptMode();

    mInError = mController.isInError();
    mBusy = mController.isBusy();
    mMoving = mController.isMoving();
    mInPosition = mController.isInPosition();
    mDone = mController.isDone();
    mBatteryLevel_pct = mController.getBatteryLevel_pct();

    mObstacleLessThan500mm = mController.isObstacleLessThan500mm();
    mObstacleLessThan1000mm = mController.isObstacleLessThan1000mm() || mObstacleLessThan500mm;
    mObstacleLessThan1500mm = mController.isObstacleLessThan1500mm() || mObstacleLessThan1000mm;
    mObstacleLessThan2000mm = mController.isObstacleLessThan2000mm() || mObstacleLessThan1500mm;
}

void cSpidercamModel::updateObstacleDistance()
{
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
}
