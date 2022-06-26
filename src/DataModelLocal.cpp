
#include "DataModelLocal.hpp"
#include "Sensors/SensorModel.hpp"
#include "ExperimentControllers/ExperimentCtrlModel.hpp"
#include "ExperimentTypes.hpp"

#include <QtWidgets>

cDataModelLocal::cDataModelLocal(QObject* parent)
:
    cDataModel(parent),
    mSerializer(4096)
{
}

cDataModelLocal::~cDataModelLocal()
{
    stopDataThread();
}

void cDataModelLocal::stopDataThread()
{
    cDataModel::stopDataThread();

    if (mFile.isOpen())
    {
        mThread.mpController->stopDataRecording();

        for (auto& sensor : mThread.mActiveSensors)
        {
            sensor->endDataRecording();
        }

        mFile.close();
    }
}

bool cDataModelLocal::openDataFile(const QString& defaultPath)
{
    QString fileName = QFileDialog::getSaveFileName(nullptr, tr("New File"), defaultPath, tr("Ceres data (*.ceres);;All Files (*.*)"));

    if (fileName.isEmpty())
        return false;

    if (mFile.isOpen())
        return false;

    mFile.open(fileName.toStdString());

    mSerializer.attach(&mFile);

    return mFile.isOpen();
}

void cDataModelLocal::closeDataFile()
{
    mSerializer.endTime(time(nullptr));
    mSerializer.detach();
    mFile.close();
}

void cDataModelLocal::startExperiment()
{
    if (isExperimentRunning())
    {
        mThread.mpController->startExperiment();
        return;
    }

    mThread.mpController->writeDataHeader(mFile);

    for (auto& sensor : mThread.mActiveSensors)
    {
        sensor->writeDataHeader(mFile);
    }

    mSerializer.startTime(time(nullptr));
    mThread.mpController->startExperiment();
}

