
#include "HySpexVNIR_3000N_Model_direct.hpp"
#include "Constants.hpp"

#include <HySpexConnect/hyspex_utils.hpp>
#include <HySpexConnect/VNIR3000N.hpp>

#include <QDebug>

#include <optional>

using namespace hyspex;


void cHySpexVNIR_3000N_Model_direct::handleImageCallback(void* p, hyspex::ImageOptions a_options, const hyspex::ImageLine< unsigned short >& a_image)
{
    using namespace hyspex;

    if (!p) return;

    auto* self = static_cast<cHySpexVNIR_3000N_Model_direct*>(p);

    self->updateImageData(a_options, a_image);
}


cHySpexVNIR_3000N_Model_direct::cHySpexVNIR_3000N_Model_direct(std::unique_ptr<hyspex::cVNIR3000N> camera, QObject* parent)
:
    cHySpexVNIR_3000N_Model(parent), mCamera(std::move(camera))
{
    mTemperatureUpdateTimer.interval_sec(5);
}

cHySpexVNIR_3000N_Model_direct::~cHySpexVNIR_3000N_Model_direct()
{
    mCamera->unregisterNotificationCallback(&cHySpexVNIR_3000N_Model_direct::handleStatusCallback);
}


void cHySpexVNIR_3000N_Model_direct::updateViews()
{
    cHySpexVNIR_3000N_Model::updateViews();
}

bool cHySpexVNIR_3000N_Model_direct::configure(const nlohmann::json& jsonCfg)
{
    mID = mCamera->getId();
    mSerialNumber = mCamera->getSerialNumber();

    mCamera->registerNotificationCallback(&cHySpexVNIR_3000N_Model_direct::handleStatusCallback, this);

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
    if (mMaxPixelValue > 2)
        mSaturationValue = mMaxPixelValue - 2;

    mCommStatus = mCamera->getCommunicationStatus();
    emit commStatusChanged();

	mCoolingStatus = mCamera->getCoolingStatus();
    emit coolingStatusChanged();

    mShutterStatus = mCamera->getShutterStatus();
    emit shutterStatusChanged();

	mAverageFrames = mCamera->getAverageFrames();
    emit avgFramesChanged(mAverageFrames);

	mFramePeriod_us = mCamera->getFramePeriod_us();
    mMinFramePeriod_us = mCamera->getMinimumFramePeriod_us();
    emit minFramePeriodChanged(mMinFramePeriod_us);

    if (mFramePeriod_us < mMinFramePeriod_us)
    {
        mCamera->setFramePeriod_us(mMinFramePeriod_us);
        mFramePeriod_us = mCamera->getFramePeriod_us();
    }
    emit framePeriodChanged(mFramePeriod_us);

    mIntegrationTime_us = mCamera->getIntegrationTime_us();
    mMaxIntegrationTime_us = mCamera->getMaxIntegrationTime_us(mFramePeriod_us);
    emit maxIntegrationTimeChanged(mMaxIntegrationTime_us);

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

    mCamera->registerImageCallback(&cHySpexVNIR_3000N_Model_direct::handleImageCallback, hyspex::ImageOptions::HYSPEX_RAW, this);

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

    mCamera->unregisterImageCallback(&cHySpexVNIR_3000N_Model_direct::handleImageCallback);

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
                mCamera->getBackgroundMatrix();
                mCamera->openShutter();
                mBgCurrentState = eBgStates::SH_OPEN;
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
        }

        return;
    }
}

void cHySpexVNIR_3000N_Model_direct::enableDataRecording(cBlockDataFileWriter& file)
{
    mCamera->openShutter();
    mBgCurrentState = eBgStates::NONE;
    cHySpexVNIR_3000N_Model::enableDataRecording(file);
}

void cHySpexVNIR_3000N_Model_direct::disableDataRecording()
{
    cHySpexVNIR_3000N_Model::disableDataRecording();
}

void cHySpexVNIR_3000N_Model_direct::writeDataHeader()
{
}

void cHySpexVNIR_3000N_Model_direct::setAcquisitionParameters(std::uint16_t avg_frames,
    std::uint32_t frame_period_us, std::uint32_t integration_time_us)
{
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

    // Sometimes calculating a background gets stuck in the pending stage.
    if (mBackgroundStatus == hyspex::BackgroundStatus::HYSPEX_BG_PENDING)
    {
        mCamera->stopCalculatingBackground();
    }

    mBgCurrentState = eBgStates::SH_CLOSE;
}


void cHySpexVNIR_3000N_Model_direct::open_shutter()
{
    mCamera->openShutter();
}

void cHySpexVNIR_3000N_Model_direct::close_shutter()
{
    mCamera->closeShutter();
}

void cHySpexVNIR_3000N_Model_direct::computePercentSaturation(bool compute)
{
    mCamera->openShutter();
    cHySpexVNIR_3000N_Model::computePercentSaturation(compute);
}

void cHySpexVNIR_3000N_Model_direct::computePercentBand(bool compute)
{
    mCamera->openShutter();
    cHySpexVNIR_3000N_Model::computePercentBand(compute);
}

void cHySpexVNIR_3000N_Model_direct::computeFocus(bool compute)
{
    mCamera->openShutter();
    cHySpexVNIR_3000N_Model::computeFocus(compute);
}

/********************************************************************
 *  Status Callback Methods
 ********************************************************************/

void cHySpexVNIR_3000N_Model_direct::handleStatusCallback(void* p, int eventId, int value)
{
    using namespace hyspex;

    if (!p) return;

    cHySpexVNIR_3000N_Model_direct* self = static_cast<cHySpexVNIR_3000N_Model_direct*>(p);

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

void cHySpexVNIR_3000N_Model_direct::updateInitStatus(hyspex::InitStatus status)
{
    mInitStatus = status;
    emit initStatusChanged();
}

void cHySpexVNIR_3000N_Model_direct::updateCommStatus(hyspex::CommunicationStatus status)
{
    mCommStatus = status;
    emit commStatusChanged();
}

void cHySpexVNIR_3000N_Model_direct::updateCoolingStatus(hyspex::CoolingStatus status)
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

void cHySpexVNIR_3000N_Model_direct::updateBackgroundStatus(hyspex::BackgroundStatus status)
{
    mBackgroundStatus = status;
    emit bgStatusChanged();
}

void cHySpexVNIR_3000N_Model_direct::updateAcquisitionStatus(hyspex::AcquisitionStatus status)
{
    mAcquisitionStatus = status;
    emit acqStatusChanged();
}

void cHySpexVNIR_3000N_Model_direct::updateShutterStatus(hyspex::ShutterStatus status)
{
    mShutterStatus = status;
    emit shutterStatusChanged();
}

void cHySpexVNIR_3000N_Model_direct::updateImageData(hyspex::ImageOptions a_options, const hyspex::ImageLine< unsigned short >& a_image)
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

        const std::lock_guard<std::mutex> lock(mPercentSaturationLock);

        mPercentSaturation.resize(n);
        for (uint64_t i = 0; i < n; ++i)
            mPercentSaturation[i] = (100.0f * a_image.saturated.data[i]) / spectral_size;

        emit newPercentSaturationData();
        break;
    }
    case eCompute::PERCENT_BAND:
    {
        auto spatial_size = a_image.spatial_size;
        auto spectral_size = a_image.spectral_size;

        auto image = HySpexConnect::spatial_major_data<unsigned short>(a_image.buffer.data, a_image.buffer.size, spatial_size, spectral_size);
        auto num_bands = image.num_bands();

        const std::lock_guard<std::mutex> lock(mPercentBandLock);

        mPercentBand.resize(num_bands);
        for (std::size_t b = 0; b < num_bands; ++b)
        {
            int count = 0;
            auto band = image.channels(b);
            for (auto value : band)
            {
                if (value >= mSaturationValue)
                    ++count;
            }
            mPercentBand[b] = (100.0f * count) / spatial_size;
        }

        emit newPercentBandData();
        break;
    }
    case eCompute::FOCUS:
    {
        emit newFocusData();
        break;
    }
    }
//    auto n = a_image.saturated.size;
//    mSaturationLevel.resize(n);
//    for (uint64_t i = 0; i < n; ++i)
//        mSaturationLevel[i] = a_image.saturated.data[i];

    emit newImageData();
}



