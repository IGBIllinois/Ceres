
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

    mThread.mpController->stopDataRecording();
    mThread.mpController->disableDataRecording();
}

bool cPlannerDataModelLocal::openDataFile(const QString& defaultPath,
    const std::string& defaultFileName, bool autoSave)
{
    QString fileName = QString::fromStdString(defaultFileName);

    std::replace_if(fileName.begin(), fileName.end(),
        [](QString::value_type c)
        {
            if (c == '/') return true;
            if (c == '*') return true;
            if (c == '\\') return true;
            if (c == '<') return true;
            if (c == '>') return true;
            if (c == ':') return true;
            if (c == '|') return true;
            if (c == '?') return true;
            return c <= QChar::Space;
        }, '_');

    std::time_t t = std::time(nullptr);
    tm* ltm = localtime(&t);

    if (autoSave)
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
        testPath /= fileName.toStdString();
        fileName = QString::fromStdString(testPath.string());
        fileName += ".ceres";
    }
    else
    {
        if (defaultFileName.empty())
        {
            fileName = defaultPath;
        }
        else
        {
            using namespace std::filesystem;

            path testPath = defaultPath.toStdString();
            testPath /= fileName.toStdString();
            fileName = QString::fromStdString(testPath.string());
            fileName += ".ceres";
        }

        fileName = QFileDialog::getSaveFileName(nullptr,
            tr("New File"), fileName, tr("Ceres data (*.ceres);;All Files (*.*)"));

        if (fileName.isEmpty())
            return false;
    }

    auto ext = fileName.lastIndexOf('.');

    char timestamp[100];

    std::strftime(timestamp, sizeof(timestamp), "%Y%m%d%H%M%S", ltm);

    fileName.insert(ext, "_");
    fileName.insert(ext + 1, timestamp);

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

