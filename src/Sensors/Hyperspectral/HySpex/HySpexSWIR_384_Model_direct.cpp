
#include "HySpexSWIR_384_Model_direct.hpp"
#include "Constants.hpp"

#include <HySpexConnect/hyspex_utils.hpp>
#include <HySpexConnect/SWIR384.hpp>

#include <QDebug>

#include <optional>

using namespace hyspex;

cHySpexSWIR_384_Model_direct::cHySpexSWIR_384_Model_direct(std::unique_ptr<hyspex::cSWIR384> camera, QObject* parent)
:
    cHySpexSWIR_384_Model(parent), mCamera(std::move(camera))
{
    mTemperatureUpdateTimer.interval_sec(5);
}

cHySpexSWIR_384_Model_direct::~cHySpexSWIR_384_Model_direct()
{
    mCamera->unregisterNotificationCallback(&cHySpexCameraModel::handleStatusCallback);
}

void cHySpexSWIR_384_Model_direct::updateViews()
{
    cHySpexSWIR_384_Model::updateViews();
}

bool cHySpexSWIR_384_Model_direct::configure(const nlohmann::json& jsonCfg)
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
        auto section = jsonCfg["SWIR-384"];

        cHySpexSWIR_384_Model::configure(section);

    }
    catch (const std::exception& e)
    {
        QString msg = "Error in the \"hyspex, SWIR-384\" camera configuration section: ";
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

bool cHySpexSWIR_384_Model_direct::initialize()
{
    emit statusMessage("Retrieving HySpex SWIR-384 camera configuration...");

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
            QString msg = "SWIR-384: ";
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

    return cHySpexSWIR_384_Model::initialize();
}

bool cHySpexSWIR_384_Model_direct::startCommunications()
{
    mTemperatureUpdateTimer.reset();
    mCamera->initAcquisition();

    mCamera->startAcquisition();

    mAcquisitionStatus = mCamera->getAcquisitionStatus();

    mConnected = (mAcquisitionStatus == hyspex::AcquisitionStatus::HYSPEX_ACQ_PENDING)
        || (mAcquisitionStatus == hyspex::AcquisitionStatus::HYSPEX_ACQ_RUNNING)
        || (mAcquisitionStatus == hyspex::AcquisitionStatus::HYSPEX_ACQ_STOPPED);

    if (mConnected)
        setStatus(sensor::eStatus::RUNNING);
    else
        setStatus(sensor::eStatus::FAILED);

    return mConnected;
}

void cHySpexSWIR_384_Model_direct::stopCommunications()
{
    mCamera->stopAcquisition();

    mAcquisitionStatus = mCamera->getAcquisitionStatus();
    emit acqStatusChanged();

    mConnected = false;
}

void cHySpexSWIR_384_Model_direct::update()
{
    if (mTemperatureUpdateTimer.elapsed())
    {
        mAmbientTemp_C = mCamera->getAmbientTemperature_C();
        emit ambientTempChanged(mAmbientTemp_C);

        mSensorTemp_C = mCamera->getSensorTemperature_C();
        emit sensorTempChanged(mSensorTemp_C);
    }
}

void cHySpexSWIR_384_Model_direct::writeDataHeader()
{
}


void cHySpexSWIR_384_Model_direct::setAverageFrames(std::uint16_t frames)
{}

void cHySpexSWIR_384_Model_direct::setFramePeriod_us(std::uint32_t frame_period_us)
{}

void cHySpexSWIR_384_Model_direct::setIntegrationTime_us(std::uint32_t integration_time_us)
{}

void cHySpexSWIR_384_Model_direct::setNumOfBackgrounds(int num_backgrounds)
{
    if (num_backgrounds < 0) num_backgrounds = 0;
    if (num_backgrounds > 1000) num_backgrounds = 1000;
    mNumBackgrounds = num_backgrounds;
    mCamera->setNumberOfBackgrounds(num_backgrounds);
}

void cHySpexSWIR_384_Model_direct::calcBackground()
{

}

