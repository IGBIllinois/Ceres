
#include "PlannerDataModel.hpp"
#include "SensorModel.hpp"
#include "ExperimentCtrlModel.hpp"
#include "ExperimentTypes.hpp"

#include "Spidercam/SpidercamModel.hpp"

#include <QMessageBox>

#include <tuple>


namespace
{
    std::tuple<std::string, std::string, std::string> date_split(const std::string& date)
    {
        std::array<std::string, 3> results;
        int slot = 0;
        std::size_t startPos = 0;
        std::size_t lastPos = date.find('/');
        while ((lastPos != std::string::npos) && (slot < 2))
        {
            results[slot++] = date.substr(startPos, lastPos - startPos);
            startPos = lastPos + 1;
            lastPos = date.find('/', startPos);
        }

        results[slot++] = date.substr(startPos, date.size() - startPos);

        return { results[0], results[1], results[2] };
    }

    std::time_t to_time_t(const std::string& month, const std::string& day, const std::string& year)
    {
        if (month.empty() || day.empty() || year.empty())
            return { 0 };

        int mm = std::stoi(month, nullptr, 10);
        int dd = std::stoi(day, nullptr, 10);
        int yy = std::stoi(year, nullptr, 10);

        if (yy < 100) yy += 2000;

        struct tm date = { 0 };
        date.tm_year = yy - 1900;
        date.tm_mon = mm - 1;
        date.tm_mday = dd;
        date.tm_isdst = -1;

        return mktime(&date);
    }
}

cPlannerDataModel::cPlannerDataModel(QObject* parent)
:
    cDataModel(parent)
{
    QObject::connect(&mThread, &cDataThread::statusMessage, this, &cPlannerDataModel::onStatusUpdate);
    QObject::connect(&mThread, &cDataThread::errorMessage, this, &cPlannerDataModel::onErrorUpdate);
    QObject::connect(&mThread, &cDataThread::terminate, this, &cPlannerDataModel::onDataThreadTermination);

    QObject::connect(&mThread, &cPlannerDataThread::connectedToController,      this, &cPlannerDataModel::controllerConnected);
    QObject::connect(&mThread, &cPlannerDataThread::disconnectedFromController, this, &cPlannerDataModel::controllerDisconnected);
}

cPlannerDataModel::~cPlannerDataModel()
{
}

bool cPlannerDataModel::isConnected() const
{
    return mConnected;
}

std::size_t cPlannerDataModel::sensorCount() const
{
    return 0;
}

void cPlannerDataModel::addSensor(cSensorModel* pSensor)
{
    if (pSensor)
    {
        if (!pSensor->initialize())
        {
            emit statusMessage("Sensor failed initialization!");
            return;
        }

        pSensor->moveToThread(&mThread);
        mThread.mSensors.push_back(pSensor);
    }
}

std::vector<cSensorModel*> cPlannerDataModel::getSensors()
{
    return mThread.mSensors;
}

void cPlannerDataModel::removeAllSensors()
{
    mThread.mSensors.clear();
}

void cPlannerDataModel::addExperimentControlModel(cExperimentControlModel* pControlModel)
{
    if (pControlModel)
    {
        mThread.mpController = pControlModel;

        QObject::connect(mThread.mpController, &cExperimentControlModel::experimentStatus, this, &cPlannerDataModel::experimentStatus);
        QObject::connect(mThread.mpController, &cExperimentControlModel::experimentStateChanged, this, &cPlannerDataModel::onExperimentStateChange);

        mThread.mpController->moveToThread(&mThread);

        cSpidercamModel* pScModel = dynamic_cast<cSpidercamModel*>(pControlModel);

        if (pScModel)
        {
            QObject::connect(pScModel, &cSpidercamModel::limitsChanged, this, &cPlannerDataModel::updateLimits);
            QObject::connect(pScModel, &cSpidercamModel::positionChanged, this, &cPlannerDataModel::updatePosition);
            QObject::connect(pScModel, &cSpidercamModel::requestDataRecordingState, this, &cPlannerDataModel::updateRecordingState);
        }
    }
}

cExperimentControlModel* cPlannerDataModel::removeExperimentControlModel()
{
    auto* pModel = mThread.mpController;

    if (pModel)
    {
        cSpidercamModel* pScModel = dynamic_cast<cSpidercamModel*>(pModel);

        if (pScModel)
        {
            QObject::disconnect(pScModel, &cSpidercamModel::limitsChanged, this, &cPlannerDataModel::updateLimits);
            QObject::disconnect(pScModel, &cSpidercamModel::positionChanged, this, &cPlannerDataModel::updatePosition);
            QObject::disconnect(pScModel, &cSpidercamModel::requestDataRecordingState, this, &cPlannerDataModel::updateRecordingState);
        }

        QObject::disconnect(pModel, &cExperimentControlModel::experimentStatus, this, &cPlannerDataModel::experimentStatus);
        QObject::disconnect(pModel, &cExperimentControlModel::experimentStateChanged, this, &cPlannerDataModel::onExperimentStateChange);
    }

    mThread.mpController = nullptr;

    return pModel;
}

void cPlannerDataModel::startDataThread()
{
    mThread.start();

    QObject::connect(mThread.mpController, &cExperimentControlModel::requestDataRecordingState,
        this, &cPlannerDataModel::dataRecordingStateChange);
}

void cPlannerDataModel::stopDataThread()
{
    QObject::disconnect(mThread.mpController, &cExperimentControlModel::requestDataRecordingState,
        this, &cPlannerDataModel::dataRecordingStateChange);

    mThread.stop();
}

std::string cPlannerDataModel::measurementTitle() const
{
    return mMeasurementTitle;
}

bool cPlannerDataModel::systemReady() const
{
    if (!mThread.mpController)
        return false;

    return mThread.mpController->systemReady();
}

bool cPlannerDataModel::isExperimentRunning()
{
    if (!mThread.mpController)
        return false;

    return mThread.mpController->isExperimentRunning();
}

bool cPlannerDataModel::isExperimentPaused()
{
    if (!mThread.mpController)
        return false;

    return mThread.mpController->isExperimentPaused();
}

bool cPlannerDataModel::isExperimentLoaded() const
{
    if (!mThread.mpController)
        return false;

    return mThread.mpController->hasExperiment();
}

bool cPlannerDataModel::experimentRequiresDataFile() const
{
    if (!mThread.mpController)
        return false;

    return mThread.mpController->experimentRequiresDataFile();
}

bool cPlannerDataModel::loadExperiment(const std::string& expPath, const std::string& expName, const nlohmann::json& expDoc)
{
    using namespace nlohmann;

    if (isExperimentRunning())
    {
        return false;
    }

    try
    {
        if (!expDoc.contains("experiment"))
        {
            return false;
        }
    
        std::string ctrl = expDoc["controller"];
        if (ctrl.compare(mThread.mpController->descriptor()) != 0)
        {
            return false;
        }

        if (mThread.mpController->loadExperiment(expPath, expName, expDoc["experiment"]))
        {
            if (expDoc.contains("measurement name"))
                mMeasurementTitle = static_cast<std::string>(expDoc["measurement name"]);
            else if(expDoc.contains("measurement_name"))
                mMeasurementTitle = static_cast<std::string>(expDoc["measurement_name"]);
            else if (expDoc.contains("experiment name"))
                mMeasurementTitle = static_cast<std::string>(expDoc["experiment name"]);
            else
                mMeasurementTitle = static_cast<std::string>(expDoc["experiment_name"]);
        }
    }
    catch (const detail::parse_error& e)
    {
        QString msg = "Parse Error: ";
        msg += e.what();
        msg += "\n\n";
        msg += "Skipping experiment: " + QString::fromStdString(expName);

        QMessageBox mb(QMessageBox::Critical, QString::fromStdString(expName), msg);
        mb.exec();

        return false;
    }
    catch (const detail::type_error& e)
    {
        QString msg = "Type Error: ";
        msg += e.what();
        msg += "\n\n";
        msg += "Skipping experiment: " + QString::fromStdString(expName);

        QMessageBox mb(QMessageBox::Critical, QString::fromStdString(expName), msg);
        mb.exec();

        return false;
    }
    catch (const detail::exception& e)
    {
        QString msg = "Unknown Error: ";
        msg += e.what();
        msg += "\n\n";
        msg += "Skipping experiment: " + QString::fromStdString(expName);

        QMessageBox mb(QMessageBox::Critical, QString::fromStdString(expName), msg);
        mb.exec();

        return false;
    }

    return true;
}

bool cPlannerDataModel::unloadExperiment()
{
    if (isExperimentRunning())
    {
        return false;
    }

    mThread.mpController->clearExperiment();
    return true;
}

void cPlannerDataModel::pauseExperiment()
{
    if (mThread.mpController)
        mThread.mpController->pauseExperiment();
}

void cPlannerDataModel::terminateExperiment()
{
    if (mThread.mpController)
        mThread.mpController->terminateExperiment();
}

spidercam::sPosition_1_t cPlannerDataModel::getPosition() const
{
    return mCurrentPosition;
}

void cPlannerDataModel::updateLimits(spidercam::sWorkingDimensions limits)
{
    mLimits = limits;
    emit limitsChanged(limits);

}

void cPlannerDataModel::updatePosition(spidercam::sPosition_1_t pos)
{
    mCurrentPosition = pos;
    emit positionChanged(pos);
}

void cPlannerDataModel::updateRecordingState(bool recording)
{
    mRecording = recording;
}

void cPlannerDataModel::onExperimentStateChange(experiment::eState state)
{
    using namespace experiment;

    switch (state)
    {
        case eState::PAUSED:
            break;
        case eState::EXP_ERROR:
        {
            doExperimentCleanup();

            emit experimentTerminated();

            break;
        }
        case eState::COMPLETED:
        {
            doExperimentCleanup();

            emit experimentCompleted();

            break;
        }
        case eState::TERMINATED:
        {
            doExperimentCleanup();

            emit experimentTerminated();

            break;
        }
    }

    emit experimentStateChanged(state);
}

void cPlannerDataModel::doExperimentCleanup()
{
    endDataRecording();

    closeDataFile();

    mMeasurementTitle.clear();
	
    if (mThread.mpController)
    {
        mThread.mpController->clearExperiment();
    }
}

void cPlannerDataModel::onDataThreadTermination()
{
    mConnected = false;

    if (isExperimentRunning())
        terminateExperiment();
}

void cPlannerDataModel::controllerConnected()
{
    mConnected = true;

    emit connectedToController();
}

void cPlannerDataModel::controllerDisconnected()
{
    mConnected = false;

    emit disconnectedFromController();
}

