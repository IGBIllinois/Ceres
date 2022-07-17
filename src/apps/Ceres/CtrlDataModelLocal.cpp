
#include "CtrlDataModelLocal.hpp"
#include "SensorModel.hpp"
#include "ExperimentCtrlModel.hpp"
#include "ExperimentTypes.hpp"
#include "TimestampProvider.hpp"

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
        mThread.mpController->disableDataRecording();

        for (auto& sensor : mThread.mActiveSensors)
        {
            sensor->disableDataRecording();
        }

        mFile.close();
    }
}

bool cCtrlDataModelLocal::openDataFile(const QString& defaultPath, bool autoSave)
{
    QString fileName;

    std::time_t t = std::time(nullptr);
    tm* ltm = localtime(&t);

    if (autoSave)
    {
        QString _filename = defaultPath;
        _filename += "/";

        switch (ltm->tm_mon)
        {
        case 0: _filename += "Jan"; break;
        case 1: _filename += "Feb"; break;
        case 2: _filename += "Mar"; break;
        case 3: _filename += "Apr"; break;
        case 4: _filename += "May"; break;
        case 5: _filename += "June"; break;
        case 6: _filename += "July"; break;
        case 7: _filename += "Aug"; break;
        case 8: _filename += "Sept"; break;
        case 9: _filename += "Oct"; break;
        case 10: _filename += "Nov"; break;
        case 11: _filename += "Dec"; break;
        }
        _filename += QString::number(ltm->tm_mday);
        _filename += "/";
        _filename += QString::fromStdString(mExperimentTitle);
    }
    else
    {
        fileName = QFileDialog::getSaveFileName(nullptr, tr("New File"), defaultPath, tr("Ceres data (*.ceres);;All Files (*.*)"));

        if (fileName.isEmpty())
            return false;
    }

    if (mFile.isOpen())
        return false;


    auto ext = fileName.lastIndexOf('.');

    char timestamp[100];

    std::strftime(timestamp, sizeof(timestamp), "%Y%m%d%H%M%S", ltm);

    fileName.insert(ext, "_");
    fileName.insert(ext + 1, timestamp);

    std::replace_if(fileName.begin(), fileName.end(),
        [](QString::value_type c) {return c <= QChar::Space; }, '_');

    mFile.open(fileName.toStdString());

    if (!mFile.isOpen())
    {
        return false;
    }

    mSerializer.attach(&mFile);

    for (auto& sensor : mThread.mActiveSensors)
    {
        sensor->enableDataRecording(mFile);
    }

    return true;
}

bool cCtrlDataModelLocal::isDataFileOpen() const
{
    return mFile.isOpen();
}

void cCtrlDataModelLocal::closeDataFile()
{
    mThread.mpController->disableDataRecording();

    for (auto& sensor : mThread.mActiveSensors)
    {
        sensor->disableDataRecording();
    }

    mSerializer.endTime(time(nullptr));
    mSerializer.detach();
    mFile.close();

    mExperimentTitle.clear();
    mResearcher.clear();
    mCultivar.clear();
    mExperimentDoc.clear();
}

void cCtrlDataModelLocal::endDataRecording()
{
    mThread.mpController->stopDataRecording();

    mSerializer.writeBeginFooter();
    mThread.mpController->writeDataFooter();

    for (auto& sensor : mThread.mActiveSensors)
    {
        sensor->writeDataFooter();
    }
    mSerializer.writeEndOfFooter();

}

void cCtrlDataModelLocal::dataRecordingStateChange(bool record)
{
    if (record)
        mSerializer.startRecordingTimestamp(timestamp_ns());
    else
        mSerializer.endRecordingTimestamp(timestamp_ns());
}


void cCtrlDataModelLocal::startExperiment()
{
    if (isExperimentRunning())
    {
        mThread.mpController->startExperiment();
        return;
    }

    mSerializer.writeBeginHeader();
    mSerializer.writeTitle(mExperimentTitle);

    if (!mResearcher.empty())
        mSerializer.writeResearcher(mResearcher);

    if (!mCultivar.empty())
        mSerializer.writeCultivar(mCultivar);

    mSerializer.writeExperimentDoc(mExperimentDoc);

    mThread.mpController->writeDataHeader();

    mSerializer.writeBeginSensorList();
    for (auto& sensor : mThread.mActiveSensors)
    {
        mSerializer.writeSensorBlockInfo(sensor->data_class_id(), sensor->name());
    }
    mSerializer.writeEndOfSensorList();

    for (auto& sensor : mThread.mActiveSensors)
    {
        sensor->writeDataHeader();
    }

    mSerializer.startTime(time(nullptr));
    mSerializer.writeEndOfHeader();

    mThread.mpController->startExperiment();
}

