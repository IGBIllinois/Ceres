
#include "CtrlDataModel.hpp"
#include "SensorModel.hpp"
#include "ExperimentCtrlModel.hpp"
#include "ExperimentTypes.hpp"

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

cCtrlDataModel::cCtrlDataModel(QObject* parent)
:
    cDataModel(parent)
{
    QObject::connect(&mThread, &cDataThread::statusMessage, this, &cCtrlDataModel::onStatusUpdate);
    QObject::connect(&mThread, &cDataThread::errorMessage, this, &cCtrlDataModel::onErrorUpdate);
    QObject::connect(&mThread, &cDataThread::terminate, this, &cCtrlDataModel::onDataThreadTermination);

    mPlantingDate = 0;
    mHarvestDate = 0;
}

cCtrlDataModel::~cCtrlDataModel()
{
}


void cCtrlDataModel::addExperimentControlModel(cExperimentControlModel* pControlModel)
{
    if (pControlModel)
    {
        mThread.mpController = pControlModel;
        QObject::connect(mThread.mpController, &cExperimentControlModel::experimentStateChanged, this, &cCtrlDataModel::onExperimentStateChange);
        mThread.mpController->moveToThread(&mThread);
    }
}

std::size_t cCtrlDataModel::sensorCount() const
{
    return mThread.mSensors.size();
}

void cCtrlDataModel::addSensor(cSensorModel* pSensor)
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

void cCtrlDataModel::addSensorPropertyPage(cSensorPropertyPage* pPage)
{
    //TODO
}

void cCtrlDataModel::removeSensorPropertyPage(cSensorPropertyPage* pPage)
{
    //TODO
}

void cCtrlDataModel::startDataThread()
{
    mThread.start();

    QObject::connect(mThread.mpController, &cExperimentControlModel::requestDataRecordingState,
        this, &cCtrlDataModel::dataRecordingStateChange);
}

void cCtrlDataModel::stopDataThread()
{
    QObject::disconnect(mThread.mpController, &cExperimentControlModel::requestDataRecordingState,
        this, &cCtrlDataModel::dataRecordingStateChange);

    mThread.stop();
}

std::string cCtrlDataModel::measurementTitle() const
{
    if (mMeasurementTitle.empty())
        return mExperimentTitle;

    return mMeasurementTitle;
}

bool cCtrlDataModel::systemReady() const
{
    if (!mThread.mpController)
        return false;

    return mThread.mpController->systemReady();
}

bool cCtrlDataModel::isExperimentRunning()
{
    if (!mThread.mpController)
        return false;

    return mThread.mpController->isExperimentRunning();
}

bool cCtrlDataModel::isExperimentPaused()
{
    if (!mThread.mpController)
        return false;

    return mThread.mpController->isExperimentPaused();
}

bool cCtrlDataModel::isExperimentLoaded() const
{
    if (!mThread.mpController)
        return false;

    return mThread.mpController->hasExperiment();
}

bool cCtrlDataModel::experimentRequiresDataFile() const
{
    if (!mThread.mpController)
        return false;

    return mThread.mpController->experimentRequiresDataFile();
}

bool cCtrlDataModel::loadExperiment(const std::string& expName, const nlohmann::json& expDoc)
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

        auto required_sensors = expDoc["sensors"];

/*
        for (auto required_sensor : required_sensors)
        {
            bool found = false;

            for (auto& sensor : mThread.mActiveSensors)
            {
                if (required_sensor == sensor->descriptor())
                {
                    found = true;
                    break;
                }
            }
        }
*/

        if (mThread.mpController->loadExperiment(expName, expDoc["experiment"]))
        {
            mMeasurementTitle.clear();
            mPrincipalInvestigator.clear();
            mResearchers.clear();
            mSpecies.clear();
            mCultivar.clear();
            mPermitInfo.clear();
            mTreatments.clear();
            mConstructName.clear();
            mEventNumbers.clear();
            mFieldDesign.clear();
            mComments.clear();
            mPlantingDate = 0;
            mHarvestDate = 0;

            if (expDoc.contains("experiment name"))
                mExperimentTitle = static_cast<std::string>(expDoc["experiment name"]);
            else
                mExperimentTitle = static_cast<std::string>(expDoc["experiment_name"]);

            if (expDoc.contains("measurement name"))
                mMeasurementTitle = static_cast<std::string>(expDoc["measurement name"]);
            else if (expDoc.contains("measurement_name"))
                mMeasurementTitle = static_cast<std::string>(expDoc["measurement_name"]);

            if (expDoc.contains("principal investigator"))
            {
                mPrincipalInvestigator = expDoc["principal investigator"];
            }

            if (expDoc.contains("researcher"))
            {
                mResearchers.push_back( expDoc["researcher"] );
            }

            if (expDoc.contains("researchers"))
            {
                auto researchers = expDoc["researchers"];
                for (auto it = researchers.begin(); it != researchers.end(); ++it)
                    mResearchers.push_back(*it);
            }

            if (expDoc.contains("species"))
            {
                mSpecies = expDoc["species"];
            }

            if (expDoc.contains("cultivar"))
            {
                mCultivar = expDoc["cultivar"];
            }

            if (expDoc.contains("permit info"))
            {
                mPermitInfo = expDoc["permit info"];
            }

            if (expDoc.contains("construct"))
            {
                mConstructName = expDoc["construct"];
            }

            if (expDoc.contains("event number"))
            {
                mEventNumbers.push_back( expDoc["event number"] );
            }

            if (expDoc.contains("event numbers"))
            {
                auto event_numbers = expDoc["event numbers"];
                for (auto it = event_numbers.begin(); it != event_numbers.end(); ++it)
                    mEventNumbers.push_back(*it);
            }

            if (expDoc.contains("field design"))
            {
                mFieldDesign = expDoc["field design"];
            }

            if (expDoc.contains("treatment"))
            {
                mTreatments.push_back(expDoc["treatment"]);
            }

            if (expDoc.contains("treatments"))
            {
                auto treatments = expDoc["treatments"];
                if (treatments.is_string())
                {
                    mTreatments.push_back(treatments);
                }
                else if (treatments.is_array())
                {
                    for (auto it = treatments.begin(); it != treatments.end(); ++it)
                        mTreatments.push_back(*it);
                }
            }

            if (expDoc.contains("comment"))
            {
                mComments.push_back(expDoc["comment"]);
            }

            if (expDoc.contains("comments"))
            {
                auto comments = expDoc["comments"];
                if (comments.is_string())
                {
                    mComments.push_back(comments);
                }
                else if (comments.is_array())
                {
                    for (auto it = comments.begin(); it != comments.end(); ++it)
                        mComments.push_back(*it);
                }
            }

            std::string month;
            std::string day;
            std::string year;

            if (expDoc.contains("planting date (m/d/y)"))
            {
                std::string date = expDoc["planting date (m/d/y)"];
                std::tie(month, day, year) = date_split(date);
            }

            if (expDoc.contains("planting date (d/m/y)"))
            {
                std::string date = expDoc["planting date (d/m/y)"];
                std::tie(day, month, year) = date_split(date);
            }

            if (expDoc.contains("planting date (y/m/d)"))
            {
                std::string date = expDoc["planting date (y/m/d)"];
                std::tie(year, month, day) = date_split(date);
            }

            mPlantingDate = to_time_t(month, day, year);

            month.clear();
            day.clear();
            year.clear();

            if (expDoc.contains("target harvest date (m/d/y)"))
            {
                std::string date = expDoc["target harvest date (m/d/y)"];
                std::tie(month, day, year) = date_split(date);
            }

            if (expDoc.contains("target harvest date (d/m/y)"))
            {
                std::string date = expDoc["target harvest date (d/m/y)"];
                std::tie(day, month, year) = date_split(date);
            }

            if (expDoc.contains("target harvest date (y/m/d)"))
            {
                std::string date = expDoc["target harvest date (y/m/d)"];
                std::tie(year, month, day) = date_split(date);
            }
        
            mHarvestDate = to_time_t(month, day, year);

            mExperimentDoc = to_string(expDoc);
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

bool cCtrlDataModel::unloadExperiment()
{
    if (isExperimentRunning())
    {
        return false;
    }

    mThread.mpController->clearExperiment();
    return true;
}

void cCtrlDataModel::pauseExperiment()
{
    if (mThread.mpController)
        mThread.mpController->pauseExperiment();
}

void cCtrlDataModel::terminateExperiment()
{
    if (mThread.mpController)
        mThread.mpController->terminateExperiment();
}

void cCtrlDataModel::onExperimentStateChange(experiment::eState state)
{
    using namespace experiment;

    switch (state)
    {
        case eState::PAUSED:
            break;
        case eState::EXP_ERROR:
        {
            doMeasurementCleanup();

            emit experimentTerminated();

            break;
        }
        case eState::COMPLETED:
        {
            doMeasurementCleanup();

            emit experimentCompleted();

            break;
        }
        case eState::TERMINATED:
        {
            doMeasurementCleanup();

            emit experimentTerminated();

            break;
        }
    }
}

void cCtrlDataModel::doMeasurementCleanup()
{
    endDataRecording();

    closeDataFile();

    mExperimentTitle.clear();
    mMeasurementTitle.clear();
    mExperimentDoc.clear();
	
    if (mThread.mpController)
    {
        mThread.mpController->clearExperiment();
    }
}

void cCtrlDataModel::onDataThreadTermination()
{
    if (isExperimentRunning())
        terminateExperiment();
}

