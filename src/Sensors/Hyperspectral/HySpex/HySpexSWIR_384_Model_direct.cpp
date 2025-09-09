
#include "HySpexSWIR_384_Model_direct.hpp"
#include "Constants.hpp"

#include <hyspex_connect/hyspex_utils.hpp>
#include <hyspex_connect/SWIR384.hpp>
#include <hyspex_connect/HySpexExceptions.hpp>


#include <QDebug>

#include <optional>

using namespace hyspex;


void cHySpexSWIR_384_Model_direct::handleImageCallback(void* p, hyspex::ImageOptions a_options, const hyspex::ImageLine< unsigned short >& a_image)
{
    using namespace hyspex;

    if (!p) return;

    auto* self = static_cast<cHySpexSWIR_384_Model_direct*>(p);

    self->updateImageData(a_options, a_image);
}


cHySpexSWIR_384_Model_direct::cHySpexSWIR_384_Model_direct(std::unique_ptr<hyspex::cSWIR384> camera, QObject* parent)
:
    cHySpexSWIR_384_Model(parent), mCamera(std::move(camera))
{
    mTemperatureUpdateTimer.interval_sec(5);
}

cHySpexSWIR_384_Model_direct::~cHySpexSWIR_384_Model_direct()
{
    mCamera->unregisterNotificationCallback(&cHySpexSWIR_384_Model_direct::handleStatusCallback);
}

void cHySpexSWIR_384_Model_direct::updateViews()
{
    cHySpexSWIR_384_Model::updateViews();
}

bool cHySpexSWIR_384_Model_direct::configure(const nlohmann::json& jsonCfg)
{
    // Read configuration parameters first...
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

        logMessage(logERROR, msg);

        setStatus(sensor::eStatus::FAILED);

        return false;
    }

    mID = mCamera->getId();
    mSerialNumber = mCamera->getSerialNumber();

    mCamera->registerNotificationCallback(&cHySpexSWIR_384_Model_direct::handleStatusCallback, this);

    mLenses.clear();
    auto n = mCamera->getLensCount();
    for (unsigned int l = 0; l < n; ++l)
    {
        mLenses.emplace_back(mCamera->getLensNameFromId(l));
    }

    setStatus(sensor::eStatus::CONFIGURED);

    return true;
}

bool cHySpexSWIR_384_Model_direct::initialize()
{
    emit statusMessage("Retrieving HySpex SWIR-384 camera configuration...");

    mCamera->resetFramegrabber();

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
        emit statusMessage("HySpex SWIR-384 init pending detection...");
        setStatus(sensor::eStatus::PENDING);
        break;
    case hyspex::InitStatus::HYSPEX_INIT_PENDING_ELECTRONICS:
        emit statusMessage("HySpex SWIR-384 init pending electronics...");
        setStatus(sensor::eStatus::PENDING);
        break;
    case hyspex::InitStatus::HYSPEX_INIT_PENDING_SENSOR:
        emit statusMessage("HySpex SWIR-384 init pending sensor...");
        setStatus(sensor::eStatus::PENDING);
        break;
    case hyspex::InitStatus::HYSPEX_INIT_PENDING_TRANSPORT:
        emit statusMessage("HySpex SWIR-384 init pending transport...");
        setStatus(sensor::eStatus::PENDING);
        break;
    case hyspex::InitStatus::HYSPEX_INIT_NOT_STARTED:
        mCamera->init(mNumBuffersRaw, mNumBufferPreProcessing);
        emit statusMessage("HySpex SWIR-384 init not started...");
        setStatus(sensor::eStatus::PENDING);
        break;
    case hyspex::InitStatus::HYSPEX_INIT_FAILED_DETECTION:
    case hyspex::InitStatus::HYSPEX_INIT_FAILED_ELECTRONICS:
    case hyspex::InitStatus::HYSPEX_INIT_FAILED_SENSOR:
    case hyspex::InitStatus::HYSPEX_INIT_FAILED_TRANSPORT:
        {
            QString msg = "SWIR-384: ";
            msg += to_string(mInitStatus).c_str();
            logMessage(logERROR, msg);

            qCritical() << msg;

            setStatus(sensor::eStatus::FAILED);
            return false;
        }
    }

    if (mLens != mCamera->getLensName())
    {
        auto n = mCamera->getLensCount();

        for (unsigned int l = 0; l < n; ++l)
        {
            auto lens = mCamera->getLensNameFromId(l);
            if (lens == mLens)
            {
                try
                {
                    mCamera->useLensId(l);
                }
                catch (const HySpexConnect::hyspex_exception& e)
                {
                    QString msg = "Error in the setting SWIR-384 lens id: ";
                    msg.append(e.what());
                    qCritical() << msg;

                    logMessage(logERROR, msg);

                    setStatus(sensor::eStatus::FAILED);

                    return false;
                }
                break;
            }
        }
    }

    mLens = mCamera->getLensName();
    mWorkingDistance_cm = mCamera->getLensWorkingDistance_cm();
    mFieldOfView_deg = mCamera->getLensFieldOfView_rad() * nConstants::RAD_TO_DEG;
    emit lensInfoChanged();

    mWavelengthRangeId = mCamera->getWavelengthRangeId();

    mSpectralSize = mCamera->getSpectralSize();
    mSpatialDistributionSpectralBand = mSpectralSize / 2;

    mSpatialSize = mCamera->getSpatialSize();
    mSpectralDistributionSpatialChannel = mSpatialSize / 2;

    mMaxSpatialSize = mCamera->getMaxSpatialSize();
    mMaxSpectralSize = mCamera->getMaxSpectralSize();

    mMaxPixelValue = mCamera->getMaxPixelValue();
    if (mMaxPixelValue > 10)
        mSaturationValue = mMaxPixelValue - 10;

    mCommStatus = mCamera->getCommunicationStatus();
    emit commStatusChanged();

	mCoolingStatus = mCamera->getCoolingStatus();
    emit coolingStatusChanged();

    mShutterStatus = mCamera->getShutterStatus();
    emit shutterStatusChanged();

    auto averageFrames = mCamera->getAverageFrames();
    if ((mAverageFrames > 0) && (mAverageFrames != averageFrames))
    {
        mCamera->setAverageFrames(mAverageFrames);
    }

	mAverageFrames = mCamera->getAverageFrames();
    emit avgFramesChanged(mAverageFrames);

    auto framePeriod_us = mCamera->getFramePeriod_us();
    mMinFramePeriod_us = mCamera->getMinimumFramePeriod_us();
    emit minFramePeriodChanged(mMinFramePeriod_us);

    if ((mFramePeriod_us > 0) && (mFramePeriod_us != framePeriod_us) && (mFramePeriod_us >= mMinFramePeriod_us))
    {
        // We might need to reset the integration time based on the request frame period!
        auto maxIntegrationTime_us = mMaxIntegrationTime_us;
        mMaxIntegrationTime_us = mCamera->getMaxIntegrationTime_us(mFramePeriod_us);

        auto integrationTime_us = mCamera->getIntegrationTime_us();

        if (integrationTime_us > mMaxIntegrationTime_us)
        {
            integrationTime_us = mMaxIntegrationTime_us;
            mCamera->setIntegrationTime_us(integrationTime_us);
        }

        mCamera->setFramePeriod_us(mFramePeriod_us);
    }

    mFramePeriod_us = mCamera->getFramePeriod_us();
    if (mFramePeriod_us < mMinFramePeriod_us)
    {
        mCamera->setFramePeriod_us(mMinFramePeriod_us);
        mFramePeriod_us = mCamera->getFramePeriod_us();
    }
    emit framePeriodChanged(mFramePeriod_us);

    std::uint32_t integrationTime_us = mCamera->getIntegrationTime_us();
    mMaxIntegrationTime_us = mCamera->getMaxIntegrationTime_us(mFramePeriod_us);
    emit maxIntegrationTimeChanged(mMaxIntegrationTime_us);

    if ((mIntegrationTime_us > 0) && (mIntegrationTime_us != integrationTime_us) && (mIntegrationTime_us < mMaxIntegrationTime_us))
    {
        mCamera->setIntegrationTime_us(mIntegrationTime_us);
    }

    mIntegrationTime_us = mCamera->getIntegrationTime_us();
    if (mIntegrationTime_us > mMaxIntegrationTime_us)
    {
        mCamera->setIntegrationTime_us(mMaxIntegrationTime_us);
        mIntegrationTime_us = mCamera->getIntegrationTime_us();
    }
    emit integrationTimeChanged(mIntegrationTime_us);

    mAmbientTemp_C = mCamera->getAmbientTemperature_C();
    emit ambientTempChanged(mAmbientTemp_C);
	
    mSensorTemp_C = mCamera->getSensorTemperature_C();
    emit sensorTempChanged(mSensorTemp_C);

    mNumBackgrounds = mCamera->getNumberOfBackgrounds();
	mBackgroundStatus = mCamera->getBackgroundStatus();
    emit bgStatusChanged();

	mAcquisitionStatus = mCamera->getAcquisitionStatus();
    emit acqStatusChanged();

    mBadPixelCorrectionData  = mCamera->getBadPixelsWithCalculatedCorrections();
    mResponsivityMatrix      = mCamera->getResponsivityMatrix();
    mQuantumEfficiencyData   = mCamera->getQuantumEfficiencyData();

    mSpectralCalibrationPerBand = mCamera->getFullSpectralCalibrationPerBand();

    mSerializer.setBufferCapacity(mResponsivityMatrix.size() * sizeof(float) + 1024);

    return cHySpexSWIR_384_Model::initialize();
}

bool cHySpexSWIR_384_Model_direct::startCommunications()
{
    mTemperatureUpdateTimer.reset();
    mCamera->initAcquisition();

//    mCamera->registerImageCallback(&cHySpexSWIR_384_Model_direct::handleImageCallback, hyspex::ImageOptions::HYSPEX_RAW, this);
    mCamera->registerImageCallback(&cHySpexSWIR_384_Model_direct::handleImageCallback, hyspex::ImageOptions::HYSPEX_RE, this);

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

        setAcquisitionParameters(mAverageFrames, mFramePeriod_us, mIntegrationTime_us);
        calcBackground();
    }
    else
        setStatus(sensor::eStatus::FAILED);

    return mConnected;
}

void cHySpexSWIR_384_Model_direct::stopCommunications()
{
    mTemperatureUpdateTimer.stop();
    mCamera->stopAcquisition();

    mAcquisitionStatus = mCamera->getAcquisitionStatus();
    emit acqStatusChanged();

    mConnected = false;

    mCamera->unregisterImageCallback(&cHySpexSWIR_384_Model_direct::handleImageCallback);

    setStatus(sensor::eStatus::STOPPED);
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

    if (mBgCurrentState != eBgStates::NONE)
    {
        switch (mBgCurrentState)
        {
        case eBgStates::SH_CLOSE:
        {
            if (mShutterStatus == hyspex::ShutterStatus::HYSPEX_SHUTTER_CLOSED)
            {
                mBackgroundStatus = hyspex::BackgroundStatus::HYSPEX_BG_PENDING;
                unsigned int timeout_ms = ((mNumBackgrounds + 10) * mFramePeriod_us) / 1000;
                mCamera->calculateBackgroundAsync(timeout_ms, mNumBackgrounds);
                mBgCurrentState = eBgStates::STARTED;
            }
            break;
        }
        case eBgStates::STARTED:
        {
            auto oldBackgroundStatus = mBackgroundStatus;

            mBackgroundStatus = mCamera->getBackgroundStatus();
            switch (mBackgroundStatus)
            {
            case hyspex::BackgroundStatus::HYSPEX_BG_VALID:
            {
                mCamera->stopCalculatingBackground();
                mBackgroundMatrix = mCamera->getBackgroundMatrix();
                mCamera->openShutter();
                mBgCurrentState = eBgStates::SH_OPEN;

                if (mIsRecording && mSerializer)
                {
                    mSerializer.writeNumOfBackgrounds(device_id(), mNumBackgrounds);
                    mSerializer.writeBackgroundMatrix(device_id(), mBackgroundMatrix);
                }

                break;
            }
            case hyspex::BackgroundStatus::HYSPEX_BG_ABORTED:
            {
                mCamera->openShutter();
                mBgCurrentState = eBgStates::SH_OPEN;
                break;
            }
            }

            if (oldBackgroundStatus != mBackgroundStatus)
            {
                emit bgStatusChanged();
            }

            break;
        }
        case eBgStates::SH_OPEN:
        {
            if (mShutterStatus == hyspex::ShutterStatus::HYSPEX_SHUTTER_OPEN)
            {
                mBgCurrentState = eBgStates::NONE;
                emit backgroundComplete();
            }
            break;
        }
        case eBgStates::ABORT:
        {
            mCamera->stopCalculatingBackground();
            mCamera->openShutter();
            mBgCurrentState = eBgStates::SH_OPEN;
            break;
        }
        }

        return;
    }
}

void cHySpexSWIR_384_Model_direct::enableDataRecording(cBlockDataFileWriter& file)
{
    mCamera->openShutter();
    mBgCurrentState = eBgStates::NONE;

    bool computeStateChange = meComputeData != eCompute::NONE;
    meComputeData = eCompute::NONE;

    if (computeStateChange)
        emit computeModeChanged();

    cHySpexSWIR_384_Model::enableDataRecording(file);
}

void cHySpexSWIR_384_Model_direct::disableDataRecording()
{
    cHySpexSWIR_384_Model::disableDataRecording();
}

void cHySpexSWIR_384_Model_direct::writeDataHeader()
{
    cHySpexSWIR_384_Model::writeDataHeader();

    if (!mBackgroundMatrix.empty())
    {
        auto age_ms = mCamera->getBackgroundMatrixAge_ms();
        mSerializer.writeBackgroundMatrixAge_ms(device_id(), age_ms);
        mSerializer.writeBackgroundMatrix(device_id(), mBackgroundMatrix);
    }
}

void cHySpexSWIR_384_Model_direct::setAcquisitionParameters(std::uint16_t avg_frames,
    std::uint32_t frame_period_us, std::uint32_t integration_time_us)
{
    mCamera->stopAcquisition();

    auto prevAverageFrames = mAverageFrames;
    mCamera->setAverageFrames(avg_frames);
    mAverageFrames = mCamera->getAverageFrames();

    auto prevMaxIntegrationTime_us = mMaxIntegrationTime_us;
    mMaxIntegrationTime_us = mCamera->getMaxIntegrationTime_us(frame_period_us);
    if (integration_time_us > mMaxIntegrationTime_us)
        integration_time_us = mMaxIntegrationTime_us;

    auto prevIntegrationTime_us = mIntegrationTime_us;
    auto prevMinFramePeriod_us = mMinFramePeriod_us;
    mCamera->setIntegrationTime_us(integration_time_us);
    mIntegrationTime_us = mCamera->getIntegrationTime_us();
    mMinFramePeriod_us = mCamera->getMinimumFramePeriod_us();

    if (frame_period_us < mMinFramePeriod_us)
        frame_period_us = mMinFramePeriod_us;

    auto prevFramePeriod_us = mFramePeriod_us;
    mCamera->setFramePeriod_us(frame_period_us);
    mFramePeriod_us = mCamera->getFramePeriod_us();

    mCamera->startAcquisition();

    if (prevAverageFrames != mAverageFrames)
        emit avgFramesChanged(mAverageFrames);

    if (prevMaxIntegrationTime_us != mMaxIntegrationTime_us)
        emit maxIntegrationTimeChanged(mMaxIntegrationTime_us);

    if (prevMinFramePeriod_us != mMinFramePeriod_us)
        emit minFramePeriodChanged(mMinFramePeriod_us);

    if (prevFramePeriod_us != mFramePeriod_us)
        emit framePeriodChanged(mFramePeriod_us);

    if (prevIntegrationTime_us != mIntegrationTime_us)
        emit integrationTimeChanged(mIntegrationTime_us);
}

void cHySpexSWIR_384_Model_direct::setNumOfBackgrounds(int num_backgrounds)
{
    if (num_backgrounds < 0) num_backgrounds = 0;
    if (num_backgrounds > 1000) num_backgrounds = 1000;
    mCamera->setNumberOfBackgrounds(num_backgrounds);
    mNumBackgrounds = mCamera->getNumberOfBackgrounds();
}

void cHySpexSWIR_384_Model_direct::calcBackground()
{
    mCamera->closeShutter();

    // Sometimes calculating a background gets stuck in the pending stage.
    if (mBackgroundStatus == hyspex::BackgroundStatus::HYSPEX_BG_PENDING)
    {
        mCamera->stopCalculatingBackground();
    }

    mBgCurrentState = eBgStates::SH_CLOSE;
}

void cHySpexSWIR_384_Model_direct::stopBackground()
{
    mBgCurrentState = eBgStates::ABORT;
}

void cHySpexSWIR_384_Model_direct::open_shutter()
{
    mCamera->openShutter();
}

void cHySpexSWIR_384_Model_direct::close_shutter()
{
    mCamera->closeShutter();
}

void cHySpexSWIR_384_Model_direct::computePercentSaturation(bool compute)
{
    mCamera->openShutter();
    cHySpexSWIR_384_Model::computePercentSaturation(compute);
}

void cHySpexSWIR_384_Model_direct::computePercentBand(bool compute)
{
    mCamera->openShutter();
    cHySpexSWIR_384_Model::computePercentBand(compute);
}

void cHySpexSWIR_384_Model_direct::computeFocus(bool compute)
{
    mCamera->openShutter();

    mFocusAverageCount = 0;
    mFocusMatrix.resize(mSpatialSize, mSpectralSize);
    mFocusMatrix.zero();

    cHySpexSWIR_384_Model::computeFocus(compute);
}

void cHySpexSWIR_384_Model_direct::computeSpatialDistribution(bool compute)
{
    mCamera->openShutter();

    mSpatialDistributionAverageCount = 0;

    mSpatialDistWorkingData.resize(mSpatialSize);
    mSpatialDistWorkingData.zero();

    cHySpexSWIR_384_Model::computeSpatialDistribution(compute);
}

void cHySpexSWIR_384_Model_direct::computeSpectralDistribution(bool compute)
{
    mCamera->openShutter();

    mSpectralDistributionAverageCount = 0;

    mSpectralDistWorkingData.resize(mSpectralSize);
    mSpectralDistWorkingData.zero();

    cHySpexSWIR_384_Model::computeSpectralDistribution(compute);
}

/********************************************************************
 *  Status Callback Methods
 ********************************************************************/

void cHySpexSWIR_384_Model_direct::handleStatusCallback(void* p, int eventId, int value)
{
    using namespace hyspex;

    if (!p) return;

    cHySpexSWIR_384_Model_direct* self = static_cast<cHySpexSWIR_384_Model_direct*>(p);

    switch (static_cast<EventType>(eventId))
    {
    case HYSPEX_EVENT_ACQUISITION_STATUS_CHANGED:    //!< Acquisition status changed, check Camera::getAcquisitionStatus().
        self->updateAcquisitionStatus(static_cast<AcquisitionStatus>(value));
        break;
    case HYSPEX_EVENT_COOLING_STATUS_CHANGED:        //!< Cooling status changed, check Camera::getCoolingStatus().
        self->updateCoolingStatus(static_cast<CoolingStatus>(value));
        break;
    case HYSPEX_EVENT_BACKGROUND_STATUS_CHANGED:     //!< Background status changed, check Camera::getBackgroundStatus().
        self->updateBackgroundStatus(static_cast<BackgroundStatus>(value));
        break;
    case HYSPEX_EVENT_INIT_STATUS_CHANGED:           //!< Init status changed, check Camera::getInitStatus().
        self->updateInitStatus(static_cast<InitStatus>(value));
        break;
    case HYSPEX_EVENT_SHUTTER_STATUS_CHANGED:        //!< Shutter status changed, check Camera::getShutterStatus();
        self->updateShutterStatus(static_cast<ShutterStatus>(value));
        break;
    case HYSPEX_EVENT_COMMUNICATION_STATUS_CHANGED:  //!< Communication status changed. check Camera::getCommunicationStatus().
        self->updateCommStatus(static_cast<CommunicationStatus>(value));
        break;
    case HYSPEX_EVENT_MANUAL_SHUTTER_OPEN_REQUEST:   //!< Manual Shutter: open requested.
    case HYSPEX_EVENT_MANUAL_SHUTTER_CLOSE_REQUEST:  //!< Manual Shutter: close requested.
    case HYSPEX_EVENT_ERROR:                         //!< Future: unused.
    case HYSPEX_EVENT_WARNING:                       //!< Future: unused.
        break;
    }
}

void cHySpexSWIR_384_Model_direct::updateInitStatus(hyspex::InitStatus status)
{
    mInitStatus = status;
    emit initStatusChanged();
}

void cHySpexSWIR_384_Model_direct::updateCommStatus(hyspex::CommunicationStatus status)
{
    mCommStatus = status;
    emit commStatusChanged();
}

void cHySpexSWIR_384_Model_direct::updateCoolingStatus(hyspex::CoolingStatus status)
{
    mCoolingStatus = status;

    if (getStatus() == sensor::eStatus::BUSY)
    {
        if ((mCoolingStatus == hyspex::CoolingStatus::HYSPEX_COOLING_STABLE_OK) ||
            (mCoolingStatus == hyspex::CoolingStatus::HYSPEX_COOLING_STABLE_DEGRADED))
            setStatus(sensor::eStatus::RUNNING);
    }
    emit coolingStatusChanged();
}

void cHySpexSWIR_384_Model_direct::updateBackgroundStatus(hyspex::BackgroundStatus status)
{
    mBackgroundStatus = status;
    emit bgStatusChanged();
}

void cHySpexSWIR_384_Model_direct::updateAcquisitionStatus(hyspex::AcquisitionStatus status)
{
    mAcquisitionStatus = status;
    emit acqStatusChanged();
}

void cHySpexSWIR_384_Model_direct::updateShutterStatus(hyspex::ShutterStatus status)
{
    mShutterStatus = status;
    emit shutterStatusChanged();
}

void cHySpexSWIR_384_Model_direct::updateImageData(hyspex::ImageOptions a_options, const hyspex::ImageLine< unsigned short >& a_image)
{
    switch (meComputeData)
    {
    default:
    case eCompute::NONE:
        break;
    case eCompute::PERCENT_SATURATION:
    {
        auto n = a_image.saturated.size;
        auto spectral_size = a_image.spectral_size;

        const std::lock_guard<std::mutex> lock(mSaturationLock);

        mNumSaturated.resize(n);
        mMaxIntensity_pct.resize(n);
        for (uint64_t i = 0; i < n; ++i)
        {
            mNumSaturated[i]     = a_image.saturated.data[i];
            mMaxIntensity_pct[i] = (100.0f * a_image.max_saturation.data[i]) / mMaxPixelValue;
        }

        emit newPercentSaturationData();
        break;
    }
    case eCompute::PERCENT_BAND:
    {
        auto spatial_size = a_image.spatial_size;
        auto spectral_size = a_image.spectral_size;

        auto image = HySpexConnect::spatial_major_data_view<unsigned short>(a_image.buffer.data, a_image.buffer.size, spatial_size, spectral_size);
        auto num_bands = image.num_bands();

        const std::lock_guard<std::mutex> lock(mSaturationLock);

        mNumSaturated.resize(num_bands);
        mMaxIntensity_pct.resize(num_bands);
        for (std::size_t b = 0; b < num_bands; ++b)
        {
            int count = 0;
            uint16_t max_value = 0;
            auto channels = image.band(b);
            for (auto value : channels)
            {
                if (value >= mSaturationValue)
                    ++count;
                if (value >= max_value)
                    max_value = value;
            }
            mNumSaturated[b] = count;
            mMaxIntensity_pct[b] = (100.0f * max_value) / mMaxPixelValue;
        }

        emit newPercentBandData();
        break;
    }
    case eCompute::FOCUS:
    {
        ++mFocusAverageCount;
        mFocusMatrix += HySpexConnect::spatial_major_data_view<unsigned short>(a_image.buffer.data,
            a_image.buffer.size, a_image.spatial_size, a_image.spectral_size);

        if (mFocusAverageCount >= mFocusAverageMaxCount)
        {
            mFocusMatrix /= mFocusAverageCount;
            computeFocusNumber(mFocusMatrix);
            mFocusAverageCount = 0;
            mFocusMatrix.zero();
        }

        break;
    }
    case eCompute::SPATIAL_DISTRIBUTION:
    {
        ++mSpatialDistributionAverageCount;

        auto spatial_size = a_image.spatial_size;
        auto spectral_size = a_image.spectral_size;

        auto image = HySpexConnect::spatial_major_data_view<unsigned short>(a_image.buffer.data, a_image.buffer.size, spatial_size, spectral_size);

        auto view = image.band(mSpatialDistributionSpectralBand);
        mSpatialDistWorkingData += view;

        if (mSpatialDistributionAverageCount >= mSpatialDistributionAverageMaxCount)
        {
            mSpatialDistWorkingData /= mSpatialDistributionAverageCount;
            mSpatialDistributionData = mSpatialDistWorkingData;
            emit newSpatialDistributionData();

            mSpatialDistributionAverageCount = 0;
            mSpatialDistWorkingData.zero();
        }

        break;
    }
    case eCompute::SPECTRAL_DISTRIBUTION:
    {
        ++mSpectralDistributionAverageCount;

        auto spatial_size = a_image.spatial_size;
        auto spectral_size = a_image.spectral_size;

        auto image = HySpexConnect::spatial_major_data_view<unsigned short>(a_image.buffer.data, a_image.buffer.size, spatial_size, spectral_size);

        mSpectralDistWorkingData += image.channel(mSpectralDistributionSpatialChannel);

        if (mSpectralDistributionAverageCount >= mSpectralDistributionAverageMaxCount)
        {
            mSpectralDistWorkingData /= mSpectralDistributionAverageCount;
            mSpectralDistributionData = mSpectralDistWorkingData;
            emit newSpectralDistributionData();

            mSpectralDistributionAverageCount = 0;
            mSpectralDistWorkingData.zero();
        }

        break;
    }
    }

    if (mIsRecording && mSerializer)
    {
        mImageData = HySpexConnect::image_data_view<unsigned short>(a_image);

        mSerializer.writeImage(device_id(), mImageData);
    }

//    auto n = a_image.saturated.size;
//    mSaturationLevel.resize(n);
//    for (uint64_t i = 0; i < n; ++i)
//        mSaturationLevel[i] = a_image.saturated.data[i];

    emit newImageData();
}



