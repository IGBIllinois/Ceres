
#include "CtrlDataModel.hpp"
#include "SensorModel.hpp"
#include "ExperimentCtrlModel.hpp"
#include "ExperimentTypes.hpp"


cCtrlDataModel::cCtrlDataModel(QObject* parent)
:
    cDataModel(parent)
{
    QObject::connect(&mThread, &cDataThread::statusMessage, this, &cCtrlDataModel::onStatusUpdate);

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

std::string cCtrlDataModel::experimentTitle() const
{
    return mExperimentTitle;
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
    if (isExperimentRunning())
    {
        return false;
    }

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
        mPrincipalInvestigator.clear();
        mResearcher.clear();
        mSpecies.clear();
        mCultivar.clear();
        mPermitInfo.clear();
        mTreatments.clear();
        mConstructName.clear();
        mEventNumber.clear();
        mFieldDesign.clear();
        mComments.clear();
        mPlantingDate = 0;
        mHarvestDate = 0;

        mExperimentTitle = static_cast<std::string>(expDoc["experiment_name"]);

        if (expDoc.contains("principal investigator"))
        {
            mPrincipalInvestigator = expDoc["principal investigator"];
        }

        if (expDoc.contains("researcher"))
        {
            mResearcher = expDoc["researcher"];
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
            mEventNumber = expDoc["event number"];
        }

        if (expDoc.contains("field design"))
        {
            mFieldDesign = expDoc["field design"];
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

        if (expDoc.contains("planting date"))
        { 
        }

        if (expDoc.contains("target harvest date"))
        {
        }

        mExperimentDoc = to_string(expDoc);
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
}

void cCtrlDataModel::doExperimentCleanup()
{
    endDataRecording();

    closeDataFile();

    mExperimentTitle.clear();
    mExperimentDoc.clear();
	
    if (mThread.mpController)
        mThread.mpController->clearExperiment();
}

