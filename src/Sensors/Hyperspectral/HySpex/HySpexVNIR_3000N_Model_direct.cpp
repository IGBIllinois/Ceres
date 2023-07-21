
#include "HySpexVNIR_3000N_Model_direct.hpp"
#include "Constants.hpp"

#include <HySpexConnect/hyspex_utils.hpp>
#include <HySpexConnect/VNIR3000N.hpp>

#include <QDebug>

#include <optional>

using namespace hyspex;

cHySpexVNIR_3000N_Model_direct::cHySpexVNIR_3000N_Model_direct(std::unique_ptr<hyspex::cVNIR3000N> camera, QObject* parent)
:
    cHySpexVNIR_3000N_Model(parent), mCamera(std::move(camera))
{
    mTemperatureUpdateTimer.interval_sec(5);
}

cHySpexVNIR_3000N_Model_direct::~cHySpexVNIR_3000N_Model_direct()
{
    mCamera->unregisterNotificationCallback(&cHySpexCameraModel::handleStatusCallback);
}


void cHySpexVNIR_3000N_Model_direct::updateViews()
{
    cHySpexVNIR_3000N_Model::updateViews();
}

bool cHySpexVNIR_3000N_Model_direct::configure(const nlohmann::json& jsonCfg)
{
    mID = mCamera->getId();
    mSerialNumber = mCamera->getSerialNumber();

    mCamera->registerNotificationCallback(&cHySpexCameraModel::handleStatusCallback, this);

    mLenses.clear();
    auto n = mCamera->getLensCount();
    for (unsigned int l = 0; l < n; ++l)
    {
        mLenses.emplace_back(mCamera->getLensNameFromId(l));
    }


    try
    {
        auto section = jsonCfg["VNIR-3000N"];

        cHySpexVNIR_3000N_Model::configure(section);
    }
    catch (const std::exception& e)
    {
        QString msg = "Error in the \"hyspex, VNIR-3000N\" camera configuration section: ";
        msg.append(e.what());
        qCritical() << msg;

        emit logMessage(logERROR, q_name(), msg);

        setStatus(sensor::eStatus::FAILED);

        return false;
    }

    if (mLens != mCamera->getLensName())
    {
        auto n = mCamera->getLensCount();
        for (unsigned int l = 0; l < n; ++l)
        {
            auto lens = mCamera->getLensNameFromId(l);
            if (lens == mLens)
            {
                mCamera->useLensId(l);
                break;
            }
        }
    }

    mLens = mCamera->getLensName();
    mWorkingDistance_cm = mCamera->getLensWorkingDistance_cm();
    mFieldOfView_deg = mCamera->getLensFieldOfView_rad() * nConstants::RAD_TO_DEG;
    emit lensInfoChanged();

    setStatus(sensor::eStatus::CONFIGURED);

    return true;
}

bool cHySpexVNIR_3000N_Model_direct::initialize()
{
    emit statusMessage("Retrieving HySpex VNIR-3000N camera configuration...");

    // We need to initialize the connection to the physical camera.
    // This usually requires two calls to init!
    mCamera->init(mNumBuffersRaw, mNumBufferPreProcessing);

    mInitStatus = mCamera->getInitStatus();
    emit initStatusChanged();

    if (mInitStatus == hyspex::InitStatus::HYSPEX_INIT_NOT_STARTED)
    {
        mCamera->init(mNumBuffersRaw, mNumBufferPreProcessing);
    }

    mInitStatus = mCamera->getInitStatus();
    emit initStatusChanged();

    switch (mInitStatus)
    {
    case hyspex::InitStatus::HYSPEX_INIT_PENDING_DETECTION:
        break;
    case hyspex::InitStatus::HYSPEX_INIT_PENDING_ELECTRONICS:
        break;
    case hyspex::InitStatus::HYSPEX_INIT_PENDING_SENSOR:
        break;
    case hyspex::InitStatus::HYSPEX_INIT_FAILED_DETECTION:
    case hyspex::InitStatus::HYSPEX_INIT_FAILED_ELECTRONICS:
    case hyspex::InitStatus::HYSPEX_INIT_FAILED_SENSOR:
    case hyspex::InitStatus::HYSPEX_INIT_FAILED_TRANSPORT:
        {
            QString msg = "VNIR-3000N: ";
            msg += to_string(mInitStatus).c_str();
            emit logMessage(logERROR, q_name(), msg);

            qCritical() << msg;

            setStatus(sensor::eStatus::FAILED);
            return false;
        }
    }

    mWavelengthRangeId = mCamera->getWavelengthRangeId();

    mSpectralSize = mCamera->getSpectralSize();
    mSpatialSize = mCamera->getSpatialSize();

    mMaxSpatialSize = mCamera->getMaxSpatialSize();
    mMaxSpectralSize = mCamera->getMaxSpectralSize();

    mMaxPixelValue = mCamera->getMaxPixelValue();

    mCommStatus = mCamera->getCommunicationStatus();
    emit commStatusChanged();

	mCoolingStatus = mCamera->getCoolingStatus();
    emit coolingStatusChanged();

    mShutterStatus = mCamera->getShutterStatus();
    emit shutterStatusChanged();

	mAverageFrames = mCamera->getAverageFrames();
    emit avgFramesChanged(mAverageFrames);

	mFramePeriod_us = mCamera->getFramePeriod_us();
    emit framePeriodChanged(mFramePeriod_us);

    mMinFramePeriod_us = mCamera->getMinimumFramePeriod_us();
    emit minFramePeriodChanged(mMinFramePeriod_us);
	
    mIntegrationTime_us = mCamera->getIntegrationTime_us();
    emit integrationTimeChanged(mIntegrationTime_us);
	
    mMaxIntegrationTime_us = mCamera->getMaxIntegrationTime_us(mFramePeriod_us);
    emit maxIntegrationTimeChanged(mMaxIntegrationTime_us);
	
    mAmbientTemp_C = mCamera->getAmbientTemperature_C();
    emit ambientTempChanged(mAmbientTemp_C);
	
    mSensorTemp_C = mCamera->getSensorTemperature_C();
    emit sensorTempChanged(mSensorTemp_C);

    mNumBackgrounds = mCamera->getNumberOfBackgrounds();
	mBackgroundStatus = mCamera->getBackgroundStatus();
    emit bgStatusChanged();

	mAcquisitionStatus = mCamera->getAcquisitionStatus();
    emit acqStatusChanged();

/*
	auto badPixels = vnir->getBadPixels();
	std::cout << "Num Bad Pixels = " << badPixels.size << std::endl;

	auto badCorrPixels = vnir->getBadPixelsWithCalculatedCorrections();
	std::cout << "Bad Pixels With Calculated Corrections = " << badCorrPixels.size << std::endl;

	auto badPixelsMatrix = vnir->getBadPixelsMatrix();
	std::cout << "Bad Pixels Matrix = " << badPixelsMatrix.size() << std::endl;
*/

	auto reMatrix = mCamera->getResponsivityMatrix();
	auto qeMatrix = mCamera->getQuantumEfficiencyMatrix();

	auto spectralCal = mCamera->getSpectralCalibrationPerBand();
	auto fullSpectralCal = mCamera->getFullSpectralCalibrationPerBand();

    //    mBackground.resize(mSpatialSize, mSpectralSize);
    //    mBackground = mCamera->getBackgroundMatrix();

    return cHySpexVNIR_3000N_Model::initialize();
}


bool cHySpexVNIR_3000N_Model_direct::startCommunications()
{
    mTemperatureUpdateTimer.reset();
    mCamera->initAcquisition();

    mCamera->startAcquisition();

    mAcquisitionStatus = mCamera->getAcquisitionStatus();

    mConnected = (mAcquisitionStatus == hyspex::AcquisitionStatus::HYSPEX_ACQ_PENDING)
        || (mAcquisitionStatus == hyspex::AcquisitionStatus::HYSPEX_ACQ_RUNNING)
        || (mAcquisitionStatus == hyspex::AcquisitionStatus::HYSPEX_ACQ_STOPPED);

    if (mConnected)
    {
        if ((mCoolingStatus == hyspex::CoolingStatus::HYSPEX_COOLING_STABLE_OK) || 
            (mCoolingStatus == hyspex::CoolingStatus::HYSPEX_COOLING_STABLE_DEGRADED))
            setStatus(sensor::eStatus::RUNNING);
        else
            setStatus(sensor::eStatus::BUSY);
    }
    else
        setStatus(sensor::eStatus::FAILED);

    return mConnected;
}

void cHySpexVNIR_3000N_Model_direct::stopCommunications()
{
    mCamera->stopAcquisition();

    mAcquisitionStatus = mCamera->getAcquisitionStatus();
    emit acqStatusChanged();

    mConnected = false;

    setStatus(sensor::eStatus::STOPPED);
}

void cHySpexVNIR_3000N_Model_direct::update()
{
    if (mTemperatureUpdateTimer.elapsed())
    {
        mAmbientTemp_C = mCamera->getAmbientTemperature_C();
        emit ambientTempChanged(mAmbientTemp_C);

        mSensorTemp_C = mCamera->getSensorTemperature_C();
        emit sensorTempChanged(mSensorTemp_C);
    }
}

void cHySpexVNIR_3000N_Model_direct::writeDataHeader()
{
}

void cHySpexVNIR_3000N_Model_direct::setAverageFrames(std::uint16_t frames)
{
    mCamera->setAverageFrames(frames);
    mAverageFrames = mCamera->getAverageFrames();
    emit avgFramesChanged(mAverageFrames);
}

void cHySpexVNIR_3000N_Model_direct::setFramePeriod_us(std::uint32_t frame_period_us)
{
    mCamera->setFramePeriod_us(frame_period_us);
    mFramePeriod_us = mCamera->getFramePeriod_us();
    mMaxIntegrationTime_us = mCamera->getMaxIntegrationTime_us(mFramePeriod_us);
    emit framePeriodChanged(mFramePeriod_us);
    emit maxIntegrationTimeChanged(mMaxIntegrationTime_us);
}

void cHySpexVNIR_3000N_Model_direct::setIntegrationTime_us(std::uint32_t integration_time_us)
{
    mCamera->setIntegrationTime_us(integration_time_us);
    mIntegrationTime_us = mCamera->getIntegrationTime_us();
    mMinFramePeriod_us = mCamera->getMinimumFramePeriod_us();
    emit integrationTimeChanged(mIntegrationTime_us);
    emit minFramePeriodChanged(mMinFramePeriod_us);
}

void cHySpexVNIR_3000N_Model_direct::setNumOfBackgrounds(int num_backgrounds)
{
    if (num_backgrounds < 0) num_backgrounds = 0;
    if (num_backgrounds > 1000) num_backgrounds = 1000;
    mCamera->setNumberOfBackgrounds(num_backgrounds);
    mNumBackgrounds = mCamera->getNumberOfBackgrounds();
}

void cHySpexVNIR_3000N_Model_direct::calcBackground()
{
    mCamera->closeShutter();
    mCamera->calculateBackground(0, mNumBackgrounds);
}




