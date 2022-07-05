
#include "CtrlDataModelLocal.hpp"
#include "SensorModel.hpp"
#include "ExperimentCtrlModel.hpp"
#include "ExperimentTypes.hpp"

#include <QtWidgets>

cCtrlDataModelLocal::cCtrlDataModelLocal(QObject* parent)
:
    cCtrlDataModel(parent),
    mSerializer(4096)
{
}

cCtrlDataModelLocal::~cCtrlDataModelLocal()
{
    stopDataThread();
}

void cCtrlDataModelLocal::stopDataThread()
{
    cCtrlDataModel::stopDataThread();

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

bool cCtrlDataModelLocal::openDataFile(const QString& defaultPath)
{
    QString fileName = QFileDialog::getSaveFileName(nullptr, tr("New File"), defaultPath, tr("Ceres data (*.ceres);;All Files (*.*)"));

    if (fileName.isEmpty())
        return false;

    if (mFile.isOpen())
        return false;


    auto ext = fileName.lastIndexOf('.');

    char timestamp[100];

    std::time_t t = std::time(nullptr);
    std::strftime(timestamp, sizeof(timestamp), "%Y%m%d%H%M%S", std::localtime(&t));

    fileName.insert(ext, "_");
    fileName.insert(ext + 1, timestamp);

    std::replace_if(fileName.begin(), fileName.end(),
        [](QString::value_type c) {return c <= QChar::Space; }, '_');

    mFile.open(fileName.toStdString());

    mSerializer.attach(&mFile);

    return mFile.isOpen();
}

void cCtrlDataModelLocal::closeDataFile()
{
    mSerializer.endTime(time(nullptr));
    mSerializer.detach();
    mFile.close();
}

void cCtrlDataModelLocal::startExperiment()
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

