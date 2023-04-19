
#include "CtrlDataModelLocal.hpp"
#include "SensorModel.hpp"
#include "ExperimentCtrlModel.hpp"
#include "ExperimentTypes.hpp"
#include "TimestampProvider.hpp"

#include <QtWidgets>

#include <filesystem>
#include <ctime>


cCtrlDataModelLocal::cCtrlDataModelLocal(QObject* parent)
:
    cCtrlDataModel(parent),
    mSerializer(4096)
{
    mpHeartbeatTimer = new QTimer(this);
    connect(mpHeartbeatTimer, &QTimer::timeout, this, &cCtrlDataModelLocal::onHeartbeat);
}

cCtrlDataModelLocal::~cCtrlDataModelLocal()
{
    stopDataThread();
    mpHeartbeatTimer->deleteLater();
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

bool cCtrlDataModelLocal::openDataFile(const QString& defaultPath, const std::string& defaultFileName)
{
    QString fileName;

    std::time_t t = std::time(nullptr);
    tm* ltm = localtime(&t);

    if (!defaultFileName.empty())
    {
        using namespace std::filesystem;

        path testPath = defaultPath.toStdString();

        switch (ltm->tm_mon)
        {
        case 0: testPath /= "Jan"; break;
        case 1: testPath /= "Feb"; break;
        case 2: testPath /= "Mar"; break;
        case 3: testPath /= "Apr"; break;
        case 4: testPath /= "May"; break;
        case 5: testPath /= "June"; break;
        case 6: testPath /= "July"; break;
        case 7: testPath /= "Aug"; break;
        case 8: testPath /= "Sept"; break;
        case 9: testPath /= "Oct"; break;
        case 10: testPath /= "Nov"; break;
        case 11: testPath /= "Dec"; break;
        }
        testPath += std::to_string(ltm->tm_mday);

        if (!exists(testPath))
        {
            create_directories(testPath);
        }
        testPath /= defaultFileName;
        fileName = QString::fromStdString(testPath.string());
        fileName += ".ceres";
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

    std::string filename = fileName.toStdString();
    mFile.open(filename);

    if (!mFile.isOpen())
    {
        return false;
    }

    mSerializer.attach(&mFile);

    mThread.mpController->enableDataRecording(mFile);

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

    mThread.mpController->disableDataRecording();

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

    if (isDataFileOpen())
    {
        mSerializer.writeBeginFooter();
        mThread.mpController->writeDataFooter();

        for (auto& sensor : mThread.mActiveSensors)
        {
            sensor->writeDataFooter();
        }
        mSerializer.writeEndOfFooter();
    }
}

void cCtrlDataModelLocal::dataRecordingStateChange(bool record)
{
    if (record)
    {
        mSerializer.startRecordingTimestamp(timestamp_ns());
        mpHeartbeatTimer->start(1000);
    }
    else
    {
        mSerializer.endRecordingTimestamp(timestamp_ns());
        mpHeartbeatTimer->stop();
    }
}


void cCtrlDataModelLocal::startExperiment()
{
    if (isExperimentRunning())
    {
        mThread.mpController->startExperiment();
        return;
    }

    if (isDataFileOpen())
    {
        mSerializer.writeBeginHeader();
        mSerializer.writeTitle(mExperimentTitle);

        if (!mResearcher.empty())
            mSerializer.writeResearcher(mResearcher);

        if (!mCultivar.empty())
            mSerializer.writeCultivar(mCultivar);

        time_t t = time(nullptr);
        auto tm = localtime(&t);
        std::uint16_t year = tm->tm_year + 1900;
        std::uint8_t month = tm->tm_mon + 1;
        std::uint8_t day = tm->tm_mday;

        std::uint16_t doy = tm->tm_yday;

        std::uint8_t hour = tm->tm_hour;
        std::uint8_t minute = tm->tm_min;
        std::uint8_t sec = tm->tm_sec;

        mSerializer.writeDate(year, month, day);
        mSerializer.writeDayOfYear(doy);
        mSerializer.writeTime(hour, minute, sec);

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
    }

    mThread.mpController->startExperiment();
}

/***   Signals handlers   ****/
void cCtrlDataModelLocal::onHeartbeat()
{
    if (mSerializer)
    {
        mSerializer.heartbeatTimestamp(timestamp_ns());
    }
}

