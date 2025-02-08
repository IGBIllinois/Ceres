
#include "SpidercamModel_sim.hpp"


cSpidercamModel_sim::cSpidercamModel_sim(QObject* parent)
:
    cSpidercamModel(parent)
{}

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

    QString msg("Connection to Spidercam established.");

    emit statusMessage(msg);
}

bool cSpidercamModel_sim::systemReady() const
{
    return true;
}


void cSpidercamModel_sim::update()
{

    updateExperimentStateMachine();
}

