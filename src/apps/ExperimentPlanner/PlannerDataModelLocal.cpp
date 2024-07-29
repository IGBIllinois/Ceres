
#include "PlannerDataModelLocal.hpp"
#include "SensorModel.hpp"
#include "ExperimentCtrlModel.hpp"
#include "ExperimentTypes.hpp"
#include "TimestampProvider.hpp"

#include <QtWidgets>

#include <filesystem>
#include <ctime>


cPlannerDataModelLocal::cPlannerDataModelLocal(QObject* parent)
:
    cPlannerDataModel(parent)
{
    mpHeartbeatTimer = new QTimer(this);
    connect(mpHeartbeatTimer, &QTimer::timeout, this, &cPlannerDataModelLocal::onHeartbeat);
}

cPlannerDataModelLocal::~cPlannerDataModelLocal()
{
    stopDataThread();
    mpHeartbeatTimer->deleteLater();
}

void cPlannerDataModelLocal::stopDataThread()
{
    cPlannerDataModel::stopDataThread();

    if (mThread.mpController)
    {
        mThread.mpController->stopDataRecording();
        mThread.mpController->disableDataRecording();
    }
}

bool cPlannerDataModelLocal::openDataFile(const QString& defaultPath,
    const std::string& defaultFileName, bool autoSave)
{
    mFileOpen = true;

    return true;
}

bool cPlannerDataModelLocal::isDataFileOpen() const
{
    return mFileOpen;
}

void cPlannerDataModelLocal::closeDataFile()
{
    mThread.mpController->disableDataRecording();

    for (auto& sensor : mThread.mActiveSensors)
    {
        sensor->disableDataRecording();
    }

    mThread.mpController->disableDataRecording();

    mFileOpen = false;
}

void cPlannerDataModelLocal::endDataRecording()
{
    mThread.mpController->stopDataRecording();

    if (isDataFileOpen())
    {
    }
}

void cPlannerDataModelLocal::dataRecordingStateChange(bool record)
{
    if (record)
    {
        mpHeartbeatTimer->start(1000);
    }
    else
    {
        mpHeartbeatTimer->stop();
    }

    emit recordingStateChanged(record);
}


void cPlannerDataModelLocal::startExperiment()
{
    if (isExperimentRunning())
    {
        mThread.mpController->startExperiment();
        return;
    }

    if (isDataFileOpen())
    {
    }

    mThread.mpController->startExperiment();
}

/***   Signals handlers   ****/
void cPlannerDataModelLocal::onHeartbeat()
{
}

