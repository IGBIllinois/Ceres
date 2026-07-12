
#include "PlannerDataThread.hpp"
#include "ExperimentCtrlModel.hpp"
//#include "SensorModel.hpp"
//#include "../Sensors/SensorModel.hpp"

cPlannerDataThread::cPlannerDataThread()
:
    mpController(nullptr)
{
}

cPlannerDataThread::~cPlannerDataThread()
{
    stop();
}

bool cPlannerDataThread::startCommunications()
{
    cDataThread::startCommunications();

    if (!mpController->startCommunications())
    {
        return false;
    }

    emit connectedToController();

    return true;
}

bool cPlannerDataThread::stopCommunications()
{
    emit disconnectedFromController();

    // Shutdown the network communications that are tied to this thread
    cDataThread::stopCommunications();

    mpController->stopCommunications();

    return true;
}

void cPlannerDataThread::updateAll()
{
    mpController->update();

    cDataThread::updateAll();
}


